/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "AbstractMicrorlStreamSession.hpp"
#include <climits>
#include <microrl.h>
#include "globals.hpp"
#include "Helper.hpp"

microrl_pre_cmd_fn getPreCommandCallbackPointer() {
    return Stm32Shell::Readline::AbstractMicrorlStreamSession::getPreCommandCallback();
}

microrl_post_cmd_fn getPostCommandCallbackPointer() {
    return Stm32Shell::Readline::AbstractMicrorlStreamSession::getPostCommandCallback();
}


using namespace Stm32Shell::Readline;

AbstractMicrorlStreamSession::~AbstractMicrorlStreamSession() {
    getRxBuffer()->clear();
    getTxBuffer()->clear();
    microrl_t{};
}

void AbstractMicrorlStreamSession::flush() {
    loop();
}

int AbstractMicrorlStreamSession::microrlOutputCb(const char *str) {
    // log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            // ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlOutputCb()");

    write(str);
    return 0;
}

int AbstractMicrorlStreamSession::microrlExecCb(int argc, const char *const *argv) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlExecCb()");

    return executeCallback(argc, argv);
}

char **AbstractMicrorlStreamSession::microrlCompleteCb(int argc, const char *const *argv) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlCompleteCb()");

    LIBSMART_UNUSED(argc);
    LIBSMART_UNUSED(argv);
    return nullptr;
}

void AbstractMicrorlStreamSession::microrlSigintCb() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlSigintCb()");
}

void AbstractMicrorlStreamSession::microrlPreCommandCb(int argc, const char *const *argv) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlPreCommandCb()");
    LIBSMART_UNUSED(argc);
    LIBSMART_UNUSED(argv);
}

void AbstractMicrorlStreamSession::microrlPostCommandCb(int res, int argc, const char *const *argv) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlPostCommandCb()");
    LIBSMART_UNUSED(res);
    LIBSMART_UNUSED(argc);
    LIBSMART_UNUSED(argv);
}

void AbstractMicrorlStreamSession::setup() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setup()");

    // Initialize microrl library
    microrlInit(
        bounce<AbstractMicrorlStreamSession, decltype(&AbstractMicrorlStreamSession::microrlOutputCb),
            &AbstractMicrorlStreamSession::microrlOutputCb, const char *>,
        bounce<AbstractMicrorlStreamSession, decltype(&AbstractMicrorlStreamSession::microrlExecCb),
            &AbstractMicrorlStreamSession::microrlExecCb, int, const char *const *>
    );

    // Set callback for auto-completion
    setCompleteCallback(bounce<AbstractMicrorlStreamSession, decltype(&AbstractMicrorlStreamSession::microrlCompleteCb),
        &AbstractMicrorlStreamSession::microrlCompleteCb, int, const char *const *>);

    // Set callback for Ctrl+C handling
    setSigintCallback(bounce<AbstractMicrorlStreamSession, decltype(&AbstractMicrorlStreamSession::microrlSigintCb),
        &AbstractMicrorlStreamSession::microrlSigintCb>);

    println();
    print(FIRMWARE_NAME);
    print(F(" v"));
    print(FIRMWARE_VERSION);
    print(F(" "));
    println(FIRMWARE_COPY);
    flush();
    // delay(500);
    print(F("OK"));
    flush();

    processingInput("\n");
}

void AbstractMicrorlStreamSession::loop() {
    while (available() > 0) {
        auto ch = read();

        if (iac == 0 && ch == 0xff) {
            // Enable IAC mode
            iac++;
            continue;
        }

        if (iac == 1 && ch == 0xff) {
            // Second IAC marks a real 0xff byte
            iac = 0;
        }

        if (iac > 0) {
            // 1 byte commands
            if (iac == 1 && ch >= 0xf0 && ch <= 0xf9) {
                iacCmd = ch;
                iac = 0;
                iacCmd = 0;
            }

            // 2 byte commands
            if (iac == 1 && ch >= 0xfb && ch <= 0xfe) {
                iacCmd = ch;
                iac++;
            }
            if (iac == 2 && iacCmd >= 0xfb && iacCmd <= 0xfe) {
                iac = 0;
                iacCmd = 0;
            }

            // multi byte commands
            if (iac == 1 && ch == 0xfa) {
                iacCmd = ch;
                iac++;
            }
            // multi byte commands end
            if (iac > 2 && ch == 0xf0) {
                iac = 0;
                iacCmd = 0;
            }
        } else {
            // Send character to microrl, if not in IAC mode
            processingInput(&ch, 1);
        }
    }
}

void AbstractMicrorlStreamSession::end() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::end()");

    getRxBuffer()->clear();
    getTxBuffer()->clear();

    microrl_t{};
    iac = 0;
    iacCmd = 0;
}

microrlr_t AbstractMicrorlStreamSession::processingInput(const void *data_ptr, size_t len) {
    // log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
    //         ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::processingInput()");

    const auto ret = microrl_processing_input(this, data_ptr, len);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_processing_input() = 0x%02x\r\n", ret);
    }
    return ret;
}

microrlr_t AbstractMicrorlStreamSession::processingInput(const char *c_string) {
    return processingInput(c_string, strlen(c_string));
}

microrlr_t AbstractMicrorlStreamSession::setEcho(microrl_echo_t echo) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setEcho()");

    const auto ret = microrl_set_echo(this, echo);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_set_echo() = 0x%02x\r\n", ret);
    }
    return ret;
}

microrlr_t AbstractMicrorlStreamSession::setPrompt(const char *prompt_str) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setPrompt()");

    const auto ret = microrl_set_prompt(this, const_cast<char *>(prompt_str));
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_set_prompt() = 0x%02x\r\n", ret);
    }
    return ret;
}

microrl_pre_cmd_fn AbstractMicrorlStreamSession::getPreCommandCallback() {
    return bounce<AbstractMicrorlStreamSession,
        decltype(&AbstractMicrorlStreamSession::microrlPreCommandCb),
        &AbstractMicrorlStreamSession::microrlPreCommandCb,
        int, const char *const *>;
}

microrl_post_cmd_fn AbstractMicrorlStreamSession::getPostCommandCallback() {
    return bounce<AbstractMicrorlStreamSession,
        decltype(&AbstractMicrorlStreamSession::microrlPostCommandCb),
        &AbstractMicrorlStreamSession::microrlPostCommandCb,
        int, int, const char *const *>;
}

microrlr_t AbstractMicrorlStreamSession::microrlInit(microrl_output_fn out_fn, microrl_exec_fn exec_fn) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::microrlInit()");

    /* Initialize library with microrl instance and print and execute callbacks */
    const auto ret = microrl_init(this, out_fn, exec_fn);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_init() = 0x%02x\r\n", ret);
    }
    return ret;
}

microrlr_t AbstractMicrorlStreamSession::setExecuteCallback(microrl_exec_fn exec_fn) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setExecuteCallback()");

    const auto ret = microrl_set_execute_callback(this, exec_fn);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_set_execute_callback() = 0x%02x\r\n", ret);
    }
    return ret;
}

microrlr_t AbstractMicrorlStreamSession::setCompleteCallback(microrl_get_compl_fn get_completion_fn) {
#if MICRORL_CFG_USE_COMPLETE
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setCompleteCallback()");

    const auto ret = microrl_set_complete_callback(this, get_completion_fn);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_set_complete_callback() = 0x%02x\r\n", ret);
    }
    return ret;
#else
#warning "MICRORL_CFG_USE_COMPLETE" is disabled
#endif
}

microrlr_t AbstractMicrorlStreamSession::setSigintCallback(microrl_sigint_fn sigint_fn) {
#if MICRORL_CFG_USE_CTRL_C
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::setSigintCallback()");

    const auto ret = microrl_set_sigint_callback(this, sigint_fn);
    if (ret != microrlOK) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("microrl_set_sigint_callback() = 0x%02x\r\n", ret);
    }
    return ret;
#else
#warning "MICRORL_CFG_USE_CTRL_C" is disabled
#endif
}

uint32_t AbstractMicrorlStreamSession::getVersion() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Shell::Readline::AbstractMicrorlStreamSession::getVersion()");

    return microrl_get_version();
}

