#include "gray_sensor.h"
#include "main.h"

#define GW_GRAY_SERIAL_DELAY_VALUE 270

int8_t error; // 判断小车中心与线中心的差值error
uint8_t a; // 小车加速度（暂没使用）
extern uint8_t sensor[8]; // main.c中定义的传感器数据


/* 64MHz 5us的延迟时间差不多为270 */
void gray_delay(uint32_t delay_count)
{
	for (int i = 0; i < delay_count; ++i) {
		__NOP();
	}
}

/**** 读取灰度传感器数据 ****/
uint8_t gray_serial_read(void)
{
    uint8_t ret_8 = 0;
    for(int i = 0; i < 8; i++)
    {
			HAL_GPIO_WritePin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_CLK_Pin, GPIO_PIN_RESET);// 等待传感器上传数据
        gray_delay(GW_GRAY_SERIAL_DELAY_VALUE);//延时5us
        ret_8 |= HAL_GPIO_ReadPin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_DAT_Pin) << i;
        HAL_GPIO_WritePin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_CLK_Pin, GPIO_PIN_SET);//让传感器更新数据
        gray_delay(GW_GRAY_SERIAL_DELAY_VALUE);//延时5us
    }
    return ret_8;
}

/**** 将一个字节数据转换为8个bit ****/
void extractSensorData(uint8_t sensor_data, uint8_t sensor[8]) 
{
    for (int i = 0; i < 8; i++) {
        sensor[i] = (sensor_data >> i) & 1;
    }
}

/**** 根据sensor的值得到error和a ****/
void GetErrorandA(void)
{
    /********************************************* 正中 ********************************************/
    if(sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
			error = 0; a = 0;
    } 
    if ((sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==0 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1) 
     || (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==0 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)) 
    {// 正中间
        error = 0; a = 0;
    }
    /********************************************* 偏右 ******************************************/
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==0 && sensor[3]==0 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 车偏右
        error = -4; a = 1;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==0 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
        error = -5; a = 2;
    }
    else if (sensor[0]==1 && sensor[1]==0 && sensor[2]==0 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
        error = -7; a = 3;
    }
    else if (sensor[0]==1 && sensor[1]==0 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
        error = -9; a = 4;
    }
    else if (sensor[0]==0 && sensor[1]==0 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
        error = -11; a = 5;
    }
    else if (sensor[0]==0 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {
        error = -15; a = 6;
    }

    /********************************************* 车偏左 ******************************************/
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==0 && sensor[5]==0 && sensor[6]==1 && sensor[7]==1)
    {
        error = 4; a = 1;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==0 && sensor[6]==1 && sensor[7]==1)
    {
        error = 5; a = 2;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==0 && sensor[6]==0 && sensor[7]==1)
    {
        error = 7; a = 3;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==0 && sensor[7]==1)
    {
        error = 9; a = 4;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==0 && sensor[7]==0)
    {
        error = 11; a = 5;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==0)
    {
        error = 15; a = 6;
    }

}

/**** 读取sensor中0的数量 ****/
uint8_t NumofZero(void)
{
    uint8_t i = 0, count = 0;
    for(; i<8; ++i)
    {
        if (sensor[i] == 0) {
            count++;
        }
    }
    return count;
}
