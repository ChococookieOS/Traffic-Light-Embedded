#include "gd32vf103.h"
#include <stdio.h>
#include "sipeedNano_trafficLight.h"

/* BUILTIN LED OF LONGAN BOARDS IS PIN PC13 */
#define GREEN_PIN GPIO_PIN_14
#define YELLOW_PIN GPIO_PIN_15
#define RED_PIN GPIO_PIN_8

#define INPUT_FRONT_PIN GPIO_PIN_13
#define INPUT_BACK_PIN GPIO_PIN_12



uint64_t currentSystemTimeMillis(){
    return get_timer_value() / (SystemCoreClock/4000.0);
}


void state_normal(uint32_t * state_startTime, uint32_t  systime){
    uint32_t delta = systime - *state_startTime;
    if(delta < 4000){
        turnRedOn();
        turnYellowOff();
    }
    else if(delta < 5000){
        turnYellowOn();
    }
    else if(delta < 9000){
        turnRedOff();
        turnYellowOff();
        turnGreenOn();
    }
    else if(delta < 10000){
        turnGreenOff(); 
        turnYellowOn();
    }
    else {
        *state_startTime = systime;
    }
}

void state_blinking(uint32_t * state_startTime, uint32_t systime){
    uint64_t delta = systime - *state_startTime;
    turnRedOff();
    turnGreenOff();
    if(delta < 500){
        turnYellowOn();
    }
    else if(delta < 1000){
         turnYellowOff();
    }
    else {
        *state_startTime = systime;
    }
    
}

void state_off(uint32_t * state_startTime, uint32_t  systime){
    turnRedOff();
    turnYellowOff();
    turnGreenOff(); 
}


void state(int i, uint32_t * state_startTime, uint32_t  systime){
    switch (i)
    {
    case 0:
        state_off(state_startTime, systime);
        break;
    case 1:
        state_blinking(state_startTime, systime);
        break;
    case 2:
        state_normal(state_startTime, systime);
         break;
    default:
        state_off(state_startTime, systime);
        break;
    }
}

int main(void) {
    /* State management */
    uint32_t state_start = 0;
    int currentState = 1;
    /* Button management */
    int wasPressedLastTick = 0;
    uint32_t pressStartTime = 0;
    /* SleepManagement */
    uint32_t off_start = 0;

    /* Pause management */
    uint32_t currentSysTime = 0;
    int pause = 0;

    /* enable the led clock */
    enableGPIOClocksAB();
    /*rcu_periph_clock_enable(RCU_GPIOA); */

    prepareLEDS();
    turnRedOff();
    turnYellowOff();
    turnGreenOff(); 
    prepareInput();

    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);


    while(1){

        if(gpio_input_bit_get(GPIOB,INPUT_FRONT_PIN)){
            if(!wasPressedLastTick){
                pressStartTime = currentSystemTimeMillis();
            }
            wasPressedLastTick = 1;
        }
        else if(gpio_input_bit_get(GPIOB, INPUT_BACK_PIN)){   
            /*Easter Egg */
            GPIO_BC(GPIOC) = GPIO_PIN_13;
        }
        else{
            GPIO_BOP(GPIOC) = GPIO_PIN_13;
            if(wasPressedLastTick){
                off_start = currentSystemTimeMillis();
                if(currentSystemTimeMillis() - pressStartTime > 2000){
                   /* Long press */
                    currentState = 1;
                    pause = 0;
                }
                else if (currentSystemTimeMillis() - pressStartTime > 120){
                   /*Short press */
                    if(currentState == 0){ 
                        currentState = 1;
                        state_start = currentSystemTimeMillis();
                        pause = 0;}
                    else if (currentState == 2) {
                        if(pause) {
                            state_start = currentSystemTimeMillis() - (currentSysTime - state_start);
                        } 
                        pause = !pause;
                    }
                    else {
                        currentState = 2;
                        state_start = currentSystemTimeMillis();
                        pause = 0;
                    }  
                }
            }
            else{
                if(currentState == 1 && ((currentSystemTimeMillis() - off_start) > 9900)){
                    currentState = 0;
                }
            }
            wasPressedLastTick = 0;   
        }
        if(!pause){
            currentSysTime = currentSystemTimeMillis();
        }
        state(currentState, &state_start, currentSysTime);  
    }
}