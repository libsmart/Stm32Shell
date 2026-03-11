/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <main.h>
#include "Command/AbstractCommand.hpp"
#include "MCU/ControllerId.hpp"

extern "C" {
#include "lan8742.h"
#include "nx_stm32_phy_driver.h"
}

#define OUTPUT_PAUSE do{out()->flush();delay(20);}while(false);

namespace Stm32Shell::ezShell::Command {
    class Info : public Stm32Shell::Command::AbstractCommand {
    public:
        Info() {
            Nameable::setName("info");
            isSync = true;
            setLogger(&Logger);
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

            out()->print(F("CONTROLLER_ID: "));
            out()->println(Stm32Common::ControllerId::getControllerId());
            OUTPUT_PAUSE;

#ifdef LIBSMART_STM32NETX
            out()->printf("HARDWARE_MAC: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                          static_cast<unsigned int>(heth.Init.MACAddr[0]),
                          static_cast<unsigned int>(heth.Init.MACAddr[1]),
                          static_cast<unsigned int>(heth.Init.MACAddr[2]),
                          static_cast<unsigned int>(heth.Init.MACAddr[3]),
                          static_cast<unsigned int>(heth.Init.MACAddr[4]),
                          static_cast<unsigned int>(heth.Init.MACAddr[5]));
            OUTPUT_PAUSE;

            const auto linkState = nx_eth_phy_get_link_state();
            auto linkStateStr = [](const int32_t status)-> const char * {
                switch (status) {
                    case LAN8742_STATUS_READ_ERROR:
                        return "LAN8742_STATUS_READ_ERROR";
                    case LAN8742_STATUS_LINK_DOWN:
                        return "LAN8742_STATUS_LINK_DOWN";
                    case LAN8742_STATUS_100MBITS_FULLDUPLEX:
                        return "LAN8742_STATUS_100MBITS_FULLDUPLEX";
                    case LAN8742_STATUS_100MBITS_HALFDUPLEX:
                        return "LAN8742_STATUS_100MBITS_HALFDUPLEX";
                    case LAN8742_STATUS_10MBITS_FULLDUPLEX:
                        return "LAN8742_STATUS_10MBITS_FULLDUPLEX";
                    case LAN8742_STATUS_10MBITS_HALFDUPLEX:
                        return "LAN8742_STATUS_10MBITS_HALFDUPLEX";
                    case LAN8742_STATUS_AUTONEGO_NOTDONE:
                        return "LAN8742_STATUS_AUTONEGO_NOTDONE";
                    default:
                        return "Unknown";
                }
            };
            out()->printf("LINK_STATE: %s (%d)\r\n", linkStateStr(linkState), linkState);

            if (Stm32NetX::NX == nullptr) return ret;
            ULONG ip_address, network_mask;
            const auto ipInst = Stm32NetX::NX->getIpInstance();
            if (ipInst != nullptr) {
                ipInst->ipAddressGet(&ip_address, &network_mask);
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
            }
#endif

#ifdef LIBSMART_STM32RTC
            try {
                Stm32Rtc::DateTimeType dateTime;
                rtc.getDateTime(dateTime);
                out()->printf("RTC_DATE_TIME: %02d.%02d.%04d %02d:%02d:%02d\r\n",
                              dateTime.Date, dateTime.Month, dateTime.Year + 2000,
                              dateTime.Hours, dateTime.Minutes, dateTime.Seconds);
            } catch (const std::exception &e) {
                Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                        ->printf("ERROR: %s\r\n", e.what());
                out()->println("RTC_DATE_TIME: ERROR!");
            }
#endif

            return ret;
        }
    };
}
