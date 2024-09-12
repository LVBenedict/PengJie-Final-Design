#include "motor.h"
#include "main.h"
#include "oled.h"

extern int Duty_L, Duty_R; //main.c�ж��壬����С��������ռ�ձ�
extern int8_t error; // gray_sensor.c�ж��壺PID�е�ƫ����
extern int BaseSpeed, MaxSpeed;// main.c�ж���Ļ���������ٶ�

/*** �Ҷȴ���������С��PID ****/
void Gray_Sensor_Ctrl_PID(void)
{
    float Position_KP_l = 1.2, Position_KI_l = 0, Position_KD_l = 10;
    float Position_KP_r = 1.2, Position_KI_r = 0, Position_KD_r = 10;
	static int Speed_chasu_l, Speed_chasu_r;
    static int8_t Integral_error, Last_error;
    Integral_error += error;
    Speed_chasu_l = Position_KP_l * error + Position_KI_l * Integral_error + Position_KD_l * (error - Last_error);
    Speed_chasu_r = Position_KP_r * error + Position_KI_r * Integral_error + Position_KD_r * (error - Last_error);
    Last_error = error;
    Duty_L = BaseSpeed + Speed_chasu_l;
    Duty_R = BaseSpeed - Speed_chasu_r;
    XianFu();
    OLED_ShowString(4, 4, "L:");
    OLED_ShowNum(4, 6, Duty_L, 2);
    OLED_ShowString(4, 10, "R:");
    OLED_ShowNum(4, 12, Duty_R, 2);
    Set_Speed(1, Duty_L);
    Set_Speed(2, Duty_R);
}


/**** �޷����� ****/
void XianFu(void)
{
    if (Duty_L > MaxSpeed)
        Duty_L = MaxSpeed;
    else if(Duty_L <= 0)
        Duty_L = 1;
    if (Duty_R > MaxSpeed)
        Duty_R = MaxSpeed;
    else if(Duty_R <= 0)
        Duty_R = 1;
}

/**** ͣС�� ****/
void Motor_Off(void)
{
    HAL_GPIO_WritePin(Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_AIN2_GPIO_Port, Motor_AIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor_BIN2_GPIO_Port, Motor_BIN2_Pin, GPIO_PIN_RESET);
}

/**** ����С������ת�� ****/
//side = 1:   ����
//side = 2:   ����
void Set_Speed(int side, int duty)
{
    if(side == 1)
    {//����
        if(duty < 0)
        {//��ת
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, -duty);
            HAL_GPIO_WritePin(Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Motor_AIN2_GPIO_Port, Motor_AIN2_Pin, GPIO_PIN_RESET);
        }
        else if(duty > 0)
        {//��ת
        	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty);
        	HAL_GPIO_WritePin(Motor_AIN1_GPIO_Port, Motor_AIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor_AIN2_GPIO_Port, Motor_AIN2_Pin, GPIO_PIN_SET);
        }
        else Motor_Off();
    }
    else
    {//����
    	if(duty < 0)
		{//��ת
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, -duty);
			HAL_GPIO_WritePin(Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Motor_BIN2_GPIO_Port, Motor_BIN2_Pin, GPIO_PIN_RESET);
		}
		else if(duty > 0)
		{//��ת
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, duty);
			HAL_GPIO_WritePin(Motor_BIN1_GPIO_Port, Motor_BIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Motor_BIN2_GPIO_Port, Motor_BIN2_Pin, GPIO_PIN_SET);
		}
        else Motor_Off();
    }
}
