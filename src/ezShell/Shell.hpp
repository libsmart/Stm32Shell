/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_EZSHELL_SHELL_HPP
#define LIBSMART_STM32SHELL_EZSHELL_SHELL_HPP

#include "Command/CommandInterface.hpp"
#include "Command/CommandContext.hpp"
#include "Readline/AbstractMicrorlStreamSession.hpp"

#define LIBSMART_STM32SHELL_EZSHELL_MAX_PROMPT 100
#define LIBSMART_STM32SHELL_EZSHELL_MAX_CMD 20

namespace Stm32Shell::ezShell {
    class Shell : public Readline::AbstractMicrorlStreamSession {
    public:
        void setup() override;

        void setCwd(const char *cwd);

        static void registerCmd(Command::CommandInterface *cmd);

        static size_t registeredCommands();

    protected:
        int executeCallback(int argc, const char *const *argv) override;

    private:
        char prompt[LIBSMART_STM32SHELL_EZSHELL_MAX_PROMPT] = {};
        const char *cwd = prompt;

        Command::CommandContext cmdCtx;

        static std::array<Command::CommandInterface *, LIBSMART_STM32SHELL_EZSHELL_MAX_CMD> cmdRegistry;
    };
}
#endif
