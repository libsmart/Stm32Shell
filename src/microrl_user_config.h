/**
 * \file            microrl_user_config_template.h
 * \brief           MicroRL library user configurations
 */

/*
 * Copyright (c) 2021 Dmitry KARASEV
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is part of MicroRL - Micro Read Line library for small and embedded devices.
 *
 * Authors:         Dmitry KARASEV <karasevsdmitry@yandex.ru>
 * Version:         2.5.0
 */

#ifndef MICRORL_HDR_USER_CONFIG_H
#define MICRORL_HDR_USER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Rename this file to "microrl_user_config.h" for your application */

/*
 * Open "microrl_config.h" and copy & replace
 * here settings you want to change values
 */


/**
 * \brief           Print prompt at 'microrl_init()'. If enable, prompt will print at startup,
 *                  otherwise first prompt will print after first press 'Enter' in terminal
 * \note            Enable it, if you call 'microrl_init()' after your communication subsystem
 *                  already initialize and ready to print message
 */
#define MICRORL_CFG_PROMPT_ON_INIT            0

/**
 * \brief           Enable it, if you want to use "echo off" feature.
 *                  "Echo off" is used for typing the secret input data, like passwords.
 *                  When the feature is enabled, there are 2 type of echo off: ONCE and OFF.
 *                  Use \ref MICRORL_ECHO_ONCE to disable echo for password input, echo mode
 *                  will enabled after user press 'Enter'.
 *                  Use \ref MICRORL_ECHO_ON or \ref MICRORL_ECHO_OFF to turn on or off the
 *                  echo manually.
 *                  At library initialization echo is ON
 */
#define MICRORL_CFG_USE_ECHO_OFF              1

/**
 * \brief           Enable it, if you want to allow quoting command arguments to include spaces.
 *                  Quoting protects whitespace, for example, 2 quoted tokens:
 *                  "> set wifi "Home Net" "secret password"
 */
#define MICRORL_CFG_USE_QUOTING               1

/**
 * \brief           Enable it, if you want to use completion functional, also set completion callback in you code.
 *                  Completion functional calls 'completion' callback if user press 'TAB'.
 */
#define MICRORL_CFG_USE_COMPLETE              1

/**
 * \brief           Enable it and add an 'interrupt signal' callback to invoke it when the user presses Ctrl+C
 */
#define MICRORL_CFG_USE_CTRL_C                1

/**
 * \brief           Enable it to use user-defined pre- and post- command execute callbacks (hooks)
 */
#define MICRORL_CFG_USE_COMMAND_HOOKS         1


    typedef struct microrl microrl_t;
    typedef void (*microrl_pre_cmd_fn)(struct microrl* mrl, int argc, const char* const *argv);
    typedef void (*microrl_post_cmd_fn)(struct microrl* mrl, int res, int argc, const char* const *argv);
    microrl_pre_cmd_fn getPreCommandCallbackPointer();
    microrl_post_cmd_fn getPostCommandCallbackPointer();

    /**
     * \brief           Optional user implemented function called before command execution callback
     *                      Not called if \ref MICRORL_CFG_USE_COMMAND_HOOKS is set to 0
     * \param[in]       mrl: Pointer to microRL working instance
     * \param[in]       argc: Number of arguments in command line
     * \param[in]       argv: Pointer to argument list
     */
#define MICRORL_PRE_COMMAND_HOOK(mrl, argc, argv) getPreCommandCallbackPointer()(mrl, argc, argv)

    /**
     * \brief           Optional user implemented function called after command execution callback
     *                      Not called if \ref MICRORL_CFG_USE_COMMAND_HOOKS is set to 0
     * \param[in]       mrl: Pointer to microRL working instance
     * \param[in]       res: Return value of the command execution callback
     * \param[in]       argc: Number of arguments in command line
     * \param[in]       argv: Pointer to argument list
     */
#define MICRORL_POST_COMMAND_HOOK(mrl, res, argc, argv) getPostCommandCallbackPointer()(mrl, res, argc, argv)

/**
 * \brief           Enable it for use 'sprintf()' implementation from your compiler's standard library, but
 *                  this adds some overhead. If not enabled, that uses my own number conversion code,
 *                  which save about 800 byte of code size on AVR (avr-gcc build).
 *                  Try to build with and without, and compare total code size for tune library.
 */
#define MICRORL_CFG_USE_LIBC_STDIO            1



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* MICRORL_HDR_USER_CONFIG_H */
