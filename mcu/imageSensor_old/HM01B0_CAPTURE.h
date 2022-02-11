/*
 * HM01B0_CAPTURE.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Ali
 */

#ifndef HM01B0_CAPTURE_H_
#define HM01B0_CAPTURE_H_

#include "HM01B0_GPIO.h"
#include "HM01B0_FUNC.h"
#include "HM01B0_CLK.h"

/*if capture_mode=0x01 => it streams. This is useful for tetsing purposes*/
/*if capture_mode=0x03 => it takes frame_count shots. */
#define capture_mode  0x03
#define CAMERA_DEBUG 0
#define BLE_DEBUG 0

//static uint32_t ble_bytes_sent_counter = 0;
uint32_t ble_bytes_sent_counter = 0;

void hm_peripheral_init(void);


void hm_single_capture(void);


void hm_single_capture_spi(void);


#endif /* HM01B0_CAPTURE_H_ */