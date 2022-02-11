/*
 * HM01B0_SPI.c
 *
 *  Created on: Nov 30, 2018
 *      Author: Ali
 */

#include "HM01B0_CAPTURE.h"
#include "HM01B0_SPI.h"





/*******************************************************************************
 * Code SPI
 ******************************************************************************/

/**
 * @brief SPIS user event handler.
 *
 * @param event
 */
void spis_event_handler(nrf_drv_spis_event_t event)
{
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
    {
        spis_xfer_done = true;
        NRF_LOG_INFO(" Transfer completed. Received: %s",(uint32_t)m_rx_buf);
    }
}


void spi_init(void)
{
    // Enable the constant latency sub power mode to minimize the time it takes
    // for the SPIS peripheral to become active after the CSN line is asserted
    // (when the CPU is in sleep mode).
//    NRF_POWER->TASKS_CONSTLAT = 1;

    bsp_board_init(BSP_INIT_LEDS);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("SPIS example");

    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin               = APP_SPIS_CS_PIN;//pin 29
    spis_config.miso_pin              = APP_SPIS_MISO_PIN;//pin 28
    spis_config.mosi_pin              = APP_SPIS_MOSI_PIN;//pin 4 
    spis_config.sck_pin               = APP_SPIS_SCK_PIN;//pin 3

    APP_ERROR_CHECK(nrf_drv_spis_init(&spis, &spis_config, spis_event_handler));

//    printf('%d',sizeof(m_tx_buf));
    memset(m_rx_buf, 255, m_length_rx);
    spis_xfer_done = false;

    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf, m_length_rx));
}
