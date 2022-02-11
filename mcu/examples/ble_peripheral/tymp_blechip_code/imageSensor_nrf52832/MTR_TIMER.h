 /** 
 *  MTR_TIMER.h
 *
 *  Created: Nov 18, 2019
 *      Author: Ali Najafi
 */

 #ifndef MTR_TIMER_H
#define MTR_TIMER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "HM01B0_BLE_DEFINES.h"
#include "motor.h"

/*#define LEFT 0
#define RIGHT 1
*/

#define MOTOR_TIMER_CONFIG                                                   \
{                                                                                    \
    .frequency          = NRF_TIMER_FREQ_31250Hz,\
    .mode               = (nrf_timer_mode_t)NRFX_TIMER_DEFAULT_CONFIG_MODE,          \
    .bit_width          = NRF_TIMER_BIT_WIDTH_32,\
    .interrupt_priority = 5,                    \
    .p_context          = NULL                                                       \
}
extern bool move_dir;

const nrf_drv_timer_t TIMER_MOTOR = NRF_DRV_TIMER_INSTANCE(3);
extern uint32_t motor_timer_val_left = MOTOR_TIMER_VALUE_LEFT;
extern uint32_t motor_timer_val_right = MOTOR_TIMER_VALUE_RIGHT;
extern uint8_t m_new_move;
bool cmd_motor_stream;


void timer_motor_event_handler(nrf_timer_event_t event_type, void* p_context);
void motor_timer_init(uint32_t motor_timer_val);


#endif