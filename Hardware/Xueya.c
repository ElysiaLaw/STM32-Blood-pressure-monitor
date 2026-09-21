#include "Xueya.h"


uint16_t Heartbeat = 0;
uint8_t High_Pressure = 0;
uint8_t Low_Pressure = 0;

float PressureBuf[MAX_SAMPLE];
float PulseBuf[MAX_SAMPLE];
uint32_t TimeBuf[MAX_SAMPLE];


void Pressure(MY_State State)
{
	
}

void Discharge(MY_State State)
{
	
}


float ADC_To_Pa(uint16_t input)
{
    if(input <= 250)
        return 0;

    return (((float)input) - 250) * 1557 / 100;
}

float ADC_To_mmHg(uint16_t input)
{
    if(input <= 250)
        return 0;

    return (((float)input) - 250) * 117 / 1000;
}


float Get_mmhg(void)
{
	return ADC_To_mmHg(AD_Value[0]);
}

uint16_t Get_Origin(void)
{
	return AD_Value[0];
}


void BloodPressure_DebugMeasure(void)
{
    uint16_t i;
    uint16_t index = 0;

    float pressure;
    float last_pressure;

    float pulse;

    float amp;
    float max_amp = 0;
    float map_pressure = 0;

    float sys_target;
    float dia_target;

    uint32_t first_beat = 0;
    uint32_t last_beat = 0;

    uint16_t beat_count = 0;

    /* 清零 */
    Heartbeat = 0;
    High_Pressure = 0;
    Low_Pressure = 0;

    /* 等待压力达到开始检测值 */
    while(Get_mmhg() < START_PRESSURE);

    last_pressure = Get_mmhg();

    /* 数据采集 */
    while(1)
    {
        pressure = Get_mmhg();

        /* 提取脉搏信号 */
        pulse = pressure - last_pressure;
        last_pressure = pressure;
		
		
		
		OLED_Clear();
		OLED_Printfpl(0,0,OLED_8X16,"Start",Get_mmhg());
		OLED_Printfpl(0,16,OLED_8X16,"%.2f",Get_mmhg());
		OLED_Update();
		
		
		

        /* 简单低通滤波 */
        pulse = pulse * 0.6f + PulseBuf[index > 0 ? index-1 : 0] * 0.4f;

        if(index < MAX_SAMPLE)
        {
            PressureBuf[index] = pressure;
            PulseBuf[index] = pulse;
            TimeBuf[index] = RTC_GetCounter();
            index++;
        }

        if(pressure < STOP_PRESSURE)
            break;

        Delay_ms(SAMPLE_DELAY_MS);
    }

    if(index < 50)
        return;

    /* 计算振幅包络 */
    for(i = 3; i < index-3; i++)
    {
        amp = PulseBuf[i];

        if(amp < 0)
            amp = -amp;

        if(amp > max_amp)
        {
            max_amp = amp;
            map_pressure = PressureBuf[i];
        }
    }

    if(max_amp < 0.2f)
        return;

    /* 示波法比例 */
    sys_target = max_amp * 0.55f;
    dia_target = max_amp * 0.82f;

    /* 计算收缩压 */
    for(i = 5; i < index; i++)
    {
        amp = PulseBuf[i];
        if(amp < 0) amp = -amp;

        if(amp >= sys_target)
        {
            High_Pressure = (uint8_t)PressureBuf[i];
            break;
        }
    }

    /* 计算舒张压 */
    for(i = index-5; i > 5; i--)
    {
        amp = PulseBuf[i];
        if(amp < 0) amp = -amp;

        if(amp >= dia_target)
        {
            Low_Pressure = (uint8_t)PressureBuf[i];
            break;
        }
    }

    /* 心率检测（只在MAP附近检测） */

    float map_min = map_pressure - 15;
    float map_max = map_pressure + 15;

    for(i = 4; i < index-4; i++)
    {
        if(PressureBuf[i] < map_min || PressureBuf[i] > map_max)
            continue;

        if(PulseBuf[i] > PulseBuf[i-1] &&
           PulseBuf[i] > PulseBuf[i+1] &&
           PulseBuf[i] > max_amp * 0.35f)
        {
            if(beat_count == 0)
                first_beat = TimeBuf[i];

            last_beat = TimeBuf[i];
            beat_count++;
        }
    }

    if(beat_count >= 2)
    {
        uint32_t dt = last_beat - first_beat;

        if(dt > 0)
        {
            Heartbeat = (beat_count - 1) * 60 / dt;
        }
    }
}