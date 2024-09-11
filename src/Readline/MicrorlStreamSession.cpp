/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "MicrorlStreamSession.hpp"

int Stm32Shell::Readline::MicrorlStreamSession::executeCallback(int argc, const char *const *argv) {
    log()->print("Tokens found: ");
    for (int i = 0; i < argc; i++) {
        log()->printf("{%s} ", argv[i]);
    }
    log()->println();

    return 0;
}
