/*
 * HM01B0_SPI.h
 *
 *  Created on: Nov 20, 2018
 *      Author: Ali
 */

#ifndef HM01B0_SPI_H_
#define HM01B0_SPI_H_

/*  Standard C Included Files */
#include <stdbool.h>
//#include <stdio.h>
//#include <string.h>


#include "sdk_config.h"
#include "nrf_drv_spis.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define spi_buffer_size 52160
//#define spi_buffer_size 100

#define SPIS_INSTANCE 2 /**< SPIS instance index. */
static const nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE);/**< SPIS instance. */
//static nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE);/**< SPIS instance. */

//#define TEST_STRING "Nordic"
//#define TEST_STRING "N"
//static uint8_t       m_tx_buf[] = TEST_STRING;           /**< TX buffer. */
static uint8_t       m_tx_buf[1] = {0} ;                           /**< TX buffer. */
//static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
//static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */
extern uint8_t       m_rx_buf[spi_buffer_size+1];       /**< RX buffer. */
extern uint16_t m_length_rx = spi_buffer_size;        /**< Transfer length. */
extern uint16_t m_length_rx_done = 0;        /**< Transfer length. */
//static const uint8_t m_length_tx = 0;        /**< Transfer length. */
static uint8_t m_length_tx = 0;        /**< Transfer length. */

extern bool spis_xfer_done; /**< Flag used to indicate that SPIS instance completed the transfer. */

#ifdef BSP_LED_0
    #define PIN_OUT BSP_LED_0
#endif

/*******************************************************************************
 * SPI Functions
 ******************************************************************************/
/*!
* @brief SPI settings initialization.
*/

void spis_event_handler(nrf_drv_spis_event_t event);

void spi_init(void);

#endif /* HM01B0_SPI_H_ */
