/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Shell.hpp"
#include "Command/Help.hpp"

using namespace Stm32Shell::ezShell;
using namespace Stm32Shell::Command;

std::array<CommandInterface *, LIBSMART_STM32SHELL_EZSHELL_MAX_CMD> Shell::cmdRegistry = {};

void Shell::setup() {
    AbstractMicrorlStreamSession::setup();
    // registerCmd(&Command::help);
    setCwd("/");
}

void Shell::setCwd(const char *cwd) {
    snprintf(prompt, sizeof(prompt), "[user@hostname] %s> ", cwd);
    cwd = prompt + 16;
    setPrompt(prompt);
}

void Shell::registerCmd(CommandInterface *cmd) {
    for (size_t i = 0; i < cmdRegistry.size(); i++) {
        if (cmdRegistry[i] == nullptr) {
            cmdRegistry[i] = cmd;
            Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->printf("Stm32Shell::ezShell::Shell::registerCmd %lu/%lu\r\n", registeredCommands(),
                             cmdRegistry.size());
            return;
        }
    }
}

size_t Shell::registeredCommands() {
    auto count = cmdRegistry.size();
    for (auto i = 0; i < cmdRegistry.size(); i++) {
        if (cmdRegistry[i] != nullptr) count--;
    }
    return count;
}

int Shell::executeCallback(int argc, const char *const *argv) {
    log()->setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::ezShell::Shell::executeCallback()");

    log()->print("Tokens found: ");
    for (int i = 0; i < argc; i++) {
        log()->printf("{%s} ", argv[i]);
    }
    log()->println();


    for (size_t i = 0; i < cmdRegistry.size(); i++) {
        if (cmdRegistry[i] != nullptr) {
            if (std::strcmp(cmdRegistry[i]->getName(), argv[0]) == 0) {
                // Command found
                log()->setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                        ->printf("Command found: %s\r\n", cmdRegistry[i]->getName());

                cmdCtx.setLogger(getLogger());
                cmdCtx.setCommand(cmdRegistry[i]);

                cmdRegistry[i]->setParam(argc, argv);

                cmdCtx.registerOnWriteFunction([this]() {
                    // Logger.println("onWriteFn()");
                    if (this->cmdCtx.outputLength() > 0) {
                        const auto result = this->cmdCtx.outputRead(
                            reinterpret_cast<char *>(this->getTxBuffer()->getWritePointer()),
                            this->getTxBuffer()->getRemainingSpace());
                        this->getTxBuffer()->setWrittenBytes(result);
                    }
                });

                cmdCtx.registerOnCmdEndFunction([this]() {
                    // Debugger_log(DBG, "onCmdEndFn()");
                });

                cmdCtx.do_preFlightCheck();
                cmdCtx.do_init();
                if (cmdCtx.isCmdSync()) {
                    cmdCtx.do_run();
                    cmdCtx.do_cleanup();
                    cmdCtx.recycle();
                    return 0;
                }

                cmdCtx.recycle();
                break;
            }
        }
    }

    // So something useful with the tokens

    this->getTxBuffer()->printf("ERROR: Command '%s' not found\r\n", argv[0]);

    return 0; // Everything ok
}
