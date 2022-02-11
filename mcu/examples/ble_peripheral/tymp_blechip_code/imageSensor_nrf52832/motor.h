/*
 * motor.h
 *
 *  Created on: Nov 16, 2019
 *      Author: Vikram 
 */

#ifndef MOTOR_FUNC_H
#define MOTOR_FUNC_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "HM01B0_BLE_DEFINES.h"

/*#define LEFT 0
#define RIGHT 1
*/

extern bool step_forward_activated;

uint8_t bank_left, bank_right, bank_run;

uint8_t v_left, v_right, v_run; 

void motor_start(bool side);

void motor_stop(bool side);

void rt8092_init(void);

void step_forward(void);

void turn_right(void);

void turn_left(void);

void stop_robot(void);

void run_both_motors(void);

#endif