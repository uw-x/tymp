/*
 * HM01B0_CAPTURE.c
 *
 *  Created on: Nov 25, 2018
 *      Author: Ali
 */

#include "HM01B0_CAPTURE.h"
#include "HM01B0_GPIO.h"
#include "HM01B0_SPI.h"

uint8_t last_memory_value;

void hm_peripheral_init(void){


    /*Enable the camera clock*/
    hm_clk_out();

    /*Initialize the GPIO settings: 4pins for camera data, 1pin for camera clock*/
//    gpio_setting_init();


    /*Initialize the I2C for camera*/
    twi_init();
    nrf_delay_ms(100);

    /*Test if camera is functional*/
    uint8_t version = hm_i2c_read(REG_MODEL_ID_L);
    if(version != 0xB0){
        printf("REG_MODEL_ID_L: %x \n", version);
        printf("Camera version problem \n");
    }
    hm_i2c_write( REG_MODE_SELECT, 0x00);//go to stand by mode

    /*Initialize and set high the SPI chip select*/
    nrf_gpio_cfg_output(CAM_D0);//Set up the chip select for SPI
    nrf_gpio_pin_set(CAM_D0);

}


void hm_single_capture(void){

    /*Camera values initialized*/
    hm_pixel_counter = 0;
    msb_flag = 0;
    image_rd_done = 0;
    ble_bytes_sent_counter = 0;
    
    /*Camera settings initialization*/
    hm01b0_init();

    /*Enable the PCLK interrupt function*/
//    nrf_drv_gpiote_in_event_enable(PIN_IN, true);

    /*Count the number of frames sent out*/
    #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1))
        uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
        printf("Initial number of frames: %d \n", frame_cnt);
    #endif



    /*Capture and stream one frame out*/
    hm_i2c_write( REG_MODE_SELECT, capture_mode);

}


void hm_single_capture_spi(void){
    
    m_length_rx_done = 0;
    ble_bytes_sent_counter = 0;

    nrf_gpio_pin_clear(CAM_D0);


    /*Camera values initialized*/
    hm_pixel_counter = 0;
    msb_flag = 0;
    image_rd_done = 0;
    ble_bytes_sent_counter = 0;
    
    /*Camera settings initialization*/
    hm01b0_init();

    /*Enable the PCLK interrupt function*/
//    nrf_drv_gpiote_in_event_enable(PIN_IN, true);

    /*Count the number of frames sent out*/
    #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1))
        uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
        printf("Initial number of frames: %d \n", frame_cnt);
    #endif



    /*Capture and stream one frame out*/
    hm_i2c_write( REG_MODE_SELECT, capture_mode);

//    while (!spis_xfer_done);
//    nrf_gpio_pin_set(CAM_D0);

    while (*(m_rx_buf+spi_buffer_size-325)!=0);
//    while (*(0x2000F9B7)!=0);
    last_memory_value=*(m_rx_buf+spi_buffer_size);
//    printf('last memory value %x', last_memory_value);
    nrf_gpio_pin_set(CAM_D0);
    while (!spis_xfer_done);
    m_length_rx_done = m_length_rx;
//    nrf_gpio_pin_set(CAM_D0);
    
}
