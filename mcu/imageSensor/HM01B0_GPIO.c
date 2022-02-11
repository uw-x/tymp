/*
 * HM01B0_GPIO.c
 *
 *  Created on: Nov 20, 2018
 *      Author: Ali
 */

#include "HM01B0_CAPTURE.h"
#include "HM01B0_GPIO.h"



/*******************************************************************************
 * New Code for GPIO using SPI
 ******************************************************************************/

/*!
* @brief GPIO settings initialization.
*/
void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    nrf_gpio_pin_set(CAM_SPI_CS);
}



void gpio_setting_init(void)
{
    ret_code_t err_code;

    /*gpiote initialization has been done in clock and timer initialization, no need to do it again*/
//    err_code = nrf_drv_gpiote_init();
//    APP_ERROR_CHECK(err_code);

//    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
//
//    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
//    APP_ERROR_CHECK(err_code);

    /*Finds rising edge instead of just toggling*/
    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_LOTOHI(true);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;

    err_code = nrf_drv_gpiote_in_init(FRAME_VLD, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);


    /*This function better to be written in the capture function*/
//    nrf_drv_gpiote_in_event_enable(PIN_IN, true);
}



///*******************************************************************************
// * Code GPIO
// ******************************************************************************/
//uint8_t gpio_read_cam(void)
//{
//	gpio_read = *(gpio_address);//We are reading GPIO 8,9,10,11 which are second byte
////	PRINTF("test: %x \n", gpio_read);
//	return gpio_read;
//}
//
///*!
//* @brief GPIO settings initialization.
//*/
//void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
//{
////    nrf_drv_gpiote_out_toggle(PIN_OUT);
//
////    printf("\f\r\nPINT Pin Interrupt %d event detected.", pintr);
//    if (hm_pixel_counter < hm_image_size){
//    	if (msb_flag == 0){
//    		image_mem[hm_pixel_counter] = image_mem[hm_pixel_counter] + (gpio_read_cam() >> 4);
//    		msb_flag = 1;
//    	} else{
//    		image_mem[hm_pixel_counter] = image_mem[hm_pixel_counter] + (gpio_read_cam() & mask_msb);
//    		msb_flag = 0;
//                hm_pixel_counter++;
//    	}
////        image_mem[hm_pixel_counter] = image_mem[hm_pixel_counter] + (gpio_read_cam() & mask_msb);
////        msb_flag = 0;
////        hm_pixel_counter++;
////    	}
//    } else{      
//        image_rd_done = 0x01;
////        nrf_drv_gpiote_in_event_enable(PIN_IN, true);
//
//        /*Stop camera streaming*/
//        #if (capture_mode == 0x01)
//            hm_i2c_write( REG_MODE_SELECT, 0x00);
//        #endif
//
//        #if (defined(CAMERA_DEBUG) && (CAMERA_DEBUG == 1))
//            uint8_t frame_cnt = hm_i2c_read(REG_FRAME_COUNT);
//            printf("Final number of frames: %d \n", frame_cnt);
//        #endif
//
//
//
//        /*MCU stops recording data*/
//        nrfx_gpiote_in_event_disable(PIN_IN);
//
////    	hm_i2c_write(REG_MODE_SELECT, 0x00);
////    	printf("\f\r\nWrote all data");
////    	printf("\f\r\nPINT Image value %x .", image_mem[2]);
////    	printf("\f\r\nPINT Image value %x .", image_mem[3]);
////    	printf("\f\r\nPINT Image value %x .", image_mem[4]);
//    }
//}
//
//
//
//void gpio_setting_init(void)
//{
//    ret_code_t err_code;
//
//    /*gpiote initialization has been done in clock and timer initialization, no need to do it again*/
////    err_code = nrf_drv_gpiote_init();
////    APP_ERROR_CHECK(err_code);
//
//    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
//
//    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
//    APP_ERROR_CHECK(err_code);
//
//    /*Finds rising edge instead of just toggling*/
//    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
////    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_LOTOHI(true);
//    in_config.pull = NRF_GPIO_PIN_PULLUP;
//
//    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);
//    APP_ERROR_CHECK(err_code);
//
//    nrf_gpio_cfg_sense_input(CAM_D0, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOSENSE);
//    nrf_gpio_cfg_sense_input(CAM_D1, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOSENSE);
//    nrf_gpio_cfg_sense_input(CAM_D2, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOSENSE);
//    nrf_gpio_cfg_sense_input(CAM_D3, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOSENSE);
//
//    /*This function better to be written in the capture function*/
////    nrf_drv_gpiote_in_event_enable(PIN_IN, true);
//}
//
