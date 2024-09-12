#include "gray_sensor.h"
#include "main.h"

#define GW_GRAY_SERIAL_DELAY_VALUE 27

volatile int8_t error; // 鐏板害浼犳劅鍣ㄤ紶鍥炴潵鐨別rror
volatile uint8_t a;
extern uint8_t stop_flag; // 鍋滆溅鏍囧織浣�
extern uint8_t sensor[8]; // main.c涓畾涔夌殑鐏板害浼犳劅鍣ㄥ��


/* 8MHz 5us澶ф鏄�27, 64MHz 5us澶ф鏄�270 */
void gray_delay(uint32_t delay_count)
{
	for (int i = 0; i < delay_count; ++i) {
		__NOP();
	}
}

/**** 璇诲彇鐏板害浼犳劅鍣ㄦ暟鎹� ****/
uint8_t gray_serial_read(void)
{
    uint8_t ret_8 = 0;
    for(int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_CLK_Pin, GPIO_PIN_RESET);//鏃堕挓涓嬮檷娌胯鏁�
        gray_delay(GW_GRAY_SERIAL_DELAY_VALUE);//寤舵椂5us
        ret_8 |= HAL_GPIO_ReadPin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_DAT_Pin) << i;
        HAL_GPIO_WritePin(GRAY_SERIAL_CLK_GPIO_Port, GRAY_SERIAL_CLK_Pin, GPIO_PIN_SET);//杈撳嚭楂樼數骞宠浼犳劅鍣ㄦ洿鏂版暟鎹�
        gray_delay(GW_GRAY_SERIAL_DELAY_VALUE);//寤舵椂5us
    }
    return ret_8;
}

/**** 浠庝竴涓彉閲忓垎绂诲嚭鎵�鏈夌殑bit ****/
void extractSensorData(uint8_t sensor_data, uint8_t sensor[8]) 
{
    for (int i = 0; i < 8; i++) {
        sensor[i] = (sensor_data >> i) & 1;
    }
}

/**** 浠巗ensor閲屽緱鍒癳rror鍜宎鐨勫�� ****/
void GetErrorandA(void)
{
    /********************************************* 宸＄嚎 ********************************************/
    if(sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 濡傛灉涓㈢嚎
        stop_flag = 1;
    } 
    if ((sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==0 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1) 
     || (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==0 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)) 
    {// 绾垮湪涓績锛屽皬杞︽病鍋�
        error = 0; a = 0;
    }
    /********************************************* 鍋忓彸杈� ******************************************/
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==0 && sensor[3]==0 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害灏�
        error = -4; a = 1;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==0 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害杈冨皬
        error = -5; a = 2;
    }
    else if (sensor[0]==1 && sensor[1]==0 && sensor[2]==0 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害閫備腑
        error = -7; a = 3;
    }
    else if (sensor[0]==1 && sensor[1]==0 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害杈冨ぇ
        error = -9; a = 4;
    }
    else if (sensor[0]==0 && sensor[1]==0 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害澶�
        error = -11; a = 5;
    }
    else if (sensor[0]==0 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓彸绋嬪害鍓х儓
        error = -13; a = 6;
    }

    /********************************************* 鍋忓乏杈� ******************************************/
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==0 && sensor[5]==0 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓乏绋嬪害灏�
        error = 4; a = 1;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==0 && sensor[6]==1 && sensor[7]==1)
    {// 灏忚溅鍋忓乏绋嬪害杈冨皬
        error = 5; a = 2;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==0 && sensor[6]==0 && sensor[7]==1)
    {// 灏忚溅鍋忓乏绋嬪害閫備腑
        error = 7; a = 3;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==0 && sensor[7]==1)
    {// 灏忚溅鍋忓乏绋嬪害杈冨ぇ
        error = 9; a = 4;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==0 && sensor[7]==0)
    {// 灏忚溅鍋忓乏绋嬪害澶�
        error = 11; a = 5;
    }
    else if (sensor[0]==1 && sensor[1]==1 && sensor[2]==1 && sensor[3]==1 && sensor[4]==1 && sensor[5]==1 && sensor[6]==1 && sensor[7]==0)
    {// 灏忚溅鍋忓乏绋嬪害鍓х儓
        error = 13; a = 6;
    }

}

/**** 鑾峰彇鐏板害浼犳劅鍣ㄤ腑闆剁殑鏁伴噺 ****/
uint8_t NumofZero(void)
{
    uint8_t i = 0, count = 0;
    for(; i<8; ++i)
    {// 閬嶅巻sensor鏁扮粍锛屾煡璇㈤浂鐨勬暟閲忥紝鍙垽鏂槸鍚﹀埌寮亾鍜屽仠杞�
        if (sensor[i] == 0) {
            count++;
        }
    }
    return count;
}
