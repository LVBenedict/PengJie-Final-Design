#ifndef GRAY_SENSOR_H
#define GRAY_SENSOR_H

#include "stm32f1xx_hal.h"

// 延迟
void gray_delay(uint32_t delay_count);
/**** 读取灰度传感器数据 ****/
uint8_t gray_serial_read();
// sensor中0的数量
uint8_t NumofZero(void);
// 获取pid所需要的error和a
void GetErrorandA(void);

// 将sensor的值分离为8个bit
void extractSensorData(uint8_t sensor_data, uint8_t sensor[8]);


#endif
