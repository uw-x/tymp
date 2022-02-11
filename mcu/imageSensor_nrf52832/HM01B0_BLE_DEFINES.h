/*
 * HM01B0_BLE_DEFINES.h
 *
 *  Created on: Apr 05, 2019
 *      Author: Ali
 */


//Got from HM01B0_SPI.h +++++++++++++++++++++++++++++++++++++++++
#ifndef HM01B0_BLE_DEFINES_H_
#define HM01B0_BLE_DEFINES_H_

//#define spi_buffer_size 33000
#define LINE_NUM 50
#define spi_buffer_size 220
#define total_spi_buffer_size spi_buffer_size*LINE_NUM 

#define SPIS_INSTANCE 2 /**< SPIS instance index. */


#ifdef BSP_LED_0
    #define PIN_OUT BSP_LED_0
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Got from HM01B0_CAPTURE.h +++++++++++++++++++++++++++++++++++++++++
/*if capture_mode=0x01 => it streams. This is useful for tetsing purposes*/
/*if capture_mode=0x03 => it takes frame_count shots. */
#define capture_mode  0x03
#define CAMERA_DEBUG 0
#define BLE_DEBUG 0
#define JPEG_COMPRESS 1
#define FINAL_CODE 0
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Got from HM01B0_PWM.h +++++++++++++++++++++++++++++++++++++++++
#define PIEZO_PIN  NRF_GPIO_PIN_MAP(0,24)
#define BOOST_PIN  NRF_GPIO_PIN_MAP(0,25)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//Got from HM01B0_CLK.h +++++++++++++++++++++++++++++++++++++++++
#if defined(BOARD_PCA10056)
#define HM_CLK 15  /**< Pin number for output. */
#endif 

#if defined(BOARD_PCA10040)
#define HM_CLK 12  /**< Pin number for output. */
#endif 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Got from HM01B0_COMPRESS.h +++++++++++++++++++++++++++++++++++++++++
#define compressed_file "test_lowest4.jpg"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Got from HM01B0_LVLD_TIMER.h +++++++++++++++++++++++++++++++++++++++++
#define LVLD_TIMER_VALUE (spi_buffer_size + 20)*8*2 //the second number in multiplication is equal to 64/cam_mcl_freq; if cam_mcl_freq=8MHz => 8
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Timers used +++++++++++++++++++++++++++++++++++++++++
/*Timer 0 => BLE timing
Timer 1 => camera clk
Timer 3 => pwm 3
Timer 4 => pwm 4
Timer 2 => LVLD timer!!*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//IRQ priorities +++++++++++++++++++++++++++++++++++++++++
/*Timer 0 => BLE timing
GPIOTE:5
LVLD Timer:3
SPIS_ENABLED: 4
NRFX_SPIS:4
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif /* HM01B0_BLE_DEFINES_H_ */