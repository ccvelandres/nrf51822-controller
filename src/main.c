/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_assert.h"
#include "nrf_uart.h"
#include "nrf_drv_uart.h"

static nrf_drv_uart_t m_instance_uart = NRF_DRV_UART_INSTANCE(0);


static void init(void)
{
    NRF_RNG->TASKS_START = 1;

    // Start 16 MHz crystal oscillator
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    // Wait for the external oscillator to start up
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
}



static void init_uart()
{
    nrf_drv_uart_config_t cfg = NRF_DRV_UART_DEFAULT_CONFIG;
    cfg.baudrate = NRF_UART_BAUDRATE_115200;
    cfg.pselrxd = UART0_RX_PIN_NUMBER;
    cfg.pseltxd = UART0_TX_PIN_NUMBER;
    cfg.pselcts = UART0_CTS_PIN_NUMBER;
    cfg.pselrts = UART0_RTS_PIN_NUMBER;
    cfg.hwfc = UART0_HWFC;
    cfg.parity = NRF_UART_PARITY_EXCLUDED;
    nrf_drv_uart_uninit(&m_instance_uart);
    nrf_drv_uart_init(&m_instance_uart, &cfg, NULL);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init();
    // init_uart();

    // const char msg1[] = "STARTUP";
    // nrf_drv_uart_tx(&m_instance_uart, (const uint8_t*) msg1, sizeof(msg1));

    NRF_LOG_INIT(NULL);
    NRF_LOG_INFO("STARTUP\r\n");

    nrf_gpio_cfg_output(LED_RGB_RED);
    nrf_gpio_cfg_output(LED_RGB_BLUE);

    /* Toggle LEDs. */
    while (true)
    {
        for (int i = 0; i < LEDS_NUMBER; i++)
        {
            // const char msg2[] = "TOGGLING";
            // nrf_drv_uart_tx(&m_instance_uart, (const uint8_t*) msg2, sizeof(msg2));
            NRF_LOG_INFO("TOGGLING..\r\n");
            nrf_gpio_pin_toggle(LED_RGB_RED);
            nrf_gpio_pin_toggle(LED_RGB_BLUE);
            nrf_delay_ms(500);
        }
    }
}

/**
 *@}
 **/
