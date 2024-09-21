/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_COMMAND_ABSTRACTCOMMAND_HPP
#define LIBSMART_STM32SHELL_COMMAND_ABSTRACTCOMMAND_HPP

#include "CommandContext.hpp"
#include "CommandInterface.hpp"
#include "main.hpp"
#include "Loggable.hpp"

namespace Stm32Shell::Command {
    class AbstractCommand : public CommandInterface, public Stm32ItmLogger::Loggable {
        friend CommandContext;

    public:
        AbstractCommand();

        preFlightCheckReturn preFlightCheck() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::preFlightCheck()");
            return preFlightCheckReturn::READY;
        };

        initReturn init() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::init()");
            return initReturn::READY;
        };

        runReturn run() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::run()");
            return runReturn::FINISHED;
        };

        cleanupReturn cleanup() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::cleanup()");
            return cleanupReturn::OK;
        };

        void terminate() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::terminate()");
        };

        void recycle() override {
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->print(getName());
            log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
                    ->println("::recycle()");
            setCommandLine("", 0);
            argc = 0;
            argv = nullptr;
        };


        virtual void setParam(int argc, const char *const *argv);

        virtual void setParam(char paramName, const char *paramString);

        virtual void setParam(char paramName, long paramLong) {
        };

        virtual void setParam(char paramName, double paramDouble) {
        };


        virtual void setCommandLine(const char *cmdLine, size_t size);

        const char *getCommandLine() override;

        virtual const char *getName() { return "AbstractCommand"; }

        void setRunTimeout(unsigned long timeout);

        void setQuiet(bool quiet = true);

        bool isQuietRun() const;

        CommandContextInterface *getCommandContext() { return ctx; }

        virtual AbstractCommand *factory() { return new AbstractCommand; }

        bool isCmdSync() override {
            return isSync;
        }

        uint32_t getRunTimeout() override {
            return runTimeout;
        }

    protected:
        /**
         * @brief This virtual function is called when the run timeout occurs.
         *
         * This function is a placeholder that can be overridden in derived classes to implement
         * specific functionality when the run timeout occurs.
         */
        void onRunTimeout() override {
        };

        /**
         * @brief This virtual function is called when an error occurs during the execution of the command.
         */
        void onRunError() override {
        };

        /**
         * @brief Virtual function called when the execution of the command is finished.
         *
         * The function is called regardless of the error status.
         */
        void onRunFinished() override {
        };

        /**
         * @brief Virtual function called when the cleanup is finished.
         *
         * The function is called regardless of the error status.
         */
        void onCleanupFinished() override {
        };

        /**
         * @brief Virtual function called when the command has ended and is ready for deletion.
         *
         * The function is called regardless of the error status.
         */
        void onCmdEnd() override {
        };

    protected:
        bool write(const char *str);

        bool write(const void *in, size_t strlen);

        bool printf(const char *format, ...);

        CommandContextInterface::cmdOutputBufferClass *out();

    protected:
        /** true: the command is executed immediately and synchronous. */
        bool isSync = false;


        int argc = 0;
        const char *const *argv = nullptr;

    private:
        CommandContextInterface *ctx{};

        void setContext(CommandContextInterface *ctx) override;

        /** Timeout [ms] in which the command must be completed. */
        unsigned long runTimeout = 0;
        /** Quiet run: no "ok" after run */
        bool quietRun = false;
        /** Used to delay debug output */
        unsigned long lastRunDurationDebugOutput = 0;
        /** Stores the originial command line entered */
        char commandLine[32] = {};
    };
}

#endif //LIBSMART_STM32GCODERUNNER_ABSTRACTCOMMAND_HPP
