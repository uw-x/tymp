/*
 * HM01B0_CAPTURE.c
 *
 *  Created on: Nov 25, 2018
 *      Author: Ali
 */

#include "HM01B0_CAPTURE.h"

uint8_t last_memory_value;

#if defined(BOARD_PCA10040)

#endif




void hm_peripheral_init(void){


    /*Enable the camera clock*/
//    uint32_t* hfclkstart_address = 0x40000304;//address for timer1 prescaler
//    uint32_t hfclkstart_address_val = 0x00000001;
//    (*hfclkstart_address) = hfclkstart_address_val;
    hm_clk_out();

    /*Initialize the GPIO settings: 4pins for camera data, 1pin for camera clock*/
//    gpio_setting_init();

    /*Initialize the GPIO settings: Frame valid for */
    gpio_setting_init();

    /*reset i2c pins */
//    nrf_gpio_cfg_output(ARDUINO_SCL_PIN);
//    nrf_gpio_pin_set(ARDUINO_SCL_PIN);
//    nrf_gpio_pin_clear(ARDUINO_SCL_PIN);
//    
//    nrf_gpio_cfg_output(ARDUINO_SDA_PIN);
//    nrf_gpio_pin_set(ARDUINO_SDA_PIN);
//    nrf_gpio_pin_clear(ARDUINO_SCL_PIN);


    /*Initialize the I2C for camera*/
    twi_init();
    nrf_delay_ms(100);

    /*Test if camera is functional*/
//    hm_i2c_write(0x2108,0x33);
//    hm_i2c_write(0x210A,0x00);
    hm_i2c_write(REG_MODE_SELECT,0x00);
    uint8_t version = hm_i2c_read(REG_MODEL_ID_L);
    if(version != 0xB0){
        printf("REG_MODEL_ID_L: %x \n", version);
        printf("Camera version problem \n");
    }
    hm_i2c_write( REG_MODE_SELECT, 0x00);//go to stand by mode

    /*Initialize and set high the SPI chip select*/
    nrf_gpio_cfg_output(CAM_SPI_CS);//Set up the chip select for SPI
    nrf_gpio_pin_set(CAM_SPI_CS);


    /*Camera settings initialization*/
    hm01b0_init_fixed_rom_qvga_fixed();
//    hm01b0_init_brighter();

    lvld_timer_init();
    

}


void hm_single_capture(void){

    /*Camera values initialized*/
//    hm_pixel_counter = 0;
//    msb_flag = 0;
//    image_rd_done = 0;
    ble_bytes_sent_counter = 0;
    
    /*Camera settings initialization*/
    hm01b0_init_fixed_rom_qvga_fixed();

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

    /*SPI registers initilization*/
    spis_xfer_done = false;
//    memset(m_rx_buf, 0, m_length_rx);
    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf, m_length_rx));

    nrf_gpio_pin_clear(CAM_SPI_CS);


    /*Camera values initialized*/
//    hm_pixel_counter = 0;
//    msb_flag = 0;
//    image_rd_done = 0;
    ble_bytes_sent_counter = 0;
   

    /*Enable the FRAME VALID interrupt*/
    nrf_drv_gpiote_in_event_enable(FRAME_VLD, true);

    /*Count the number of frames sent out*/
    #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1) && (FINAL_CODE == 0))
        uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
        printf("Initial number of frames: %d \n", frame_cnt);
    #endif


    /*Capture and stream one frame out*/
    hm_i2c_write( REG_MODE_SELECT, capture_mode);

//    while (!spis_xfer_done);
//    nrf_gpio_pin_set(CAM_D0);

//    while (*(m_rx_buf+spi_buffer_size-325)!=0);
//    while (*(0x2000F9B7)!=0);
//    last_memory_value=*(m_rx_buf+spi_buffer_size);
//    printf('last memory value %x', last_memory_value);
//    nrf_gpio_pin_set(CAM_SPI_CS);
    while (!spis_xfer_done);
//    spis_xfer_done = false;
    m_length_rx_done = m_length_rx;
//    nrf_gpio_pin_set(CAM_D0);
    
}


void hm_single_capture_spi_832(void){

    (*timer0_prescaler_address) = timer0_prescaler0;
    //nrf_delay_ms(100);
    m_length_rx_done = 0;
    ble_bytes_sent_counter = 0;
    line_count = 0;

    /*Defining the followings to decrease GPIO number of clock cycles*/
//    NRF_GPIO_Type * gpio_p_reg = nrf_gpio_pin_port_decode(&CAM_SPI_CS);
//    uint32_t CAM_SPI_PIN_MASK = 1UL << (CAM_SPI_CS);

    /*SPI registers initilization*/
    spis_xfer_done = false;
    memset(m_rx_buf, 0x11, total_spi_buffer_size);
    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf, m_length_rx));


    /*Camera values initialized*/
//    hm_pixel_counter = 0;
//    msb_flag = 0;
    image_rd_done = 0;
    image_frame_done = 0;
    ble_bytes_sent_counter = 0;
   

    /*Enable the FRAME VALID interrupt*/
    
//    nrf_drv_gpiote_in_event_enable(FRAME_VLD, true);
//    nrf_drv_gpiote_in_event_enable(LINE_VLD, true);
   

    /*Count the number of frames sent out*/
    uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
    #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1) && (FINAL_CODE == 0))
        
        printf("Initial number of frames: %d \n", frame_cnt);
    #endif
    //printf("Initial number of frames: %d \n", frame_cnt);

    nrf_drv_timer_enable(&TIMER_LVLD);
    nrf_gpio_pin_clear(CAM_SPI_CS);

    hm_i2c_write( REG_MODE_SELECT, capture_mode);
  

    while (image_rd_done != 1);
  //  while (image_frame_done != 1);
    while (!spis_xfer_done);
//    (*timer0_prescaler_address) = timer0_prescaler0;
    spis_xfer_done = false;

    #if defined(BOARD_PCA10056)
    m_length_rx_done = m_length_rx;
    #endif

    #if defined(BOARD_PCA10040)
    m_length_rx_done = total_spi_buffer_size;
    #endif
    
//    nrf_gpio_pin_set(CAM_D0);
    
}


void hm_single_capture_spi_832_compressed(void){
    
    m_length_rx_done = 0;
    ble_bytes_sent_counter = 0;
    line_count = 0;

    /*Defining the followings to decrease GPIO number of clock cycles*/
//    NRF_GPIO_Type * gpio_p_reg = nrf_gpio_pin_port_decode(&CAM_SPI_CS);
//    uint32_t CAM_SPI_PIN_MASK = 1UL << (CAM_SPI_CS);

    /*SPI registers initilization*/
    spis_xfer_done = false;
    memset(m_rx_buf, 0x11, total_spi_buffer_size);
    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf, m_length_rx));


    /*Camera values initialized*/
//    hm_pixel_counter = 0;
//    msb_flag = 0;
    image_rd_done = 0;
    image_frame_done = 0;
    ble_bytes_sent_counter = 0;
   

    /*Enable the FRAME VALID interrupt*/
    //nrf_drv_gpiote_in_event_enable(FRAME_VLD, true);
    nrf_drv_gpiote_in_event_enable(LINE_VLD, true);
   

    /*Count the number of frames sent out*/
    #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1) && (FINAL_CODE == 0))
        uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
        printf("Initial number of frames: %d \n", frame_cnt);
    #endif


    nrf_gpio_pin_clear(CAM_SPI_CS);

    hm_i2c_write( REG_MODE_SELECT, capture_mode);
  

    while (image_rd_done != 1);
    while (image_frame_done != 1);
    while (!spis_xfer_done);
    spis_xfer_done = false;

    #if defined(BOARD_PCA10056)
    m_length_rx_done = m_length_rx;
    #endif

    #if defined(BOARD_PCA10040)
    m_length_rx_done = total_spi_buffer_size;
    #endif
//    uint8_t xtest;
    compressed_size = main_compression(quality);
    
}

//    uint32_t* spis_rxd_address_begin = 0x40023534;//reg address for the RXD start pointer
//    uint32_t* spis_semstat = 0x40023400;//reg address for the RXD start pointer
//    uint8_t i = 0;
    /*Capture and stream one frame out*/
//    uint32_t i;
//    uint32_t pin_read = 0x00000000;
//    uint32_t pin_read2 = 0x00000000;
//    pin_read = nrf_gpio_pin_read(CAM_LINE_VLD); 
//    pin_read2 = nrf_gpio_pin_read(CAM_LINE_VLD); 





//    for (i = 1; i<384; i++){
//     while(1){
//        while(!(pin_read == 1 && pin_read2 == 0)){
//            pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//            pin_read2 = nrf_gpio_pin_read(CAM_LINE_VLD);
//        }
//        while (*(m_rx_buf +i*spi_buffer_size-1) == 0x11);
//        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//        nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + i*spi_buffer_size, m_length_rx);
//        pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//        gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        }




//    for (i = 0; i<100; i++){
//        while (*(m_rx_buf + (i+1)*spi_buffer_size-1) == 0x00);
//        (*spis_rxd_address_begin) = m_rx_buf + (i+1)*spi_buffer_size;
//        }
    
//    while(pin_read){
//        pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    }
//    while (*(m_rx_buf +spi_buffer_size-1) == 0x11);
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
//
//      nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx);
//    pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//    while(pin_read){
//        pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    }
//    while (*(m_rx_buf +2*spi_buffer_size-1) == 0x11);
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
//
//      nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + 2*spi_buffer_size, m_length_rx);
//    pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//    while(pin_read){
//        pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    }
//    while (*(m_rx_buf +3*spi_buffer_size-1) == 0x11);
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
//      nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + 3*spi_buffer_size, m_length_rx);
//    pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//
//    while(pin_read){
//        pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//    }
//    while (*(m_rx_buf +4*spi_buffer_size-1) == 0x11);
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;


//    printf("Received the first 240 bytes: \n");
    

//    while (!spis_xfer_done);
//    nrf_gpio_pin_set(CAM_D0);

//    while (*(m_rx_buf+spi_buffer_size-325)!=0);
//    while (*(0x2000F9B7)!=0);
//    last_memory_value=*(m_rx_buf+spi_buffer_size);
//    printf('last memory value %x', last_memory_value);
//    nrf_gpio_pin_set(CAM_SPI_CS);













//    for (i = 0; i<100; i++){
//        while (*(m_rx_buf + (i+1)*spi_buffer_size-1) == 0x00);
//        (*spis_rxd_address_begin) = m_rx_buf + (i+1)*spi_buffer_size;
//        }
//    while (*(m_rx_buf +spi_buffer_size-1) == 0x11);
//    //nrf_gpio_pin_set(CAM_SPI_CS);
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
////    (*spis_rxd_address_begin) = m_rx_buf + spi_buffer_size;
////    (*spis_semstat) = 0x00000000;
////    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx));
//      nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx);
////    nrf_drv_spis_buffers_set_832(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx);
////    nrf_gpio_pin_clear(CAM_SPI_CS);
//    
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        
//    while (*(m_rx_buf +2*spi_buffer_size-1) == 0x11);
//
////    (*spis_rxd_address_begin) = m_rx_buf + 2*spi_buffer_size;
////    printf("Received the first 240 bytes: \n");
//
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
////    (*spis_rxd_address_begin) = m_rx_buf + spi_buffer_size;
////    (*spis_semstat) = 0x00000000;
//    nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + 2*spi_buffer_size, m_length_rx);
////    nrf_gpio_pin_clear(CAM_SPI_CS);
//    
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        
//    while (*(m_rx_buf +3*spi_buffer_size-1) == 0x11);
//
//
//        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
////    (*spis_rxd_address_begin) = m_rx_buf + spi_buffer_size;
////    (*spis_semstat) = 0x00000000;
//    nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + 3*spi_buffer_size, m_length_rx);
////    nrf_gpio_pin_clear(CAM_SPI_CS);
//    
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        
//    while (*(m_rx_buf +4*spi_buffer_size-1) == 0x11);
//
//    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//
////    (*spis_rxd_address_begin) = m_rx_buf + spi_buffer_size;
////    (*spis_semstat) = 0x00000000;
//    nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + 4*spi_buffer_size, m_length_rx);
////    nrf_gpio_pin_clear(CAM_SPI_CS);
//    
//    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        
//    while (*(m_rx_buf +5*spi_buffer_size-1) == 0x11);
