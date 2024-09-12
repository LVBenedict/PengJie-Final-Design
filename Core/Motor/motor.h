#include "stm32f1xx_hal.h"
#include "tim.h"

// 停小车
void Motor_Off(void);

// 控制小车轮子转速
void Set_Speed(int side, int duty);

// 灰度传感器控制小车PID
void Gray_Sensor_Ctrl_PID(void);

// 限幅函数
void XianFu(void);
