#ifndef __XUEYA_H
#define __XUEYA_H

#include "main.h"
#include <stdint.h>
#include <math.h>

#define START_PRESSURE 155.0f
#define STOP_PRESSURE 60.0f

#define SAMPLE_DELAY_MS 5
#define MAX_SAMPLE 900


extern uint16_t Heartbeat;
extern uint8_t High_Pressure;
extern uint8_t Low_Pressure;

typedef enum
{ MY_RESET = 0,
  MY_SET
}MY_State;





void Pressure(MY_State State);
void Discharge(MY_State State);
float Get_mmhg(void);


void BloodPressure_Measure(void);
void BloodPressure_DebugMeasure(void);


uint16_t Get_Origin(void);
float ADC_To_Pa(uint16_t input);
float ADC_To_mmHg(uint16_t input);


#endif
