/*
 * TYMP_FUNC.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Ali Najafi
 */

#ifndef TYMP_FUNC_H_
#define TYMP_FUNC_H_

#include <math.h>
#include "MOTOR.h"
#include "PRESSURE_SENSOR.h"

extern uint8_t max_press[4] = {0,0,0,0};
extern uint8_t min_press[4] = {0,0,0,0};  
extern uint8_t init_press[4] = {0,0,0,0};
extern uint8_t max_range[3];
extern uint8_t min_range[3];
extern bool tymp_done = false;
extern uint32_t ble_bytes_sent_counter;

void tymp_init(void);
void calculate_max_min_press(uint8_t* init_press, uint8_t* max_range, uint8_t* min_range, uint8_t* max_press, uint8_t* min_press);
bool compare_pressure(uint8_t* press1, uint8_t* press2, uint32_t press_offset);
bool compare_pressure_range(uint8_t* press1, uint8_t* press2, uint32_t press_offset);
bool compare_pressure_human(uint8_t* press1, uint8_t* press2, uint32_t press_offset);
bool compare_pressure_human2(uint8_t* press1, uint8_t* press2, uint32_t press_offset);
double convert_to_human(uint8_t* press1);
void tymp_start(void);
void tymp_move_to_init(void);
void tymp_stop(void);


//void set_max_pressure(unit8_t* max_pressure_pointer);
//void set_min_pressure(unit8_t* min_pressure_pointer);
//void set_press_step(unit8_t* press_step);
//void set_sweep_duration(unit8_t* sweep_duration);
//void check_seal(void);
//void start_tymp(void);
//void stop_tymp(void);


#endif /* TYMP_FUNC_H_ */