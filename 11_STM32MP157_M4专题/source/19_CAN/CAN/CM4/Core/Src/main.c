/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "driver_fdcan.h"
#include "driver_key.h"
#include "driver_rs485.h"
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
uint16_t rs485_id = 0x485;
uint16_t fdcan_id = 0x584;

uint8_t rs485_tx_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t fdcan_tx_data[8] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x17, 0x28};

uint8_t rs485_rx_data[16] = {0};
uint8_t fdcan_rx_data[16] = {0};

uint8_t test_flag = 0;
uint8_t step = 0;
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
	uint8_t i = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  if(IS_ENGINEERING_BOOT_MODE())
  {
    /* Configure the system clock */
    SystemClock_Config();
  }

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN2_Init();
  MX_UART4_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_FDCAN_SetFilter();
  printf("CAN Send And Receive test\r\n");
  printf("\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(test_flag==1)
	  {
		  test_flag = 0;
		  // RS485 Send ID
		  // RS485->CAN
		  RS485_TX((uint8_t*)&rs485_id, 2);
		  printf("\r\n");
		  printf("Step 0: RS485 Send ID to CAN\r\n");
		  printf("RS485 Send ID 0x%x\r\n", rs485_id);

		  step = 1;
	  }

	  switch(step)
	  {
		  case 0:
		  {
			  break;
		  }
		  case 1:
		  {
			  // Wait for CAN receive finish
			  if(CAN_Receive() == 1)
			  {
				  CAN_SetReceiveFlag(0);
				  printf("\r\n");
				  printf("Step 1: CAN wait for Receive ID from RS485\r\n");
				  printf("CAN Receive Message: \r\n");
				  printf("\tID: 0x%x\r\n", can_rx_data.ID);
				  printf("\tLength: 0x%x\r\n", can_rx_data.length);
				  printf("\tData:");
				  for(i=0; i<can_rx_data.length; i++)
				  {
					  printf("\t0x%x", can_rx_data.RX_Data[i]);
				  }
				  printf("\r\n");

				  step = 2;
			  }

			  break;
		  }
		  case 2:
		  {
			  RS485_TX((uint8_t*)rs485_tx_data, 8);
			  printf("\r\n");
			  printf("Step 2: RS485 Send Data to CAN\r\n");
			  printf("RS485 Send Data: \r\n");
			  printf("\tData:");
			  for(i=0; i<8; i++)
			  {
				  printf("\t0x%x", rs485_tx_data[i]);
			  }
			  printf("\r\n");

			  step = 3;
		  }
		  case 3:
		  {
			  // Wait for CAN receive finish
			  if(CAN_Receive() == 1)
			  {
				  CAN_SetReceiveFlag(0);
				  printf("\r\n");
				  printf("Step 3: CAN wait for Receive Data from RS485\r\n");
				  printf("CAN Receive Message: \r\n");
				  printf("\tID: 0x%x\r\n", can_rx_data.ID);
				  printf("\tLength: 0x%x\r\n", can_rx_data.length);
				  printf("\tData:");
				  for(i=0; i<can_rx_data.length; i++)
				  {
					  printf("\t0x%x", can_rx_data.RX_Data[i]);
				  }
				  printf("\r\n");
				  step = 4;
			  }
			  break;
		  }
		  case 4:
		  {
			  // CAN->RS485
			  printf("\r\n");
			  printf("Step 4: CAN Send Frame Data to RS485, and RS485 start receive data from CAN\r\n");
			  CAN_Transmit(fdcan_id, fdcan_tx_data, 8);
			  RS485_RX(rs485_rx_data, 8);
			  step = 5;
			  break;
		  }
		  case 5:
		  {
			  if(RS485_GetRxFlag())
			  {
				  step = 0;
				  printf("\r\n");
				  printf("Step 5: RS485 Received data from CAN finished.\r\n");
				  printf("CAN Send Message:\r\n");
				  printf("\tID:0x%x\r\n", fdcan_id);
				  printf("\tLength:8\r\n");
				  printf("\tData:");
				  for(i=0; i<8; i++)
				  {
					  printf("\t0x%x", fdcan_tx_data[i]);
				  }
				  printf("\r\n");
				  printf("RS485 Receive Data:\r\n");
				  printf("\tData: ");
				  for(i=0; i<8; i++)
				  {
					  printf("\t0x%x", rs485_rx_data[i]);
				  }
				  printf("\r\n");
			  }
		  }
		  default:break;
  	  }/*End switch(step)*/
  }/* End while(1) */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSIDivValue = RCC_HSI_DIV1;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLL3SOURCE_HSE;
  RCC_OscInitStruct.PLL3.PLLM = 2;
  RCC_OscInitStruct.PLL3.PLLN = 52;
  RCC_OscInitStruct.PLL3.PLLP = 3;
  RCC_OscInitStruct.PLL3.PLLQ = 2;
  RCC_OscInitStruct.PLL3.PLLR = 2;
  RCC_OscInitStruct.PLL3.PLLRGE = RCC_PLL3IFRANGE_1;
  RCC_OscInitStruct.PLL3.PLLFRACV = 2048;
  RCC_OscInitStruct.PLL3.PLLMODE = RCC_PLL_FRACTIONAL;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** RCC Clock Config
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_ACLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3|RCC_CLOCKTYPE_PCLK4
                              |RCC_CLOCKTYPE_PCLK5;
  RCC_ClkInitStruct.AXISSInit.AXI_Clock = RCC_AXISSOURCE_HSI;
  RCC_ClkInitStruct.AXISSInit.AXI_Div = RCC_AXI_DIV1;
  RCC_ClkInitStruct.MCUInit.MCU_Clock = RCC_MCUSSOURCE_PLL3;
  RCC_ClkInitStruct.MCUInit.MCU_Div = RCC_MCU_DIV1;
  RCC_ClkInitStruct.APB4_Div = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5_Div = RCC_APB5_DIV1;
  RCC_ClkInitStruct.APB1_Div = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2_Div = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB3_Div = RCC_APB3_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Set the HSE division factor for RTC clock
  */
  __HAL_RCC_RTC_HSEDIV(1);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
