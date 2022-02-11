/**
 * Copyright (c) 2014 - 2018, Nordic Semiconductor ASA
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
 * @defgroup nrf_dev_timer_example_main main.c
 * @{
 * @ingroup nrf_dev_timer_example
 * @brief Timer Example Application main file.
 *
 * This file contains the source code for a sample application using Timer0.
 *
 */

#include "HM01B0_LVLD_TIMER.h"



/**
 * @brief Handler for timer events.
 */
void timer_lvld_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    if (line_count<LINE_NUM){
    // here we need to activate SPI CS; enable the lvld_timer; and activate the line_vld interrupt; increase the counter of lines
        nrf_drv_timer_disable(&TIMER_LVLD);
        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
        nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + line_count*spi_buffer_size, m_length_rx);
        nrf_drv_gpiote_in_event_enable(LINE_VLD, true);
        } 
    else{
        nrf_drv_gpiote_in_event_disable(LINE_VLD);
        nrf_drv_timer_disable(&TIMER_LVLD);
        image_rd_done = 1;
        image_frame_done = 1;
        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
        #if (FINAL_CODE ==0)
        printf("Received all \n");
        #endif
    }

}

void lvld_timer_init(void)
{
    uint32_t err_code = NRF_SUCCESS;
    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_LVLD, &timer_cfg, timer_lvld_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_extended_compare(
         &TIMER_LVLD, NRF_TIMER_CC_CHANNEL4, lvld_timer_val, NRF_TIMER_SHORT_COMPARE4_CLEAR_MASK, true);

    //nrf_drv_timer_enable(&TIMER_LVLD);
}

/** @} */
