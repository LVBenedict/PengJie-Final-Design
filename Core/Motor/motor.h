#include "stm32f1xx_hal.h"
#include "tim.h"

// ͣС��
void Motor_Off(void);

// ����С������ת��
void Set_Speed(int side, int duty);

// �Ҷȴ���������С��PID
void Gray_Sensor_Ctrl_PID(void);

// �޷�����
void XianFu(void);
