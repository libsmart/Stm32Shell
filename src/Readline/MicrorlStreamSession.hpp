/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef LIBSMART_STM32SHELL_READLINE_MICRORLSTREAMSESSION_HPP
#define LIBSMART_STM32SHELL_READLINE_MICRORLSTREAMSESSION_HPP

#include "AbstractMicrorlStreamSession.hpp"

namespace Stm32Shell::Readline {
    class MicrorlStreamSession : public AbstractMicrorlStreamSession {
    protected:
        int executeCallback(int argc, const char *const *argv) override;
    };
}
#endif
