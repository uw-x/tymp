/*
 * HM01B0_CLK.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Ali
 */

#ifndef HM01B0_CLK_H_
#define HM01B0_CLK_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"

#include "boards.h"
#include "app_error.h"

#define HM_CLK 15  /**< Pin number for output. */

static nrf_drv_timer_t timer = NRF_DRV_TIMER_INSTANCE(1);

static void timer_dummy_handler(nrf_timer_event_t event_type, void * p_context){}

void set_timer0_prescaler(uint32_t timer0_prescaler);

void hm_clk_out(void);

#endif /* HM01B0_CLK_H_ */