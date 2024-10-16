/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_EZSHELL_COMMANDS_HELP_HPP
#define LIBSMART_STM32SHELL_EZSHELL_COMMANDS_HELP_HPP

#include "Command/AbstractCommand.hpp"
#include "ezShell/Shell.hpp"

namespace Stm32Shell::ezShell::Command {
    class Help : public Stm32Shell::Command::AbstractCommand {
    public:
        Help() {
            Nameable::setName("help");
            isSync = true;
            setLogger(&Stm32ItmLogger::logger);
        }

        runReturn run() override {
            auto ret = AbstractCommand::run();
            out()->println("HELP");
            out()->println("HELP");
            return ret;
        }
    };
}
#endif
