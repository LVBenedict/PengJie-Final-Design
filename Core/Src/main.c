/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "gray_sensor.h"
#include "motor.h"
#include "ultrasonic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// 灰度传感器全局变量
uint8_t sensor_data = 0;
uint8_t sensor[8] = {0};
extern int8_t error;

// 电机全局变量
int Duty_L = 0, Duty_R = 0;
int BaseSpeed = 20;
int MaxSpeed = 30;

// 超声波测距全局变量
extern float distance_m;
extern float distance_l;
extern float distance_r;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  OLED_Init();// OLED初始化
	HCSR04_Init();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);// 小车左轮PWM通道
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);// 小车右轮PWM通道

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  OLED_ShowString(1, 3, "Final Design");

  while (1)
  {
		if(HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == 0)
		{// 如果按键1被按下: 进入灰度传感器巡线模式
			HAL_Delay(10);// 消抖
			if(HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == 0)
			{
				//HAL_TIM_Base_Start_IT(&htim2);// 灰度传感器定时器2中断
				OLED_ShowString(2, 1, "Gray Sensor Mode");
				OLED_ShowString(3, 1, "NUM:");
				while(HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin))
				{// 只要按键4没有被按下，就一直巡线
					sensor_data = gray_serial_read();// 读一次灰度传感器值
					extractSensorData(sensor_data, sensor); //分离变量
					GetErrorandA();
					Gray_Sensor_Ctrl_PID(); //灰度传感器巡线
					OLED_ShowNum(3, 5, sensor_data, 3);
					OLED_ShowBinNum(3, 9, sensor_data, 8);
				}
				OLED_Clear();
				OLED_ShowString(1, 3, "Final Design");
				Motor_Off();
			}
		}
		if(HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == 0)
		{// 如果按键2被按下,超声波避障模式
			HAL_Delay(10);// 消抖
			if(HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == 0)
			{
				OLED_Clear();
				OLED_ShowString(1, 1, "Ultrasonic Mode");
				Set_Speed(1, 20);
				Set_Speed(2, 20);
				while(HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin))
				{
					//HCSR04_Read_l();
					HCSR04_Read_m();
					//HCSR04_Read_r();
					HCSR04_OLEDShowResult();
					if(distance_m < 15)
					{// 如果距离突然小于15
						Set_Speed(1, -15);
						Set_Speed(2, -15);
						HAL_Delay(600);  // 延迟2s钟
						Motor_Off();
						HCSR04_Read_l();
						HCSR04_Read_r();// 用超声波左右探探距离
						if(distance_l < distance_r)
						{// 如果左边距离更近，就往右拐
							Set_Speed(1, 20);
							Set_Speed(2, 10);
						}
						else
						{// 如果右边距离更近，就往左拐
							Set_Speed(2, 20);
							Set_Speed(1, 10);
						}
						HAL_Delay(1000);
						Set_Speed(1, 15);// 回归走直线
						Set_Speed(2, 15); 
					}
				}
				OLED_Clear();
				OLED_ShowString(1, 3, "Final Design");
				Motor_Off();
			}
		}
		if(HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin) == 0)
		{// 如果按键3被按下
			OLED_ShowString(3, 1, "button3 press");
		}
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
