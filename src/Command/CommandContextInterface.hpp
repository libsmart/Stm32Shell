/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_COMMAND_COMMANDCONTEXTINTERFACE_HPP
#define LIBSMART_STM32SHELL_COMMAND_COMMANDCONTEXTINTERFACE_HPP

#include "StringBuffer.hpp"

#define LIBSMART_STM32SHELL_COMMAND_OUTPUT_BUFFER_SIZE 256


namespace Stm32Shell::Command {
    class AbstractCommand;
}

namespace Stm32Shell::Command {
    class CommandContextInterface {
        friend AbstractCommand;

    public:
        using fn_t = std::function<void()>;

        virtual ~CommandContextInterface() = default;

    protected:
        class cmdOutputBufferClass final : public Stm32Common::StringBuffer<
                    LIBSMART_STM32SHELL_COMMAND_OUTPUT_BUFFER_SIZE> {
        public:
            cmdOutputBufferClass() = delete;

            explicit cmdOutputBufferClass(CommandContextInterface &context)
                : context(context) {
            }

        protected:
            CommandContextInterface &context;

            void onWrite() override {
                StringBuffer::onWrite();
                context.onWriteFn();
            }
        } cmdOutputBuffer{*this};

        fn_t onRunFinishedFn = []() {
        };
        fn_t onCleanupFinishedFn = []() {
        };
        fn_t onCmdEndFn = []() {
        };
        fn_t onWriteFn = []() {
        };
    };
}
#endif
