/*
 * MOTOR.c
 *
 *  Created on: Jan 2nd, 2020
 *      Author: Ali Najafi
 */

#include "MOTOR.h"

uint8_t press_mesearements[MAX_STEP_TIMES*4]; //each pressure reading is 24bits!
double press_mesearements_human[MAX_STEP_TIMES];
uint32_t press_measurement_cnt;
extern uint32_t press_measurement_human_cnt = 0;
uint8_t measured_press[4];
bool init_press_mid_reach = true;
uint32_t measured_press_human;

bool thresh1;
bool thresh2;
bool pressureThresholdCheck;
bool pcheck;
bool pin_test1 = false;

//void seal_check1(void) {
//    read_pressure_data(measured_press);
////    nrf_gpio_pin_set(PIN_TEST);
////    read_pressure_data_interrupt();
//    //check if the seal is still good
//    uint32_t measured_press_human = convert_to_human(measured_press);
//    uint32_t max_press_human = convert_to_human(max_press);
//    uint32_t min_press_human = convert_to_human(min_press);
//
//    bool thresh1 = compare_pressure_human2(measured_press,max_press,50);
//    bool thresh2 = compare_pressure_human2(measured_press,min_press,50);
//    bool pressureThresholdCheck =  thresh1||thresh2;
//
//    if (sweep_dir == 0) {
//        if(!move_to_init ) {
//            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
//                if(press_measurement_cnt >= 4*SEAL_PRESS_CNT){
//                    if (!pressureThresholdCheck) {
//                      uint32_t press1 = convert_to_human(measured_press);
//                      uint32_t press2 = convert_to_human(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1));
//                      uint32_t diff = abs(press1-press2);
//                      // bool comp_result = compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), mtr_seal_check_threshold);
//                      bool comp_result_human = compare_pressure_human(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), mtr_seal_check_threshold);
//                      if(!comp_result_human){
//                          //motor goes one step back
//                          seal_flag = false;
//                       } else {
//                          seal_flag = true;
//                      }
//                    }
//                    else {
//                      seal_flag = true;
//                    }
//                }
//            }
//
//            if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
//                if (!pressureThresholdCheck) {
//                    // bool comp_result = compare_pressure(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), measured_press, mtr_seal_check_threshold);
//                    bool comp_result_human = compare_pressure_human(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), measured_press, mtr_seal_check_threshold);
//                    if(!comp_result_human) {
//                        //motor goes one step back
//                        seal_flag = false;
//                    } else{
//                        seal_flag = true;
//                    }
//                }
//                else {
//                    seal_flag = true;
//                }
//            }
//        }
//
//        if(move_to_init) {
//            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
//                if(press_measurement_cnt >= 4*SEAL_PRESS_CNT){
//                    // bool comp_result = !(compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT));
//                    bool comp_result_human = !(compare_pressure_human(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT_HUMAN));
//
//                    if (comp_result_human) {
//                        //motor goes one step back
//                        seal_flag = false;
//                    } else{
//                        seal_flag = true;
//                    }
//                }
//            }
//
//            if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
//                // bool comp_result = !(compare_pressure(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT));
//                bool comp_result_human = !(compare_pressure_human(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT_HUMAN));
//
//                if (comp_result_human){
//                    //motor goes one step back
//                    seal_flag = false;
//                } else{
//                    seal_flag = true;
//                }
//            }
//        }
//    }
//    else if (sweep_dir == 1) {
//        if(!move_to_init ) {
//            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
//                if(press_measurement_cnt >= 4*SEAL_PRESS_CNT){
//                    if (!pressureThresholdCheck) {
//                        // bool comp_result = !(compare_pressure(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), measured_press, mtr_seal_check_threshold));                        
//                        bool comp_result_human = !(compare_pressure_human(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), measured_press, mtr_seal_check_threshold));                        
//                        if (comp_result_human){
//                            //motor goes one step back
//                            seal_flag = false;
//                        } else{
//                            seal_flag = true;
//                        }
//                    }
//                    else {
//                        seal_flag = true;
//                    }
//                }
//            }
//
//        if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
//                if (!pressureThresholdCheck) {
//                    // bool comp_result = !(compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), mtr_seal_check_threshold));                    
//                    bool comp_result_human = !(compare_pressure_human(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT+1), mtr_seal_check_threshold));                    
//                    if(comp_result_human) {
//                        //motor goes one step back
//                        seal_flag = false;
//                    } else{
//                        seal_flag = true;
//                    }
//                }
//                else {
//                    seal_flag = true;
//                }
//            }
//        }
//
//        if(move_to_init) {
//            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
//                if(press_measurement_cnt >= 4*SEAL_PRESS_CNT){
//                    // bool comp_result = !(compare_pressure(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT));
//                    bool comp_result_human = !(compare_pressure_human(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT_HUMAN));
//
//                    if (comp_result_human) {
//                        //motor goes one step back
//                        seal_flag = false;
//                    } else{
//                        seal_flag = true;
//                    }
//                }
//            }
//
//            if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
//                // bool comp_result = !(compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT));
//                bool comp_result_human = !(compare_pressure_human(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT_HUMAN));
//
//                if (comp_result_human){
//                    //motor goes one step back
//                    seal_flag = false;
//                } else{
//                    seal_flag = true;
//                }
//            }
//        }
//    }
//
//    if (seal_flag) {
//        memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
//        press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
//    } 
//    else {
//        // press_measurement_cnt = 4;
//        memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
//        press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
//    }
//}

void seal_check(void) {
//    read_pressure_data(measured_press);
    // pressure_sensor_init();
    read_pressure_data_interrupt();

    thresh1 = !(compare_pressure_range(measured_press,max_press,40000));
    thresh2 = !(compare_pressure_range(measured_press,min_press,40000));
    pressureThresholdCheck =  thresh1||thresh2;
    // bool pressureThresholdCheck =  false;
//    nrf_gpio_pin_set(PIN_TEST);
//    read_pressure_data_interrupt();
    //check if the seal is still good
    if(pressureThresholdCheck&&mic_check==1){
        seal_flag=true;
    }
    else {
    if (sweep_dir == 0) {
        if(!move_to_init ) {
//            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
                if(press_measurement_cnt >= 4*(SEAL_PRESS_CNT + INIT_PRESS_NOT_CARE)){
                    pcheck = compare_pressure_range(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT), mtr_seal_check_threshold);
                    if (pcheck && mic_check==1) {
                        seal_flag = true;
                    }
                    else {
                        seal_flag = false;
                    }
                }
//            }

          // if (press_measurement_cnt >= 4) {
//          if (mtr_dir == 1 && dir_just_changed == 0) {
                  // if(!(compare_pressure_range(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT), measured_press, mtr_seal_check_threshold))){
                  //     //motor goes one step back
                  //     seal_flag = false;
                  // } else{
                  //     seal_flag = true;
                  // }
            // }
//            }
        }

        if(move_to_init) {
            if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
                if(press_measurement_cnt >= 4*(SEAL_PRESS_CNT + INIT_PRESS_NOT_CARE)){
                    if (mic_check == 2) {
                        seal_flag = false;
                    }
                    else {
                        pcheck = compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT);
                        if (pcheck && mic_check==1) {
                            seal_flag = true;
                        }
                        else {
                            seal_flag = false;
                        }
                    }
                }
            }

            if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
                if (mic_check == 2) {
                    seal_flag = false;
                }
                else {
                    pcheck = compare_pressure(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT);
                    if (pcheck && mic_check==1) {
                        seal_flag = true;
                    }
                    else {
                        seal_flag = false;
                    }
                }
            }
        }
    }
    // else if (sweep_dir == 1) {
    //     if(!move_to_init ) {
    //         if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
    //             if(press_measurement_cnt >= 4*(SEAL_PRESS_CNT + INIT_PRESS_NOT_CARE)){
    //                 bool pcheck = compare_pressure(press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT), measured_press, mtr_seal_check_threshold);
    //                 if (pcheck && mic_check==1) {
    //                     seal_flag = true;
    //                 }
    //                 else {
    //                     seal_flag = false;
    //                 }
    //             }
    //         }

    //     if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
    //             bool pcheck = compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 4*(SEAL_PRESS_CNT), mtr_seal_check_threshold);
    //             if (pcheck && mic_check==1) {
    //                 seal_flag = true;
    //             }
    //             else {
    //                 seal_flag = false;
    //             }
    //         }
    //     }

    //     if(move_to_init) {
    //         if ((mtr_dir == 0 || mtr_dir == 2) && dir_just_changed == 0) {
    //             if(press_measurement_cnt >= 4*(SEAL_PRESS_CNT + INIT_PRESS_NOT_CARE)){
    //                 bool pcheck = compare_pressure(press_mesearements + press_measurement_cnt - 8, measured_press, PRESS_CHANGE_STEP_MV_INIT);
    //                 if (pcheck && mic_check==1) {
    //                     seal_flag = true;
    //                 }
    //                 else {
    //                     seal_flag = false;
    //                 }
    //             }
    //         }

    //         if (press_measurement_cnt >= 4 && mtr_dir == 1 && dir_just_changed == 0) {
    //             bool pcheck = compare_pressure(measured_press, press_mesearements + press_measurement_cnt - 8, PRESS_CHANGE_STEP_MV_INIT);
    //             if (pcheck && mic_check==1){
    //                 seal_flag = true;
    //             }
    //             else {
    //                 seal_flag = false;
    //             }
    //         }
    //     }
    // }
    }
    press_measurement_human_cnt = press_measurement_human_cnt + 1;
    if (seal_flag) {
//        pressure_seal_marker = press_mesearements;
//        measured_press_human = (uint32_t)convert_to_human(measured_press);
//        memcpy(press_mesearements_human + press_measurement_human_cnt, measured_press_human, sizeof(measured_press_human));
        memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
        press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
    } 
    else {
//        press_measurement_cnt = 4;
//        measured_press_human = (uint32_t)convert_to_human(measured_press);
//        memcpy(press_mesearements_human + press_measurement_human_cnt, measured_press_human, sizeof(measured_press_human));
        memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
        press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
    }
}

uint32_t convert8to32(uint8_t* press1) {
  return (((uint32_t)press1[0] << 24) | (uint32_t)press1[1] << 16) | ((uint32_t)press1[2] << 8) | ((uint32_t) press1[3]);
}

void pressure_range_check(void){//checks the seal and the fact the pressure should be lower than the max pressure
    if (sweep_dir == 0) {
    //if pressure is higher than max pressure and we are increasing the pressure or if pressure is lower than the min pressure and we are decreasing the pressure then turnoff the motor
        // uint32_t measured_press_human = convert_to_human(measured_press);
        // uint32_t max_press_human = convert_to_human(max_press);
        // uint32_t min_press_human = convert_to_human(min_press);
        // uint32_t init_press_human = convert_to_human(init_press);

        bool b0 = compare_pressure(measured_press, max_press, 0);
        bool b1 = compare_pressure(min_press, measured_press, 0);
        bool b2 = compare_pressure(measured_press, init_press, 0);

        switch(mtr_dir) {
        case 0:            
            if (b0) {
//                keep_max_min = true;
                keep_num = keep_num_val;
                motor_loc_status = MAX_PRESS_REACH;
                status_sent = false;
                
                mtr_dir++;
                dir_just_changed = DIR_JUST_CHANGED_VAL;
                
                pressure_index = press_measurement_cnt/4;
                pressure_value = convert8to32(measured_press);

                nrf_drv_timer_disable(&MTR_PWM_TIMER);
                nrf_drv_timer_extended_compare(
                     &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_pwm_timer_val, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
                nrf_drv_timer_enable(&MTR_PWM_TIMER); 
            }

            break;

        case 1: 
            if (b1) {
//                keep_max_min = true;
                keep_num = keep_num_val;
                motor_loc_status = MIN_PRESS_REACH;
                status_sent = false;

                mtr_dir++;
                dir_just_changed = DIR_JUST_CHANGED_VAL;

                pressure_index = press_measurement_cnt/4;
                pressure_value = convert8to32(measured_press);

                nrf_drv_timer_disable(&MTR_PWM_TIMER);
                nrf_drv_timer_extended_compare(
                     &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_fast_speed, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
                nrf_drv_timer_enable(&MTR_PWM_TIMER); 
            }

            if(init_press_mid_reach) {
                if (compare_pressure(init_press, measured_press, 0)) {
                    motor_loc_status = INIT_PRESS_MID;
                    status_sent = false;
                    init_press_mid_reach = false;
                }
            }

            break;

        case 2: 
            if (b2) {
                motor_loc_status = INIT_PRESS_REACH;
                status_sent = false;

                mtr_dir++;
                tymp_done = true;
                mtr_stop();
            }

            break;
        }
    }
    else if (sweep_dir == 1) {
        switch(mtr_dir){
        case 0:
            if (compare_pressure(min_press, measured_press, 0)) {
//                keep_max_min = true;
                keep_num = keep_num_val;

                motor_loc_status = MIN_PRESS_REACH;
                status_sent = false;

                mtr_dir++;
                dir_just_changed = DIR_JUST_CHANGED_VAL;
            }
            break;

        case 1: 
            if (compare_pressure(measured_press, max_press, 0)) {
//                keep_max_min = true;
                keep_num = keep_num_val;

                motor_loc_status = MAX_PRESS_REACH;
                status_sent = false;

                mtr_dir++;
                dir_just_changed = DIR_JUST_CHANGED_VAL;
            }

            if(init_press_mid_reach){
                if (compare_pressure(measured_press, init_press, 0)){
                    motor_loc_status = INIT_PRESS_MID;
                    status_sent = false;
                    init_press_mid_reach = false;
                }
            }
            break;

        case 2: 
            if (compare_pressure(init_press, measured_press, 0)){
                motor_loc_status = INIT_PRESS_REACH;
                status_sent = false;

                mtr_dir++;
                tymp_done = true;
                mtr_stop();
            }
            break;
        }
    }
}

void mtr_move(uint32_t step_num, uint8_t mtr_dir1) {//
    counter_before_seal = 0;
    mtr_on = true;
    nrf_drv_timer_extended_compare(
         &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_pwm_timer_val, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
    currentstep = CURRENT_STEP_INIT;
    seal_flag = true;
    just_mtr_move = true;
    step_num_mtr = step_num;
    mtr_dir = mtr_dir1;
    keep_num = 0;
    nrf_drv_timer_enable(&MTR_PWM_TIMER);  
    nrf_gpio_pin_set(nSleep);//start the motor driver
}

void abortCheck(void) {
    int32_t step = mtr_step_counter8;
    int32_t start = mtr_start_point;

    int32_t x1 = step-start;
    int32_t x2 = start-step;
    bool b1 = x1 > MTR_MOVED_TOO_FAR_FORWARD_THRESHOLD;
    bool b2 = x2 > MTR_MOVED_TOO_FAR_BACKWARD_THRESHOLD;
    // end the measurement
    if (seal_found_counter >= 1 && (b1||b2)) {
        tymp_aborted = true;
        tymp_done = true;
        mtr_stop();
    }
}

void mtr_pwm_timer_event_handler(nrf_timer_event_t event_type, void* p_context) {
    uint32_t ret_code;
    //Just update the duty-cycle
    //calculate the next

//    // toggle the gpio pin everytime this interrupt is triggered
//    pin_test1 = !pin_test1;
//    if(pin_test1) {
//        nrf_gpio_pin_set(PIN_TEST);
//    } else {
//        nrf_gpio_pin_clear(PIN_TEST);
//    }

    // one sine wave cycle is output
    // Each set of microsteps represents one quarter of the sine wave
    
    if (!mtr_on) {
        return;
    }

    if (MICROSTEPS == 8)
      counter_before_seal+=8;
    else if (MICROSTEPS == 64) {
      counter_before_seal++;
    }

    if (keep_num != 0) {
        keep_num--;
        if (keep_num % (4*MICROSTEPS)==0) {
            read_pressure_data_interrupt();
            memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
            press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
        }
    } else {
        mtr_move_logic();
        if (currentstep == CURRENT_STEP_INIT) {
            //logic to update the counters
            press_read_cnt--;
            if (press_read_cnt == 0) {
                press_read_cnt = PRESS_READ_CNT;

                if (!move_to_init && !just_mtr_move) {
                    //Why are we checking this here? Because we update microstepcurve64 here not microstepcurve8?
                    abortCheck();

                    if (!seal_flag) {//if it was not sealed we should go one step ahead and see what happens!
                        if (seal_check_cnt == 0) {
                            seal_check_cnt = SEAL_PRESS_CNT;
                            if(mtr_dir == 0) {
                                seal_check();
                            }

                            if (!seal_flag) {
                                if(mtr_dir == 0) {
                                    mtr_dir = 1;
                                } else {
                                    mtr_dir = 0;
                                }
                                motor_loc_status = SEAL_FALSE_INFO;
                                seal_status = false;
                                status_sent = false;

                                MICROSTEPS = 64;
                                microstepcurve = microstepcurve64;

                                seal_lost_counter += 1;
                            } else {   
                                pressure_seal_marker = press_measurement_cnt;
                                pressure_index = press_measurement_cnt/4;
                                pressure_value = convert8to32(measured_press);
                                motor_loc_status = SEAL_TRUE_INFO;
                                seal_status = true;
                                status_sent = false;
                                seal_found_counter += 1;

                                MICROSTEPS = 8;
                                microstepcurve = microstepcurve8;
                            }
                        } else {
                            if( mtr_dir == 0 ) {
                                read_pressure_data_interrupt();
                                memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
                                press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure
                            }
                            seal_check_cnt--;
                        }
                    }
                    else {
                        seal_check();
                        if(seal_flag) {
                            if (dir_just_changed > 0) {
                                dir_just_changed--;
                            }
                            pressure_range_check();
                        }
                    }
                }
            }

            if (just_mtr_move) {
                step_num_mtr--;
                if (step_num_mtr == 0){
                    just_mtr_move = false;
                    tymp_done = false;
                    mtr_stop();
                }
            }

            if (move_to_init) {
    //            if (debug_array_counter==0){
    //              mtr_step_counter8=67628;
    //              mtr_step_counter64=67628;
    //            }
                if (debug_array_counter < 500&&!seal_flag) {
                  debug_array[debug_array_counter++] = mtr_step_counter64;
                  debug_array2[debug_array_counter2++] = mtr_step_counter8;
                }
                move_to_init_logic_fast();
            }
        }
    }
}

void move_to_init_logic_fast(void) {
    bool b1 = (mtr_step_counter8 == mtr_start_point);
    bool b2 = (mtr_step_counter16 == mtr_start_point);
    bool b3 = (mtr_step_counter32 == mtr_start_point);
    bool b4 = (mtr_step_counter64 == mtr_start_point);
    
    if (move_to_init_seal_lost && !seal_flag && b1 && b2 && b3 && b4) {
      finish_move_to_init();
    } 
    else if (!move_to_init_seal_lost) {
      if (seal_flag) {
        MICROSTEPS = 64;
        microstepcurve = microstepcurve64;

        if (seal_check_cnt == 0){
            mtr_seal_check_threshold = 1000;//the previous seal setting is hard to find seal. Here, we want to set it in a way that is hard to lose!
            seal_check_cnt = SEAL_PRESS_CNT;
            seal_check();

//            bool rcheck = compare_pressure_range(init_press, measured_press, SEAL_LOSE_PRESSURE_THRESHOLD);
//            seal_flag = seal_flag && rcheck;
             
            if (seal_flag){
                move_to_init_seal_lost = false;
                if(mtr_dir == 0){
                    mtr_dir = 1;
                } else{
                    mtr_dir = 0;
                } 
            }
        } else {
//            read_pressure_data(measured_press);
            // pressure_sensor_init();
            read_pressure_data_interrupt();
            memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
            press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure

            seal_check_cnt--;
        }
    } else {
        move_to_init_seal_lost = true;

        int32_t t1 = mtr_step_counter64;
        int32_t t2 = mtr_step_counter32;
        int32_t t3 = mtr_step_counter16;
        int32_t t4 = mtr_step_counter8;
        int32_t s = mtr_start_point;
        
        int32_t c1 = (t1-s)/8;
        int32_t c2 = (t2-s)/4;
        int32_t c3 = (t3-s)/2;
        int32_t c4 = (t4-s);

        int32_t converted_steps = c1+c2+c3+c4;

        mtr_step_counter64 = mtr_start_point;
        mtr_step_counter32 = mtr_start_point;
        mtr_step_counter16 = mtr_start_point;
        mtr_step_counter8 = mtr_start_point+converted_steps;

        if (mtr_step_counter8!=mtr_start_point) {
            if (mtr_step_counter8 < mtr_start_point) {
                mtr_dir=0;
            }
            else if (mtr_step_counter8 > mtr_start_point) {
                mtr_dir=1;
            }
            MICROSTEPS=8;
            microstepcurve = microstepcurve8;
        }
        else {
          finish_move_to_init();
        }
    }
    }
}

void finish_move_to_init(void) {
  move_to_init = false;
  mtr_stop();
  motor_loc_status = GONE_BACK_TO_INIT;
  status_sent = false;
  tymp_done = false;
  move_to_init_seal_lost = false;
  ble_bytes_sent_counter = 0;
  press_measurement_cnt = 0;
  mtr_dir = 0;
  seal_flag = false;
  currentstep = CURRENT_STEP_INIT;
  memset(press_mesearements, 0x00, MAX_STEP_TIMES*4);//erase the press_measurements memory!
  if (start_cmd_in_progress) {
      start_cmd_in_progress = false;
  }

  if (stop_cmd_in_progress) {
      stop_cmd_in_progress = false;
  }
}

void microstep_move(void) {
  ocra = ocrb = 0;
  if ( (currentstep >= 0) && (currentstep < MICROSTEPS)) {
      mtr_sine_quarter = 0;
      ocra = microstepcurve[MICROSTEPS - currentstep];
      ocrb = microstepcurve[currentstep];
  } else if  ( (currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS*2)) {
      mtr_sine_quarter = 1;
      ocra = microstepcurve[currentstep - MICROSTEPS];
      ocrb = microstepcurve[MICROSTEPS*2 - currentstep];
  } else if  ( (currentstep >= MICROSTEPS*2) && (currentstep < MICROSTEPS*3)) {
      mtr_sine_quarter = 2;
      ocra = microstepcurve[MICROSTEPS*3 - currentstep];
      ocrb = microstepcurve[currentstep - MICROSTEPS*2];
  } else if  ( (currentstep >= MICROSTEPS*3) && (currentstep < MICROSTEPS*4)) {
      mtr_sine_quarter = 3;
      ocra = microstepcurve[currentstep - MICROSTEPS*3];
      ocrb = microstepcurve[MICROSTEPS*4 - currentstep];
  }
}

void mtr_move_logic(void) {
  if (currentstep==4*MICROSTEPS) {
    currentstep = 0;
    microstep_move();
  }
  else {  
    microstep_move();
  }
  currentstep++;
  
  if (sweep_dir == 0) {
    if(mtr_dir != 1){//if mtr_dir == 0 or == 2 => increase the pressure
        incMtrStepCounter();
    } else if (mtr_dir == 1) {//otherwise decrease the pressure
        decMtrStepCounter();
    } 
  }
  else if (sweep_dir == 1) {
      if(mtr_dir != 1){//if mtr_dir == 0 or == 2 => increase the pressure
          incMtrStepCounter();
      } else if (mtr_dir == 1) {//otherwise decrease the pressure
          decMtrStepCounter();
      } 
  }

  mtr_pwm_duty_set( ocra, ocrb, mtr_sine_quarter, mtr_dir);
  //        nrf_gpio_pin_set(PIN_TEST);
}

void mtr_move_to_init(void){ 
    mtr_on = true;
    MICROSTEPS = 64;
    microstepcurve = microstepcurve64;
    if (MOVE_TO_INIT == 1) {
        seal_check_cnt = SEAL_PRESS_CNT;
        dir_just_changed = 0;
        keep_num = 0;
        nrf_drv_timer_extended_compare(
             &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_pwm_timer_val, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
        nrf_gpio_cfg_output(PIN_TEST);
        dir_just_changed = 0;
        move_to_init = true;
        currentstep = CURRENT_STEP_INIT;
        nrf_drv_timer_enable(&MTR_PWM_TIMER); //start the motor driver
        nrf_gpio_pin_set(nSleep);
    }
    else {
        if (start_cmd_in_progress) {
            start_cmd_in_progress = false;
        }
        
        if (stop_cmd_in_progress) {
            stop_cmd_in_progress = false;
        }
    }
}

void mtr_start(void) {
    pcheck = false;
    mtr_on = true;
    counter_before_seal = 0;
    pressure_index = 0;
    pressure_value = 0;
//    max_pressure_index = 0;
//    min_pressure_index = 0;
    // read_pressure_data(measured_press);
    read_pressure_data_interrupt();
    memcpy(press_mesearements + press_measurement_cnt, measured_press, sizeof(measured_press));
    press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure

    press_measurement_human_cnt = 0;

    keep_num = 0;
    nrf_drv_timer_extended_compare(
         &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_fast_speed, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
    init_press_mid_reach = true;
    nrf_gpio_cfg_output(PIN_TEST);
    dir_just_changed = 0;
    seal_check_cnt = SEAL_PRESS_CNT;
    nrf_drv_timer_enable(&MTR_PWM_TIMER); 
    currentstep = CURRENT_STEP_INIT;
    seal_flag = false;
    nrf_gpio_pin_set(nSleep);//start the motor driver
}

void incMtrStepCounter() {
    if (MICROSTEPS == 8) {
        mtr_step_counter8++;
    }
    else if (MICROSTEPS == 16) {
        mtr_step_counter16++;
    }
    else if (MICROSTEPS == 32) {
        mtr_step_counter32++;
    }
    else if (MICROSTEPS == 64) {
        mtr_step_counter64++;
    }
}

void decMtrStepCounter() {
    if (MICROSTEPS == 8) {
        mtr_step_counter8--;
    }
    else if (MICROSTEPS == 16) {
        mtr_step_counter16--;
    }
    else if (MICROSTEPS == 32) {
        mtr_step_counter32--;
    }
    else if (MICROSTEPS == 64) {
        mtr_step_counter64--;
    }
}

void mtr_pwm_duty_set(uint32_t ocrb, uint32_t ocra, uint8_t mtr_sine_quarter, uint8_t mtr_dir) {
    uint32_t ocr1, ocr2;
    uint8_t channel1, channel2;

    ocr1 = ocra;
    ocr2 = ocrb;

    if (sweep_dir == 0) {
        if(mtr_dir != 1){//if mtr_dir == 0 or == 2 => increase the pressure
            channel1 = 0;
            channel2 = 1;
        } else if (mtr_dir == 1) {//otherwise decrease the pressure
            channel1 = 1;
            channel2 = 0;
        } 
    }
    else if (sweep_dir == 1) {
        if(mtr_dir != 1){//if mtr_dir == 0 or == 2 => increase the pressure
            channel1 = 1;
            channel2 = 0;
        } else if (mtr_dir == 1) {//otherwise decrease the pressure
            channel1 = 0;
            channel2 = 1;
        } 
    }

    switch(mtr_sine_quarter){
        case 0:
            while (app_pwm_channel_duty_set(&PWM1, channel1, ocr1) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM1, channel2, 10000UL) == NRF_ERROR_BUSY); 

            while (app_pwm_channel_duty_set(&PWM2, 0, 10000UL) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM2, 1, ocr2) == NRF_ERROR_BUSY); 
            break;

        case 1:
            while (app_pwm_channel_duty_set(&PWM1, channel1, ocr1) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM1, channel2, 10000UL) == NRF_ERROR_BUSY); 

            while (app_pwm_channel_duty_set(&PWM2, 0, ocr2) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM2, 1, 10000UL) == NRF_ERROR_BUSY); 
            break;

        case 2:
            while (app_pwm_channel_duty_set(&PWM1, channel1, 10000UL) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM1, channel2, ocr1) == NRF_ERROR_BUSY); 

            while (app_pwm_channel_duty_set(&PWM2, 0, ocr2) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM2, 1, 10000UL) == NRF_ERROR_BUSY); 
            break;

        case 3:
            while (app_pwm_channel_duty_set(&PWM1, channel1, 10000UL) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM1, channel2, ocr1) == NRF_ERROR_BUSY); 

            while (app_pwm_channel_duty_set(&PWM2, 0, 10000UL) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM2, 1, ocr2) == NRF_ERROR_BUSY); 
            break;

        default:
            break;
    }
}

void mtr_stop(void){
    int32_t s64 = mtr_step_counter64;
    int32_t s32 = mtr_step_counter32;
    int32_t s16 = mtr_step_counter16;
    int32_t s8 = mtr_step_counter8;
    int32_t s = mtr_start_point;
    
    while(abs(s64-s)%(4*64) != 0 ||
          abs(s32-s)%(4*32) != 0 ||
          abs(s16-s)%(4*16) != 0 ||
          abs(s8-s)%(4*8) != 0) {    
        s64 = mtr_step_counter64;
        s32 = mtr_step_counter32;
        s16 = mtr_step_counter16;
        s8 = mtr_step_counter8;
    }

    mtr_on = false;

    nrf_gpio_pin_clear(nSleep);//stop the motor driver
//    nrf_drv_timer_disable(&MTR_STEP_TIMER); //disable the pressure step timer
    nrf_drv_timer_disable(&MTR_PWM_TIMER); //disable the pwm timer timer     
    while (app_pwm_channel_duty_set(&PWM1, 0, 0UL) == NRF_ERROR_BUSY);
    while (app_pwm_channel_duty_set(&PWM1, 1, 0UL) == NRF_ERROR_BUSY); 

    while (app_pwm_channel_duty_set(&PWM2, 0, 0UL) == NRF_ERROR_BUSY);
    while (app_pwm_channel_duty_set(&PWM2, 1, 0UL) == NRF_ERROR_BUSY); 
    counter_before_seal = 0;
}

void mtr_pwm_timer_init(void) {
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_timer_config_t timer_cfg = NRFX_MTR_PWM_TIMER_CONFIG;

    err_code = nrf_drv_timer_init(&MTR_PWM_TIMER, &timer_cfg, mtr_pwm_timer_event_handler);
    APP_ERROR_CHECK(err_code);

//    nrf_drv_timer_extended_compare(
//         &MTR_PWM_TIMER, NRF_TIMER_CC_CHANNEL3, mtr_pwm_timer_val, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);

}

void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

void pwm_ready_callback2(uint32_t pwm_id)    // PWM callback function
{
    ready_flag2 = true;
}

void mtr_pwm_init(void) {
    ret_code_t err_code;

    /* 2-channel PWM, 200Hz, output on DK LED pins. */
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_2CH(MTR_PWM_PERIOD, PIN_AIN1, PIN_AIN2);
    app_pwm_config_t pwm2_cfg = APP_PWM_DEFAULT_CONFIG_2CH(MTR_PWM_PERIOD, PIN_BIN1, PIN_BIN2);

    /* Switch the polarity of the channel. */
    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
    pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    pwm2_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
    pwm2_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize and enable PWM. */
    err_code = app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM1);

    err_code = app_pwm_init(&PWM2,&pwm2_cfg,pwm_ready_callback2);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM2);   

    nrf_gpio_cfg_output(PIN_AIN1);
    nrf_gpio_cfg_output(PIN_AIN2);

    nrf_gpio_cfg_output(PIN_BIN1);
    nrf_gpio_cfg_output(PIN_BIN2);

    nrf_gpio_cfg_output(nSleep);

    nrf_gpio_pin_clear(PIN_AIN1);
    nrf_gpio_pin_clear(PIN_AIN2);

    nrf_gpio_pin_clear(PIN_BIN1);
    nrf_gpio_pin_clear(PIN_BIN2);
}
