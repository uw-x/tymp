/*
 * HM01B0_I2c.c
 *
 *  Created on: Sept 15, 2018
 *      Author: Ali Najafi
 */

#include "HM01B0_FUNC.h"

void hm01b0_init(void){

//    hm_i2c_write(REG_SW_RESET, 0x00); //Software reset, reset all serial interface registers to its default values
    hm_i2c_write( REG_MODE_SELECT, 0x00);//go to stand by mode
    hm_i2c_write( REG_ANA_REGISTER_17, 0x00);//register to change the clk source(osc:1 mclk:0), if no mclk it goes to osc by default
    hm_i2c_write( REG_TEST_PATTERN_MODE, 0x01);//Enable the test pattern, set it to walking 1

//    i2c_write(REG_FRAME_LENGTH_LINES_H, 0x00);//Set frame length lines MSB to QQVGA :
//    i2c_write(REG_FRAME_LENGTH_LINES_L, 0xD7);//Set frame length lines LSB to QQVGA : 0xD7 = 215
//
//    i2c_write(REG_FRAME_LENGTH_PCK_H, 0x00);//Set line length MSB to QQVGA
//    i2c_write(REG_FRAME_LENGTH_PCK_L, 0x80);//Set line length LSB to QQVGA : 0x80 = 128

    hm_i2c_write( REG_QVGA_WIN_EN, 0x01);//Set line length LSB to QQVGA => enabled: makes the image 160(row)*240(col)
//    disable: image 160*320 //In test pattern mode, enabling this does not have any effect

    hm_i2c_write( REG_OSC_CLK_DIV, 0x20);//This is effective when we use external clk, Use the camera in the gated clock mode to make the clock zero when there is no data

    hm_i2c_write( REG_BIN_RDOUT_X, 0x03);//Horizontal Binning enable
    hm_i2c_write( REG_BIN_RDOUT_Y, 0x03);//vertical Binning enable
    hm_i2c_write( REG_BIN_MODE, 0x03);//VERTICAL BIN MODE

    hm_i2c_write( REG_BIT_CONTROL, 0x20);//Set the output to send 1 bit serial

    hm_i2c_write( REG_PMU_PROGRAMMABLE_FRAMECNT, 0x01);//set the number of frames to be sent out, it sends N frames

}

