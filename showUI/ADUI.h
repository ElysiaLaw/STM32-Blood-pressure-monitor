#ifndef __ADUI_H
#define __ADUI_H

#include "main.h"



extern uint8_t ADUI_state;

void ADUI_Init(void);
void ADUI_In(void);
void ADUI_Update(void);
void ADSHOWUI_In(void);
void ADSHOWUI_Update(void);
void ADSHOWUI_Showx(uint16_t y ,uint16_t num);



#endif
