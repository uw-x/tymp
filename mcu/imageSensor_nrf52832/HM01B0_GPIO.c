/*
 * HM01B0_GPIO.c
 *
 *  Created on: Nov 20, 2018
 *      Author: Ali
 */


#include "HM01B0_GPIO.h"

/*******************************************************************************
 * New Code for GPIO using SPI
 ******************************************************************************/

/*!
* @brief GPIO settings initialization.
*/


//void in_pin_handler_line_vld(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
//{
////    nrf_drv_gpiote_in_event_disable(LINE_VLD);
//    if (line_count<LINE_NUM){//Nice:this number (324) is the maximum number of lines that 
////        while (*(m_rx_buf +line_count*spi_buffer_size-1) == 0x11);
//        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//        nrfx_spis_buffers_set_back(&spis, m_tx_buf, m_length_tx, m_rx_buf + line_count*spi_buffer_size, m_length_rx);
//    //    pin_read = nrf_gpio_pin_read(CAM_LINE_VLD);
//        gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//
//      
//    //    rxd_ptr = rxd_ptr + spi_buffer_size;
//    //    nrfx_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx);
//    //    APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length_tx, m_rx_buf + spi_buffer_size, m_length_rx));
//        line_count++;
//        
//    //    nrf_gpio_pin_set(CAM_SPI_CS);
//    //    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
//        nrf_drv_gpiote_in_event_enable(LINE_VLD, true);
//        } 
//    else{
//        nrf_drv_gpiote_in_event_disable(LINE_VLD);
//        image_rd_done = 1;
//        image_frame_done = 1;
//        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//        #if (FINAL_CODE ==0)
//        printf("Received all \n");
//        #endif
//    }
//}

void in_pin_handler_line_vld(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
//    nrf_drv_gpiote_in_event_disable(LINE_VLD);
    if (line_count<LINE_NUM){
    // here we need to activate SPI CS; enable the lvld_timer; and activate the line_vld interrupt; increase the counter of lines

        gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
        line_count++;
//        nrf_drv_gpiote_in_event_enable(LINE_VLD, true);
        nrf_drv_timer_enable(&TIMER_LVLD);
        } 
    else{
        nrf_drv_gpiote_in_event_disable(LINE_VLD);
        nrf_drv_timer_disable(&TIMER_LVLD);
        image_rd_done = 1;
        image_frame_done = 1;
        gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
        #if (FINAL_CODE ==0)
        printf("Received all \n");
        #endif
    }
}

void in_pin_handler_frame_vld(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    image_frame_done = 1;
    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
//    spis_xfer_done = true;
}




void gpio_setting_init(void)
{


    gpio_p_reg->OUTSET = CAM_SPI_PIN_MASK;
    gpio_p_reg->OUTCLR = CAM_SPI_PIN_MASK;
    ret_code_t err_code;

    /*gpiote initialization has been done in clock and timer initialization, no need to do it again*/
//    err_code = nrf_drv_gpiote_init();
//    APP_ERROR_CHECK(err_code);

//    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
//
//    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
//    APP_ERROR_CHECK(err_code);

    /*Finds rising edge instead of just toggling*/
    nrf_drv_gpiote_in_config_t in_config_frmvld = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_LOTOHI(true);
    in_config_frmvld.pull = NRF_GPIO_PIN_PULLDOWN;

    err_code = nrf_drv_gpiote_in_init(FRAME_VLD, &in_config_frmvld, in_pin_handler_frame_vld);
    APP_ERROR_CHECK(err_code);


    /*Finds falling for line valid edge instead of just toggling*/
    nrf_drv_gpiote_in_config_t in_config_lnvld = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    nrf_drv_gpiote_in_config_t in_config = NRFX_GPIOTE_RAW_CONFIG_IN_SENSE_LOTOHI(true);
    in_config_lnvld.pull = NRF_GPIO_PIN_PULLDOWN;

    err_code = nrf_drv_gpiote_in_init(LINE_VLD, &in_config_lnvld, in_pin_handler_line_vld);
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
