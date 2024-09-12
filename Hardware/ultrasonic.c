#include "ultrasonic.h"
#include "oled.h"
#include "main.h"

uint32_t pMillis;
uint32_t start = 0;
uint32_t end = 0;
float distance_m = 0;
float distance_l = 0;
float distance_r = 0;
char string[15];

// 初始化定时器和Trig引脚
void HCSR04_Init(void)
{
	HAL_TIM_Base_Start(&htim2);
	HAL_GPIO_WritePin(Trig_Middle_GPIO_Port, Trig_Middle_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Trig_Left_GPIO_Port, Trig_Left_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Trig_Right_GPIO_Port, Trig_Right_Pin, GPIO_PIN_RESET);
}

// 读取中间超声波模块的值
void HCSR04_Read_m(void)
{
	HAL_GPIO_WritePin(Trig_Middle_GPIO_Port, Trig_Middle_Pin, GPIO_PIN_SET);//拉高TRIG_PIN，开始发射脉冲
	HAL_TIM_Base_Stop(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2, 0);//重置定时器
	HAL_TIM_Base_Start(&htim2);
	while (__HAL_TIM_GET_COUNTER(&htim2) < 10)//等待10us
		;
	HAL_GPIO_WritePin(Trig_Middle_GPIO_Port, Trig_Middle_Pin, GPIO_PIN_RESET);// 关闭超声波传感器的 TRIG_PIN 引脚，停止发射脉冲信号

	pMillis = HAL_GetTick();// 记录当前时间（以毫秒为单位）
	while (!(HAL_GPIO_ReadPin(Echo_Middle_GPIO_Port, Echo_Middle_Pin)) && pMillis + 10 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚开始接收到回波信号，或者超过 10 毫秒的时间
		;
	start = __HAL_TIM_GET_COUNTER(&htim2);//记录 ECHO_PIN 开始接收到回波信号时的计数器值

	pMillis = HAL_GetTick();// 继续记录当前时间（以毫秒为单位）
	while ((HAL_GPIO_ReadPin(Echo_Middle_GPIO_Port, Echo_Middle_Pin)) && pMillis + 50 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚停止接收到回波信号，或者超过 50 毫秒的时间
		;
	end = __HAL_TIM_GET_COUNTER(&htim2);// 记录 ECHO_PIN 停止接收到回波信号时的计数器值

	distance_m = (end - start) * 0.034 / 2;// 计算单程距离值（单位：厘米），声波的传播速度约为 0.034 厘米/微秒
}

void HCSR04_Read_l(void)
{
	HAL_GPIO_WritePin(Trig_Left_GPIO_Port, Trig_Left_Pin, GPIO_PIN_SET);//拉高TRIG_PIN，开始发射脉冲
	HAL_TIM_Base_Stop(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2, 0);//重置定时器
	HAL_TIM_Base_Start(&htim2);
	while (__HAL_TIM_GET_COUNTER(&htim2) < 10)//等待10us
		;
	HAL_GPIO_WritePin(Trig_Left_GPIO_Port, Trig_Left_Pin, GPIO_PIN_RESET);// 关闭超声波传感器的 TRIG_PIN 引脚，停止发射脉冲信号

	pMillis = HAL_GetTick();// 记录当前时间（以毫秒为单位）
	while (!(HAL_GPIO_ReadPin(Echo_Left_GPIO_Port, Echo_Left_Pin)) && pMillis + 10 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚开始接收到回波信号，或者超过 10 毫秒的时间
		;
	start = __HAL_TIM_GET_COUNTER(&htim2);//记录 ECHO_PIN 开始接收到回波信号时的计数器值

	pMillis = HAL_GetTick();// 继续记录当前时间（以毫秒为单位）
	while ((HAL_GPIO_ReadPin(Echo_Left_GPIO_Port, Echo_Left_Pin)) && pMillis + 50 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚停止接收到回波信号，或者超过 50 毫秒的时间
		;
	end = __HAL_TIM_GET_COUNTER(&htim2);// 记录 ECHO_PIN 停止接收到回波信号时的计数器值

	distance_l = (end - start) * 0.034 / 2;// 计算单程距离值（单位：厘米），声波的传播速度约为 0.034 厘米/微秒
}

void HCSR04_Read_r(void)
{
	HAL_GPIO_WritePin(Trig_Right_GPIO_Port, Trig_Right_Pin, GPIO_PIN_SET);//拉高TRIG_PIN，开始发射脉冲
	HAL_TIM_Base_Stop(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2, 0);//重置定时器
	HAL_TIM_Base_Start(&htim2);
	while (__HAL_TIM_GET_COUNTER(&htim2) < 10)//等待10us
		;
	HAL_GPIO_WritePin(Trig_Right_GPIO_Port, Trig_Right_Pin, GPIO_PIN_RESET);// 关闭超声波传感器的 TRIG_PIN 引脚，停止发射脉冲信号

	pMillis = HAL_GetTick();// 记录当前时间（以毫秒为单位）
	while (!(HAL_GPIO_ReadPin(Echo_Right_GPIO_Port, Echo_Right_Pin)) && pMillis + 10 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚开始接收到回波信号，或者超过 10 毫秒的时间
		;
	start = __HAL_TIM_GET_COUNTER(&htim2);//记录 ECHO_PIN 开始接收到回波信号时的计数器值

	pMillis = HAL_GetTick();// 继续记录当前时间（以毫秒为单位）
	while ((HAL_GPIO_ReadPin(Echo_Right_GPIO_Port, Echo_Right_Pin)) && pMillis + 50 > HAL_GetTick())// 等待直到 ECHO_PIN 引脚停止接收到回波信号，或者超过 50 毫秒的时间
		;
	end = __HAL_TIM_GET_COUNTER(&htim2);// 记录 ECHO_PIN 停止接收到回波信号时的计数器值

	distance_r = (end - start) * 0.034 / 2;// 计算单程距离值（单位：厘米），声波的传播速度约为 0.034 厘米/微秒	
}


// 将值显示到OLED屏幕上
void HCSR04_OLEDShowResult(void)
{
	OLED_ShowString(2, 3, "DISTANCE: cm");
	sprintf(string, "%.1f ", distance_l); // 使用sprintf函数将距离（distance）的值转换为字符串格式，并保留一位小数
	OLED_ShowString(3, 1, "               ");// 第三行清空
	OLED_ShowString(3, 1, string);
	sprintf(string, "%.1f ", distance_m); // 使用sprintf函数将距离（distance）的值转换为字符串格式，并保留一位小数
	OLED_ShowString(3, 6, string);
	sprintf(string, "%.1f ", distance_r); // 使用sprintf函数将距离（distance）的值转换为字符串格式，并保留一位小数
	OLED_ShowString(3, 11, string);
	HAL_Delay(50 * 2);
}

// 测量的数据并不总是准确，这里提供一个可供参考的解决方法
// 将一定时间内采集到的数据，先存起来，再做一个排序，去除最大值和最小值，也可以多去除几个极端值，然后取平均

// 会影响到测量结果的因素有很多：气温、气压、湿度等会影响声音在大气中传播的速度(distance = v * t)

// 此外由于驱动函数是直接操作GPIO口进行翻转，再延时等待以完成通信协议，而这一系列操作都会影响到接受数据的解析
//更为精准的一种方式应该是输入捕获采集脉冲变化，若还要更精准，可以使用不同的通道分别采集上升和下降沿，这样可以减小一些误差
// 还有就是模块和被测物体的相对位置，若偏了或者其他原因造成声波的反射都会影响到正确的测量结果
//0.034 = 340 * 10^2 / 10^6
