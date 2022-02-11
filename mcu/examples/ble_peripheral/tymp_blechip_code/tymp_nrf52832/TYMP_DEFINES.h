/*
 * TYMP_FUNC.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Ali Najafi
 */

#ifndef TYMP_DEFINES_H_
#define TYMP_DEFINES_H_

#define KEEP_NUM_VAL 100*32

#define INIT_GO 0

#define PRESS_READ_CNT 1

#define PRESSURE_READ_DELAY 6

#define MTR_STEP_TIMER_VALUE 30000
#define MTR_PWM_TIMER_VALUE 50

#define MAX_STEP_TIMES 1000

// used for short cable and 3ml syringe
#define PRESS_CHANGE_STEP 10000 //This number should calculated based on the total pressure change that we need and the duration of the tympanometry test and duration of each motor step

// used for long cable and 5ml syringe
// #define PRESS_CHANGE_STEP 10000 //This number should calculated based on the total pressure change that we need and the duration of the tympanometry test and duration of each motor step

#define PRESS_CHANGE_STEP_MV_INIT_HUMAN 50
#define PRESS_CHANGE_STEP_MV_INIT 5000
#define SWEEP_DIRECTION 0
#define SEAL_PRESS_CNT 4
#define DIR_JUST_CHANGED_VAL 0//DIR_JUST_CHANGED_VAL should be higher than SEAL_PRESS_CNT
#define INIT_PRESS_NOT_CARE 0

#define SEAL_TRUE_INFO 3//We send something similar to image info with just one byte to indicate that we got the seal
#define SEAL_FALSE_INFO 4//We send something similar to image info with just one byte to indicate that the seal is broken

#define MAX_PRESS_REACH 5
#define MIN_PRESS_REACH 6
#define INIT_PRESS_REACH 7
#define INIT_PRESS_MID 8

#define INIT_PRESS_SEND 9

#define GONE_BACK_TO_INIT 10
#define SEND_START_ACK 11
#define SEND_TURN_INFO 12

#define SEAL_LOSE_PRESSURE_THRESHOLD 5000

#define CURRENT_STEP_INIT 1
#define MTR_FAST_SPEED 60
#endif /* TYMP_DEFINES_H_ */