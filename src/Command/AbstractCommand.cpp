/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "AbstractCommand.hpp"
#include "Helper.hpp"

using namespace Stm32Shell::Command;

AbstractCommand::AbstractCommand() {
    // auto ret = Parser::registerCommand(this);
    // assert_param(ret == Parser::registerCommandReturn::SUCCESS);
}

void AbstractCommand::setCommandLine(const char *cmdLine, const size_t size) {
    auto sz = std::min(size, sizeof commandLine - 1);
    memset(commandLine, 0, sizeof commandLine);
    memcpy(commandLine, cmdLine, sz);
}

const char * AbstractCommand::getCommandLine() {
    return commandLine;
}

void AbstractCommand::setRunTimeout(unsigned long timeout) {
    runTimeout = timeout;
}

void AbstractCommand::setQuiet(bool quiet) {
    quietRun = quiet;
}

bool AbstractCommand::isQuietRun() const {
    return quietRun;
}

void AbstractCommand::setContext(CommandContextInterface *cmdContext) {
    ctx = cmdContext;
}

bool AbstractCommand::write(const char *str) {
    if (ctx == nullptr) return false;
    return ctx->cmdOutputBuffer.write(str);
}

bool AbstractCommand::write(const void *in, size_t strlen) {
    if (ctx == nullptr) return false;
    return ctx->cmdOutputBuffer.write(static_cast<const uint8_t *>(in), strlen);
}

bool AbstractCommand::printf(const char *format, ...) {
    if (ctx == nullptr) return false;
    va_list args;
    va_start(args, format);
    auto ret = ctx->cmdOutputBuffer.vprintf(format, args);
    va_end(args);
    return ret;
}

CommandContextInterface::cmdOutputBufferClass * AbstractCommand::out() {
    return &ctx->cmdOutputBuffer;
}

void AbstractCommand::setParam(char paramName, const char *paramString) {
    setParam(paramName, strtol(paramString, nullptr, 10));
    setParam(paramName, strtod(paramString, nullptr));
}

