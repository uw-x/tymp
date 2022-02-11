/*
 * TYMP_FUNC.c
 *
 *  Created on: Dec 30, 2019
 *      Author: Ali Najafi
 */
#include "TYMP_FUNC.h"


void tymp_init(void){
    //init the motor
    mtr_pwm_init();
    mtr_pwm_timer_init();

    //init the pressure sensor
    pressure_sensor_init();
//    read_pressure_data_interrupt();
    nrf_delay_ms(3);

    press_measurement_cnt = 0;
    ble_bytes_sent_counter = 0;
    
    tymp_done = false;
 }

void calculate_max_min_press(uint8_t* init_press, uint8_t* max_range, uint8_t* min_range, uint8_t* max_press, uint8_t* min_press){//max_range and min_range are 3 bytes!
     
    uint32_t num_init = 0; 
    uint32_t num_max = 0; 
    uint32_t num_min = 0;

    uint32_t num_max1 = 0;
    uint32_t num_min1 = 0;

    num_init = ((uint32_t)init_press[1] << 16) | ((uint32_t)init_press[2] << 8) | ((uint32_t) init_press[3]);
    num_max1 = ((uint32_t)max_range[0] << 16) | ((uint32_t)max_range[1] << 8) | ((uint32_t) max_range[2]); 
    num_max = num_init + num_max1;
    num_min1 = ((uint32_t)min_range[0] << 16) | ((uint32_t)min_range[1] << 8) | ((uint32_t) min_range[2]); 
    num_min = num_init - num_min1;

    max_press[1] = ((uint8_t) (num_max >> 16));
    max_press[2] = ((uint8_t) (num_max >> 8));
    max_press[3] = ((uint8_t) num_max );

    min_press[1] = ((uint8_t) (num_min >> 16));
    min_press[2] = ((uint8_t) (num_min >> 8));
    min_press[3] = ((uint8_t) num_min );
    
 }


bool compare_pressure(uint8_t* press1, uint8_t* press2, uint32_t press_offset){
    
    uint32_t num1, num2;
    num1 = ((uint32_t)press1[1] << 16) | ((uint32_t)press1[2] << 8) | ((uint32_t) press1[3]);
    num2 = ((uint32_t)press2[1] << 16) | ((uint32_t)press2[2] << 8) | ((uint32_t) press2[3]);
    
    if (num1 >= (num2 + press_offset)){
        return true;
    } else {
        return false;
    }

}

bool compare_pressure_range(uint8_t* press1, uint8_t* press2, uint32_t press_offset){
    
    uint32_t num1, num2;
    num1 = ((uint32_t)press1[1] << 16) | ((uint32_t)press1[2] << 8) | ((uint32_t) press1[3]);
    num2 = ((uint32_t)press2[1] << 16) | ((uint32_t)press2[2] << 8) | ((uint32_t) press2[3]);
    
    if (abs(num1 - num2) > press_offset){
        return true;
    } else {
        return false;
    }

}


bool compare_pressure_human(uint8_t* press1, uint8_t* press2, uint32_t press_offset){
    
    uint32_t convert1=convert_to_human(press1);
    uint32_t convert2=convert_to_human(press2);

    if (abs(convert1 - convert2) >= press_offset){
        return true;
    } else {
        return false;
    }
}

bool compare_pressure_human2(uint8_t* press1, uint8_t* press2, uint32_t press_offset){
    
    uint32_t convert1=convert_to_human(press1);
    uint32_t convert2=convert_to_human(press2);

    if (abs(convert1 - convert2) < press_offset){
        return true;
    } else {
        return false;
    }
}

double convert_to_human(uint8_t* press1){
    double num1;
    num1 = ((uint32_t)press1[1] << 16) | ((uint32_t)press1[2] << 8) | ((uint32_t) press1[3]);
    
    double pownum=pow(2.0,24.0);
    double omin=.1*pownum;
    double omax=.9*pownum;
    double rr = omax-omin;

    double numerator = (num1-omin);
    double convert1=numerator/rr;
    convert1=convert1*25*6895;
    convert1=convert1/10;

    return convert1;
}

void tymp_start(void){
    //initial pressure and max and min pressure should be measured here and never change again!
//    read_pressure_data(init_press);
//    memcpy(press_mesearements, init_press, sizeof(init_press));
//    calculate_max_min_press(init_press, max_range, min_range, max_press, min_press);
//    press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure 

//    seal_check_init();
    mic_check=0;
    memset(debug_array,sizeof(debug_array),100);
    memset(debug_array2,sizeof(debug_array2),100);
    debug_array_counter=0;
    debug_array_counter2=0;

    tymp_aborted = false;
    seal_found_counter = 0;
    seal_lost_counter = 0;

    num_timer_ticks = 0;
    MICROSTEPS = 64;
    microstepcurve = microstepcurve64;
    
    mtr_start();
}

void tymp_move_to_init(void){
    mtr_move_to_init();
}

void tymp_stop(void){
    mtr_stop();
    tymp_done = false;
    ble_bytes_sent_counter = 0;
    press_measurement_cnt = 0;
    mtr_dir = 0;
    seal_flag = true;
    currentstep = CURRENT_STEP_INIT;
    memset(press_mesearements, 0x00, MAX_STEP_TIMES*4);//erase the press_measurements memory!
    // nrf_delay_ms(100);
    tymp_move_to_init();
}
