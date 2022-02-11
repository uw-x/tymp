/*
 * PRESSURE_SENSOR.c
 *
 *  Created on: Dec 30, 2019
 *      Author: Ali Najafi
 */

#include "PRESSURE_SENSOR.h"
#include "TYMP_DEFINES.h"

__STATIC_INLINE void data_handler(uint8_t temp)
{
    NRF_LOG_INFO("Temperature: %d Celsius degrees.", temp);
}

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                data_handler(m_sample);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}

/**
 * @brief TWI initialization.
 */
void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_HWMPR_config = {
       .scl                = 27,
       .sda                = 26,
       .frequency          = NRF_DRV_TWI_FREQ_250K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_HWMPR_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}




/**
 * @brief Function for reading data from pressure sensor.
 */
void hwmpr_i2c_read(uint8_t * data_out, uint8_t rdsize)
{
    ret_code_t err_code;
    m_xfer_done = false;

    err_code = nrf_drv_twi_rx(&m_twi, HWMPR_ADDR, data_out, rdsize);
    APP_ERROR_CHECK(err_code);

    while (m_xfer_done == false);
    m_xfer_done = false;

}

void hwmpr_i2c_write(uint8_t cmd, uint16_t cmdData)
{
    uint8_t reg[3];

    reg[0] = cmd;    // cmd
    reg[1] = (cmdData >> 8) & 0xFF;         
    reg[2] =  cmdData & 0xFF;

    ret_code_t err_code;

    /* Writing to SLAVE_REG_CONF "0" set temperature sensor in NORMAL mode. */
    err_code = nrf_drv_twi_tx(&m_twi, HWMPR_ADDR, reg, sizeof(reg), false);
    APP_ERROR_CHECK(err_code);

    while (m_xfer_done == false);

    m_xfer_done = false;
}
void pressure_sensor_reset(void){
    nrf_gpio_cfg_output(HWMPR_RSTn);
    nrf_gpio_pin_set(HWMPR_RSTn);
    nrf_delay_ms(100);
    nrf_gpio_pin_clear(HWMPR_RSTn);
    nrf_delay_ms(100);
    nrf_gpio_pin_set(HWMPR_RSTn);
    nrf_delay_ms(100);
}

void pressure_sensor_init(void){
    
    pressure_sensor_reset();
    nrf_delay_ms(500);
    twi_init();
    nrf_delay_ms(500);
    eoc_pin_int_init();

}

void read_pressure_data(uint8_t * data_out)
{
    hwmpr_i2c_write(0xAA, 0x0000);
    nrf_delay_ms(PRESSURE_READ_DELAY);
    hwmpr_i2c_read(data_out, 1);
//    nrf_delay_ms(PRESSURE_READ_DELAY);

    // check if this is a valid pressure value?
    if (data_out[0] == 0x40){
        hwmpr_i2c_read(data_out, 4);
//        nrf_delay_ms(1);
    } else {
        mtr_stop();
    }
}

void eoc_pin_int_init(void)
{
    ret_code_t err_code;

    /*Finds rising edge instead of just toggling*/
    nrf_drv_gpiote_in_config_t in_config_eoc_pin= NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);

    in_config_eoc_pin.pull = NRF_GPIO_PIN_NOPULL;

    err_code = nrf_drv_gpiote_in_init(EOC_PIN, &in_config_eoc_pin, in_pin_handler_eoc_pin);
    APP_ERROR_CHECK(err_code);
}

void read_pressure_data_interrupt(void){
    nrf_drv_gpiote_in_event_enable(EOC_PIN, true);
    hwmpr_i2c_write(0xAA, 0x0000);
}

void in_pin_handler_eoc_pin(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{//we need to read the frame_valid pin. If it is high do the camera and if it is low do the motor timer
    nrf_drv_gpiote_in_event_disable(EOC_PIN);
    hwmpr_i2c_read(measured_press, 1);
    if (measured_press[0] == 0x40){
        hwmpr_i2c_read(measured_press, 4);
    } else {
        mtr_stop();
    }
}





/** @} */
