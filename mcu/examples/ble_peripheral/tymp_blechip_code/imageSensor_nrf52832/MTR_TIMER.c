 /** 
 *  MTR_TIMER.h
 *
 *  Created: Nov 18, 2019
 *      Author: Ali Najafi
 */
#include "MTR_TIMER.h"
//bool move_dir;

void timer_motor_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    nrf_drv_timer_disable(&TIMER_MOTOR);
    if(!cmd_motor_stream){
        if(move_dir) {
            motor_stop(LEFT);
            move_dir = false;
            //nrf_delay_ms(40);
            nrf_drv_timer_extended_compare(
             &TIMER_MOTOR, NRF_TIMER_CC_CHANNEL3, motor_timer_val_right, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
             nrf_drv_timer_enable(&TIMER_MOTOR);
            motor_start(RIGHT);
        } else {
            motor_stop(RIGHT);
            move_dir = true;
            //nrf_delay_ms(40);
            nrf_drv_timer_extended_compare(
             &TIMER_MOTOR, NRF_TIMER_CC_CHANNEL3, motor_timer_val_left, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
             nrf_drv_timer_enable(&TIMER_MOTOR);
            motor_start(LEFT);
        }
    } else{
        stop_robot();
    }
    

}


void motor_timer_init(uint32_t motor_timer_val)
{
    uint32_t err_code = NRF_SUCCESS;
    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = MOTOR_TIMER_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_MOTOR, &timer_cfg, timer_motor_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_extended_compare(
         &TIMER_MOTOR, NRF_TIMER_CC_CHANNEL3, motor_timer_val, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
}
