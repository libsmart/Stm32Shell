/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef LIBSMART_STM32SHELL_EZSHELL_COMMANDS_INFO_HPP
#define LIBSMART_STM32SHELL_EZSHELL_COMMANDS_INFO_HPP

#include "globals.hpp"
#include "Stm32NetX.hpp"
#include "Command/AbstractCommand.hpp"
#include "ezShell/Shell.hpp"

#define OUTPUT_PAUSE do{out()->flush();delay(20);}while(false);

namespace Stm32Shell::ezShell::Command {
    class Info : public Stm32Shell::Command::AbstractCommand {
    public:
        Info() {
            Nameable::setName("info");
            isSync = true;
            setLogger(&Logger);
            Shell::registerCmd(this);
        }

        runReturn run() override {
            auto ret = AbstractCommand::run();

            out()->print(F("FIRMWARE: "));
            out()->print(FIRMWARE_NAME);
            out()->print(F(" v"));
            out()->print(FIRMWARE_VERSION);
            out()->print(F(" "));
            out()->println(FIRMWARE_COPY);
            OUTPUT_PAUSE;

            out()->print(F("FIRMWARE_NAME: "));
            out()->println(FIRMWARE_NAME);
            OUTPUT_PAUSE;

            out()->print(F("FIRMWARE_VERSION: "));
            out()->println(FIRMWARE_VERSION);
            OUTPUT_PAUSE;

            out()->print(F("FIRMWARE_BUILDTIME: "));
            out()->println(FIRMWARE_BUILDTIME);
            OUTPUT_PAUSE;

            out()->printf("HARDWARE_MAC: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                          static_cast<unsigned int>(heth.Init.MACAddr[0]),
                          static_cast<unsigned int>(heth.Init.MACAddr[1]),
                          static_cast<unsigned int>(heth.Init.MACAddr[2]),
                          static_cast<unsigned int>(heth.Init.MACAddr[3]),
                          static_cast<unsigned int>(heth.Init.MACAddr[4]),
                          static_cast<unsigned int>(heth.Init.MACAddr[5]));
            OUTPUT_PAUSE;

            ULONG ip_address, network_mask;
            Stm32NetX::NX->getIpInstance()->ipAddressGet(&ip_address, &network_mask);
            out()->printf("IP_ADDRESS: %lu.%lu.%lu.%lu\r\n",
                          (ip_address >> 24) & 0xff,
                          (ip_address >> 16) & 0xff,
                          (ip_address >> 8) & 0xff,
                          (ip_address >> 0) & 0xff
            );
            OUTPUT_PAUSE;

            out()->printf("NETWORK_MASK: %lu.%lu.%lu.%lu\r\n",
                          (network_mask >> 24) & 0xff,
                          (network_mask >> 16) & 0xff,
                          (network_mask >> 8) & 0xff,
                          (network_mask >> 0) & 0xff
            );
            OUTPUT_PAUSE;

            const auto gateway_address = Stm32NetX::NX->getIpInstance()->ipGatewayAddressGet();
            out()->printf("GATEWAY_ADDRESS: %lu.%lu.%lu.%lu\r\n",
                          (gateway_address >> 24) & 0xff,
                          (gateway_address >> 16) & 0xff,
                          (gateway_address >> 8) & 0xff,
                          (gateway_address >> 0) & 0xff
            );
            out()->println();
            OUTPUT_PAUSE;

            return ret;
        }
    };
}
#endif
