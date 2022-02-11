
    mtr_on = false;
    debug_array_counter3=0;
    debug_array_counter4=0;

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
