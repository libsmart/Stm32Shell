/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_COMMAND_COMMANDINTERFACE_HPP
#define LIBSMART_STM32SHELL_COMMAND_COMMANDINTERFACE_HPP

#include <cstdint>
#include "Nameable.hpp"
#include "CommandContextInterface.hpp"

namespace Stm32Shell::Command {
    class CommandContext;

    class CommandInterface : public Stm32Common::Nameable {
        friend CommandContext;

    public:
        ~CommandInterface() override = default;

        using u_preFlightCheckReturn = enum class preFlightCheckReturn {
            UNDEF, ERROR, READY
        };

        using u_initReturn = enum class initReturn {
            UNDEF, ERROR, READY
        };

        using u_runReturn = enum class runReturn {
            UNDEF, ERROR, TIMEOUT, RUNNING, FINISHED
        };

        using u_cleanupReturn = enum class cleanupReturn {
            UNDEF, ERROR, OK
        };

        /**
         * @brief Performs a pre-flight check on the command.
         *
         * This method is responsible for conducting any necessary checks
         * before the command execution starts. It ensures that all preconditions
         * are met and the command is ready to be executed.
         *
         * @return An enumeration value indicating the result of the pre-flight check.
         *         - READY: If the command is ready for execution.
         *         - ERROR: If an error occurred during the pre-flight check.
         *         - UNDEF: If the pre-flight check could not be completed or is undefined.
         */
        virtual preFlightCheckReturn preFlightCheck() = 0;

        /**
         * @brief Initializes the command.
         *
         * This method is responsible for running the initial setup or
         * initialization steps required for the command execution. It
         * ensures that the command is prepared and set up correctly
         * before it is run.
         *
         * @return An enumeration value indicating the result of the initialization.
         *         - READY: If the initialization completed successfully.
         *         - ERROR: If an error occurred during initialization.
         *         - UNDEF: If the initialization could not be completed or is undefined.
         */
        virtual initReturn init() = 0;

        /**
         * @brief Executes the command.
         *
         * This method is responsible for running the main logic of the command. Depending on the implementation,
         * the execution may be synchronous or asynchronous, and it may involve various stages or steps.
         *
         * @return An enumeration value indicating the result of the execution.
         *         - RUNNING: If the command is still in progress.
         *         - FINISHED: If the command completed successfully.
         *         - ERROR: If an error occurred during execution.
         *         - TIMEOUT: If the command execution timed out.
         *         - UNDEF: If the execution result is undefined or not yet determined.
         */
        virtual runReturn run() = 0;

        /**
         * @brief Cleans up resources after command execution.
         *
         * This method is responsible for performing any necessary cleanup operations
         * after the command execution has finished. This may include deallocating
         * resources, resetting states, or other finalization tasks to ensure that
         * the command and its environment return to a stable state.
         *
         * @return An enumeration value indicating the result of the cleanup operation.
         *         - OK: If the cleanup was successful.
         *         - ERROR: If an error occurred during the cleanup.
         *         - UNDEF: If the cleanup result is undefined or not yet determined.
         */
        virtual cleanupReturn cleanup() = 0;

        /**
         * @brief Terminates the command.
         *
         * This method is responsible for safely and cleanly terminating the command.
         * It ensures that any ongoing operations are halted and the command is properly shut down.
         * Subclasses should implement this method to handle specific termination logic as necessary.
         */
        virtual void terminate() = 0;

        /**
         * @brief Recycles the command for reuse.
         *
         * This pure virtual method is responsible for resetting the command's internal state
         * and preparing it for future use. Implementations of this method should ensure that
         * all necessary resources are appropriately deallocated or reset, making the command
         * ready for reinitialization and reuse without creating a new instance.
         */
        virtual void recycle() = 0;

        virtual const char *getCommandLine() = 0;

        virtual bool isCmdSync() = 0;

        virtual uint32_t getRunTimeout() = 0;

        virtual void setParam(int argc, const char *const *argv) = 0;

    protected:
        virtual void onRunTimeout() = 0;

        virtual void onRunError() = 0;

        virtual void onRunFinished() = 0;

        virtual void onCleanupFinished() = 0;

        virtual void onCmdEnd() = 0;

    private:
        virtual void setContext(CommandContextInterface *ctx) = 0;
    };
}
#endif
