/*
 * MOTOR.h
 *
 *  Created on: Jan 2nd, 2020
 *      Author: Ali Najafi
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"
#include <nrfx.h>
#include <hal/nrf_timer.h>

#include "TYMP_DEFINES.h"
#include "TYMP_FUNC.h"
#include "../ble_image_transfer_service.h"

BLE_ITS_DEF(m_its, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */

#define MTR_PWM_PERIOD 50L
uint8_t MICROSTEPS = 8;
//#define MICROSTEPS 8

uint32_t microstepcurve8[9] = {10000UL, 8100UL, 6200UL, 4500UL, 3000UL , 1700UL, 760UL, 200UL, 0UL};//this looks more correct
uint32_t microstepcurve16[17] = {10000UL, 9020UL, 8050UL, 7100UL, 6200UL, 5300UL, 4500UL, 3700UL, 3000UL, 2300UL, 1700UL, 1200UL, 800UL, 430UL, 200UL, 50UL, 0UL};
uint32_t microstepcurve32[33] = {10000UL, 9500UL, 9000UL, 8500UL, 8050UL, 7600UL, 7100UL, 6630UL, 6170UL, 5720UL, 5290UL, 4860UL, 4440UL, 4040UL, 3660UL, 3280UL, 2930UL, 2590UL, 2270UL, 1970UL, 1690UL, 1420UL, 1180UL, 960UL, 760UL, 580UL, 430UL, 300UL, 190UL, 110UL, 50UL, 12UL, 0};//this looks more correct
uint32_t microstepcurve64[65] = {10000UL, 9755UL, 9510UL, 9260UL, 9020UL, 8775UL, 8530UL, 8290UL, 8050UL, 7810UL, 7570UL, 7330UL, 7100UL, 6863UL, 6630UL, 6400UL, 6170UL, 5947UL, 5724UL, 5503UL, 5286UL, 5071UL, 4860UL, 4650UL, 4444UL, 4240UL, 4040UL, 3847UL, 3656UL, 3468UL, 3284UL, 3104UL, 2930UL, 2757UL, 2590UL, 2427UL, 2270UL, 2116UL, 1970UL, 1824UL, 1690UL, 1551UL, 1420UL, 1300UL, 1180UL, 1067UL, 960UL, 857UL, 760UL, 670UL, 580UL, 500UL, 430UL, 362UL, 300UL, 242UL, 190UL, 147UL, 110UL, 75UL, 50UL, 27UL, 12UL, 3UL, 0};

uint32_t* microstepcurve;

extern uint32_t debug_array[500]= {0};
extern uint32_t debug_array_counter = 0;
extern uint32_t debug_array2[500]= {0};
extern uint32_t debug_array_counter2 = 0;
extern int32_t debug_array3[500]= {0};
extern int32_t debug_array_counter3 = 0;
extern int32_t debug_array4[500]= {0};
extern int32_t debug_array_counter4 = 0;

#if (MICROSTEPS == 4)
uint32_t microstepcurve[MICROSTEPS+1] = {10000UL, 6170UL, 2930UL, 761UL, 0UL};
#elif (MICROSTEPS == 8)
///! A sinusoial microstepping curve for the PWM output (8-bit range) with 9 points - last one is start of next step.
//uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
//uint32_t microstepcurve[MICROSTEPS+1] = {0UL, 1951UL, 3827UL, 5556UL, 7071UL, 8315UL, 9239UL, 9808UL, 10000UL};
//uint32_t microstepcurve[MICROSTEPS+1] = {0UL, 200UL, 760UL, 1700UL, 3000UL , 4500UL, 6200UL, 8100UL, 10000UL};//good for bigger syringe
//uint32_t microstepcurve[MICROSTEPS+1] = {0UL, 200UL, 500UL, 1000UL, 2000UL , 3500UL, 5000UL, 8100UL, 10000UL};
//uint32_t microstepcurve[MICROSTEPS+1] = {0UL, 0UL, 0UL, 0UL, 0UL , 10000UL, 10000UL, 10000UL, 10000UL};
//uint32_t microstepcurve[MICROSTEPS+1] = {10000UL, 10000UL, 0UL, 0UL, 0UL , 0UL, 0UL, 0UL, 0UL};
//uint32_t microstepcurve[MICROSTEPS+1] = {0UL, 0UL, 0UL, 0UL, 0UL , 0UL, 0UL, 10000UL, 10000UL};
//uint32_t microstepcurve[MICROSTEPS+1] = {10000UL, 10000UL, 10000UL, 10000UL, 3000UL , 0UL, 0UL, 0UL, 0UL};//This is similar to what arduino does for single!

//uint32_t microstepcurve8[MICROSTEPS+1] = {10000UL, 8100UL, 6200UL, 4500UL, 3000UL , 1700UL, 760UL, 200UL, 0UL};//this looks more correct

#elif (MICROSTEPS == 16)
///! A sinusoial microstepping curve for the PWM output (8-bit range) with 17 points - last one is start of next step.
//uint32_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
//uint32_t microstepcurve[MICROSTEPS+1] = {0, 980, 1951, 2903, 3827, 4714, 5556, 6344, 7071, 7730, 8315, 8819, 9239, 9569, 9808, 9952, 10000};
//uint32_t microstepcurve16[MICROSTEPS+1] = {10000UL, 9020UL, 8050UL, 7100UL, 6200UL, 5300UL, 4500UL, 3700UL, 3000UL, 2300UL, 1700UL, 1200UL, 800UL, 430UL, 200UL, 50UL, 0UL};
#elif (MICROSTEPS == 32)

// original mstep
uint32_t microstepcurve[MICROSTEPS+1] = {10000UL, 9500UL, 9000UL, 8500UL, 8050UL, 7600UL, 7100UL, 6630UL, 6170UL, 5720UL, 5290UL, 4860UL, 4440UL, 4040UL, 3660UL, 3280UL, 2930UL, 2590UL, 2270UL, 1970UL, 1690UL, 1420UL, 1180UL, 960UL, 760UL, 580UL, 430UL, 300UL, 190UL, 110UL, 50UL, 12UL, 0};

#elif (MICROSTEPS == 64)
uint32_t microstepcurve[MICROSTEPS+1] = {10000UL, 9755UL, 9510UL, 9260UL, 9020UL, 8775UL, 8530UL, 8290UL, 8050UL, 7810UL, 7570UL, 7330UL, 7100UL, 6863UL, 6630UL, 6400UL, 6170UL, 5947UL, 5724UL, 5503UL, 5286UL, 5071UL, 4860UL, 4650UL, 4444UL, 4240UL, 4040UL, 3847UL, 3656UL, 3468UL, 3284UL, 3104UL, 2930UL, 2757UL, 2590UL, 2427UL, 2270UL, 2116UL, 1970UL, 1824UL, 1690UL, 1551UL, 1420UL, 1300UL, 1180UL, 1067UL, 960UL, 857UL, 760UL, 670UL, 580UL, 500UL, 430UL, 362UL, 300UL, 242UL, 190UL, 147UL, 110UL, 75UL, 50UL, 27UL, 12UL, 3UL, 0};
#endif

#define NRFX_MTR_PWM_TIMER_CONFIG                                                   \
{                                                                                    \
    .frequency          = NRF_TIMER_FREQ_62500Hz,\
    .mode               = NRFX_TIMER_DEFAULT_CONFIG_MODE,          \
    .bit_width          = TIMER_BITMODE_BITMODE_32Bit,\
    .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,                    \
    .p_context          = NULL                                                       \
}


#define PIN_TEST 31

#define PIN_AIN1 12
#define PIN_AIN2 13

#define PIN_BIN1 15
#define PIN_BIN2 14

#define nSleep 11

// this is about the same as 300 motor steps
#define MTR_MOVED_TOO_FAR_FORWARD_THRESHOLD 3200*1.5  //150
#define MTR_MOVED_TOO_FAR_BACKWARD_THRESHOLD 3200*2.4 //240

extern bool dir1complete=false;
extern bool dir2complete=false;
extern bool dir3complete=false;

extern uint32_t counter_before_seal = 0;
bool keep_max_min = false;
extern uint32_t keep_num_val = KEEP_NUM_VAL;//How long we would stay in the max and min pressure
uint32_t keep_num = 0;
extern uint8_t press_mesearements[MAX_STEP_TIMES*4]; //each pressure reading is 24bits!
extern double press_mesearements_human[MAX_STEP_TIMES]; //each pressure reading is 24bits!
extern uint32_t press_measurement_cnt;
extern uint8_t measured_press[4];
extern uint32_t mtr_start_point = 65536;
extern uint32_t mtr_step_counter8 = 65536;
extern uint32_t mtr_step_counter16 = 65536;
extern uint32_t mtr_step_counter32 = 65536;
extern uint32_t mtr_step_counter64 = 65536;
extern uint8_t seal_press_cnt = 0;//counter so that we check the seal each that many pressure reads! this way we can confirm seal more accurately
extern uint32_t pressure_seal_marker = 0;
extern uint8_t MOVE_TO_INIT = 1;
extern bool tymp_aborted = false;
extern uint32_t num_timer_ticks=0;
extern uint8_t mic_check = 0;
extern bool mtr_on = false;
extern bool start_cmd_in_progress = false;
extern bool stop_cmd_in_progress = false;
extern uint32_t measured_press_human;
extern bool wait_for_motor = false;
extern uint32_t mtr_fast_speed = MTR_FAST_SPEED;

uint32_t step_num_mtr;
bool move_to_init = false;//flag that shows we just want to move the motor to its initial position
bool just_mtr_move = false;//flag that shows that we just want to move the motor for debuging purposes!
uint8_t dir_just_changed = 0;//this flags shows when we just changed direction! In that case, we are not going to check the seal! because the pressure may not increase enough
//when we just changed direction!

uint8_t max_press[4];
uint8_t min_press[4];
uint8_t init_press[4];
uint8_t max_range[3];
uint8_t min_range[3];
bool tymp_done;
extern bool rcheck = false;

extern bool mtr_dir_error = false;

extern uint8_t mtr_dir = 0; //0: increasing pressure 1: decreasing pressure, 2: increase to initial pressure
extern bool seal_flag = false; // 0:not seal 1:seal
extern uint8_t motor_loc_status = 0; // 3: seal 4: not seal 5:MAX_PRESS_REACH 6:MIN_PRESS_REACH 7:INIT_PRESS_REACH
extern bool seal_flag_init = false; // 0:not seal 1:seal
extern uint8_t seal_check_cnt = SEAL_PRESS_CNT;// we check the seal every SEAL_PRESS_CNT number of times

extern bool seal_status = false;
extern bool status_sent = true;
extern uint8_t seal_found_counter = 0;
extern uint8_t seal_lost_counter = 0;
extern bool move_to_init_seal_lost = false;

extern bool step_counter_equal_start_point = false;

extern uint32_t pressure_index = 0;
extern uint32_t pressure_value = 0;
//extern uint32_t max_pressure_index = 0;
//extern uint32_t min_pressure_index = 0;

uint8_t mtr_sine_quarter = 0;
uint32_t ocra = 0; 
uint32_t ocrb = 0;
uint16_t currentstep = CURRENT_STEP_INIT;
uint8_t press_read_cnt = PRESS_READ_CNT;//do a pressure reading each press_read_cnt*currentstep steps 

static volatile bool ready_flag;            // A flag indicating PWM status.
static volatile bool ready_flag2;            // A flag indicating PWM status.



APP_PWM_INSTANCE(PWM1,1);                   // Create the instance "PWM1" using TIMER1 for motor PINA 
APP_PWM_INSTANCE(PWM2,2);                   // Create the instance "PWM2" using TIMER2 for motor PINB

const nrf_drv_timer_t MTR_PWM_TIMER = NRF_DRV_TIMER_INSTANCE(3); //Use timer3 instance for the Motor PWM durty-cycle
//const nrf_drv_timer_t MTR_STEP_TIMER = NRF_DRV_TIMER_INSTANCE(4); //Use timer3 instance for the Motor PWM durty-cycle

extern uint32_t mtr_pwm_timer_val = MTR_PWM_TIMER_VALUE;
extern uint32_t mtr_seal_check_threshold = PRESS_CHANGE_STEP;
extern uint32_t sweep_dir = SWEEP_DIRECTION;
extern uint32_t mtr_step_timer_val = MTR_STEP_TIMER_VALUE;
extern bool thresh1;
extern bool thresh2;
extern bool pressureThresholdCheck;
extern bool pcheck;
//void seal_check_init(void);

void seal_check(void);

void seal_check1(void);

void pressure_range_check(void);

void mtr_move(uint32_t step_num, uint8_t mtr_dir1);

void mtr_pwm_timer_event_handler(nrf_timer_event_t event_type, void* p_context);

//void mtr_step_timer_event_handler(nrf_timer_event_t event_type, void* p_context);

void mtr_pwm_timer_init(void);

//void mtr_step_timer_init(void);
void mtr_move_to_init(void);

void mtr_pwm_duty_set(uint32_t ocra, uint32_t ocrb, uint8_t mtr_sine_quarter, uint8_t mtr_dir);

void mtr_start(void);

void mtr_stop(void);

void finish_move_to_init(void);

void pwm_ready_callback(uint32_t pwm_id);    // PWM callback function
void pwm_ready_callback2(uint32_t pwm_id);

//motor PWM initialize
void mtr_pwm_init(void);

void incMtrStepCounter(void) ;
void decMtrStepCounter(void) ;
void move_to_init_logic_slow(void);
void move_to_init_logic_fast(void);
void mtr_move_logic(void);
void microstep_move(void);
uint32_t convert8to32(uint8_t* a);

/** @} */




#endif /* MOTOR_H_ */