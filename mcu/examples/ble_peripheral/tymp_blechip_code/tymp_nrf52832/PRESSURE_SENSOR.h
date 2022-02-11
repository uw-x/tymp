/*
 * PRESSURE_SENSOR.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Ali Najafi
 */

#ifndef PRESSURE_SENSOR_H_
#define PRESSURE_SENSOR_H_


#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpio.h"

#include "MOTOR.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

/* Common addresses definition for temperature sensor. */
#define HWMPR_ADDR          (0x18U )
#define HWMPR_RSTn 28
#define EOC_PIN 29



/* Mode for HWMPR. */
#define NORMAL_MODE 0U

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

/* Buffer for samples read from temperature sensor. */
static uint8_t m_sample;

/*Pressure sensor data*/
extern uint8_t pressure_data[4];
uint8_t measured_press[4];


/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

/**
 * @brief TWI initialization.
 */
void twi_init (void);


/**
 * @brief Function for reading data from pressure sensor.
 */
void hwmpr_i2c_read(uint8_t * data_out, uint8_t rdsize);

void hwmpr_i2c_write(uint8_t cmd, uint16_t cmdData);
void pressure_sensor_reset(void);

void pressure_sensor_init(void);

void read_pressure_data(uint8_t * data_out);

void eoc_pin_int_init(void);

void read_pressure_data_interrupt(void);

void in_pin_handler_eoc_pin(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);








#endif /* PRESSURE_SENSOR_H_ */