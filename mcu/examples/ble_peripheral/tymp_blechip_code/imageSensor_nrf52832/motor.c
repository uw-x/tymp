/*
 * MC6470Regs.h
 *
 *  Created on: Nov 16, 2019
 *      Author: Vikram 
 */
#include "motor.h"
#include "MTR_TIMER.h"

bool move_dir;

void rt8092_init(void){
    bank_left = 0x81;
    bank_right = 0x81;
    bank_run = 0x81;
    v_left = 0xA0;
    v_right = 0xA0;
    v_run = 0x90;

    hm_i2c_write_8b(0x12,0x00, 0x1C);
    nrf_delay_ms(1);
    //hm_i2c_write_8b(0x1C,0x81, 0x1C);
    hm_i2c_write_8b(0x1C,0x80, 0x1C);
    nrf_delay_ms(1);
    hm_i2c_write_8b(0x10,0x80, 0x1C);
    nrf_delay_ms(1);
    step_forward_activated = false;

    /*// Print regs to debug
    uint8_t regs[6] = {0x11, 0x12, 0x16, 0x1D};
    uint8_t test = 0;
    for(int i=0; i<6; i++){
       test = hm_i2c_read_8b(regs[i], 0x1C);
       printf("%x: ", regs[i]);
       printf("%x\n", test);
    }*/
    //for(int i=0; i<100; i++)
    //   step_forward();
    //stop_robot();

}

void motor_start(bool side){
    if(side){
      nrf_gpio_pin_clear(RIGHT_EN);
      hm_i2c_write_8b(0x1C,0x82, 0x1C);
      hm_i2c_write_8b(0x10,0x8F, 0x1C);
      nrf_gpio_pin_set(RIGHT_EN);
      nrf_delay_ms(20);
      nrf_gpio_pin_clear(RIGHT_EN);
      hm_i2c_write_8b(0x1C,bank_right, 0x1C);
      hm_i2c_write_8b(0x10,v_right,0x1C);
      nrf_gpio_pin_set(RIGHT_EN);
    } else {
      nrf_gpio_pin_clear(LEFT_EN);
      hm_i2c_write_8b(0x1C,0x82, 0x1C);
      hm_i2c_write_8b(0x10,0x8F, 0x1C);
      nrf_gpio_pin_set(LEFT_EN);
      nrf_delay_ms(20);
      nrf_gpio_pin_clear(LEFT_EN);
      hm_i2c_write_8b(0x1C,bank_left, 0x1C);
      hm_i2c_write_8b(0x10,v_left, 0x1C);
      nrf_gpio_pin_set(LEFT_EN);
    }
}

void motor_stop(bool side){
    if(side){
      nrf_gpio_pin_clear(RIGHT_EN);
    } else {
      nrf_gpio_pin_clear(LEFT_EN);
    }
}

void step_forward(void){
    motor_timer_init(motor_timer_val_right);
    nrf_drv_timer_enable(&TIMER_MOTOR);
    move_dir = false;
    step_forward_activated = true;

}
//Voltage bank = 80
//CF = 7.8 mA
//DF = 8.8 mA
//EF = 9.3 mA
//FF = 10 mA
void run_both_motors(void){
//    step_forward_activated = true;
    nrf_gpio_pin_clear(LEFT_EN);
    nrf_gpio_pin_clear(RIGHT_EN);
    hm_i2c_write_8b(0x1C,0x82, 0x1C);
    hm_i2c_write_8b(0x10,0x8F, 0x1C);
    nrf_gpio_pin_set(LEFT_EN);
    nrf_gpio_pin_set(RIGHT_EN);
    nrf_delay_ms(20);
    nrf_gpio_pin_clear(LEFT_EN);
    nrf_gpio_pin_clear(RIGHT_EN);
    hm_i2c_write_8b(0x1C,bank_run, 0x1C);
    hm_i2c_write_8b(0x10,v_run, 0x1C);
    nrf_gpio_pin_set(LEFT_EN);
    nrf_gpio_pin_set(RIGHT_EN);
}

void turn_right(void){
    if(step_forward_activated) {
        nrf_drv_timer_disable(&TIMER_MOTOR);
        nrf_drv_timer_uninit(&TIMER_MOTOR);
        motor_stop(LEFT);
        step_forward_activated = false;
    }
    motor_start(RIGHT);
}

void turn_left(void){
    if(step_forward_activated) {
        nrf_drv_timer_disable(&TIMER_MOTOR);
        nrf_drv_timer_uninit(&TIMER_MOTOR);
        motor_stop(RIGHT);
        step_forward_activated = false;
    }

    motor_start(LEFT);
}

void stop_robot(void){
    //TODO: add logic to check if timers are running, otherwise if you stop a left or right turn it crashes
    if(step_forward_activated) {
        nrf_drv_timer_disable(&TIMER_MOTOR);
        nrf_drv_timer_uninit(&TIMER_MOTOR);
        step_forward_activated = false;
    }
    motor_stop(LEFT);
    motor_stop(RIGHT);
}