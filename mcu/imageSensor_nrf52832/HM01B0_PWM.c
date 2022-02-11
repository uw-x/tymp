/**
 * Copyright (c) 2015 - 2018, Nordic Semiconductor ASA
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
 * @defgroup pwm_example_main main.c
 * @{
 * @ingroup pwm_example
 *
 * @brief  PWM Example Application main file.
 *
 * This file contains the source code for a sample application using PWM.
 *
 *
 */


#include "HM01B0_PWM.h"


void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}
// duty cycle, voltage (V), current (mA)
// 6/100k, 140, 25
// 7/100k, 160, 40
// 8/100k, 180, 55

void pwm_code(uint32_t piezo_period, uint32_t piezo_duty, uint32_t boost_period, uint32_t boost_duty)
{
    ret_code_t err_code;

    /* 2-channel PWM, 200Hz, output on DK LED pins. */ 
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(piezo_period, PIEZO_PIN);

    /* Switch the polarity of the second channel. */
    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

    //app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_2CH(50L, SER_CON_SPIS_MOSI_PIN, BSP_LED_1);
    app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_1CH(boost_period, BOOST_PIN);
    pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize and enable PWM. */
    err_code = app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM1);

    err_code = app_pwm_init(&PWM2,&pwm2_cfg,pwm_ready_callback);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM2);

    while (app_pwm_channel_duty_set(&PWM1, 0, piezo_duty) == NRF_ERROR_BUSY);
    while (app_pwm_channel_duty_set(&PWM2, 0, boost_duty) == NRF_ERROR_BUSY);

}


/** @} */
