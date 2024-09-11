/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_COMMAND_COMMANDCONTEXT_HPP
#define LIBSMART_STM32SHELL_COMMAND_COMMANDCONTEXT_HPP

#include "CommandContextInterface.hpp"
#include "CommandInterface.hpp"
#include "Helper.hpp"
#include "StringBuffer.hpp"
#include "Loggable.hpp"



namespace Stm32Shell::ezShell {
    class Shell;
}

namespace Stm32Shell::Command {
    class AbstractCommand;

    class CommandContext : public CommandContextInterface, public Stm32ItmLogger::Loggable {
        friend ezShell::Shell;
        friend AbstractCommand;

    public:


        bool setCommand(CommandInterface *command);

        size_t outputLength() {
            return cmdOutputBuffer.getLength();
        }

        size_t outputRead(char *out, size_t size) {
            return cmdOutputBuffer.read(out, size);
        }


        bool hasError();

        bool isFinished() const;

        bool isCmdSync();

        void recycle();

        const char *getName();

        const char *getCommandLine();

        void registerOnRunFinishedFunction(const fn_t &fn) { this->onRunFinishedFn = fn; }
        void registerOnCleanupFinishedFunction(const fn_t &fn) { this->onCleanupFinishedFn = fn; }
        void registerOnCmdEndFunction(const fn_t &fn) { this->onCmdEndFn = fn; }
        void registerOnWriteFunction(const fn_t &fn) { this->onWriteFn = fn; }


        uint32_t getRunDuration() {
            return firstRunMillis > 0 ? millis() - firstRunMillis : 0;
        }

    protected:
        void do_preFlightCheck();

        void do_init();

        void do_run();

        void do_cleanup();

        void do_terminate();


        /**
         * @brief This virtual function is called when the run timeout occurs.
         *
         * This function is a placeholder that can be overridden in derived classes to implement
         * specific functionality when the run timeout occurs.
         */
        virtual void onRunTimeout();

        /**
         * @brief This virtual function is called when an error occurs during the execution of the command.
         */
        virtual void onRunError();

        /**
         * @brief Virtual function called when the execution of the command is finished.
         *
         * The function is called regardless of the error status.
         */
        virtual void onRunFinished();

        /**
         * @brief Virtual function called when the cleanup is finished.
         *
         * The function is called regardless of the error status.
         */
        virtual void onCleanupFinished();

        /**
         * @brief Virtual function called when the command has ended and is ready for deletion.
         *
         * The function is called regardless of the error status.
         */
        virtual void onCmdEnd();;

    private:
        CommandInterface *cmd{};
        using u_cmdStates = enum class cmdStates {
            UNDEF,
            PREFLIGHTCHECK,
            PREFLIGHTCHECK_DONE,
            PREFLIGHTCHECK_ERROR,
            INIT,
            INIT_DONE,
            INIT_ERROR,
            RUN,
            RUN_DONE,
            RUN_TIMEOUT,
            RUN_ERROR,
            TERMINATED
        };
        cmdStates cmdState = cmdStates::UNDEF;

        bool mustRecycle = false;

        /*
        AbstractCommand::preFlightCheckReturn preFlightCheckResult = AbstractCommand::preFlightCheckReturn::UNDEF;
        AbstractCommand::initReturn initResult = AbstractCommand::initReturn::UNDEF;
        AbstractCommand::runReturn runResult = AbstractCommand::runReturn::UNDEF;
        AbstractCommand::cleanupReturn cleanupResult = AbstractCommand::cleanupReturn::UNDEF;
        */
        // friend class cmdOutputBufferClass;

        /** The value of millis() when the run was first started. */
        unsigned long firstRunMillis = 0;
        /** The value of millis() when the last run started. */
        unsigned long lastRunMillis = 0;
        /** Total [ms] since the command started. */
        unsigned long runDuration = 0;


    };
}

#endif
