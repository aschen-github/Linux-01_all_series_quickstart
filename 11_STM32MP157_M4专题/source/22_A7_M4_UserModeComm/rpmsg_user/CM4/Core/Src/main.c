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
#include "openamp.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "virt_uart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define MAX_BUFFER_SIZE RPMSG_BUFFER_SIZE
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
IPCC_HandleTypeDef hipcc;

UART_HandleTypeDef huart8;

/* USER CODE BEGIN PV */
VIRT_UART_HandleTypeDef huart0;
VIRT_UART_HandleTypeDef huart1;

__IO FlagStatus VirtUart0RxMsg = RESET;
uint8_t VirtUart0ChannelBuffRx[MAX_BUFFER_SIZE];
uint16_t VirtUart0ChannelRxSize = 0;

__IO FlagStatus VirtUart1RxMsg = RESET;
uint8_t VirtUart1ChannelBuffRx[MAX_BUFFER_SIZE];
uint16_t VirtUart1ChannelRxSize = 0;

uint8_t BuffTx[MAX_BUFFER_SIZE];

#define MSG_LED_ON "*led_on"
#define MSG_LED_OFF "*led_off"
#define MSG_STOP "*stop"
#define MSG_DELAY "*delay"
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IPCC_Init(void);
static void MX_UART8_Init(void);
int MX_OPENAMP_Init(int RPMsgRole, rpmsg_ns_bind_cb ns_bind_cb);
/* USER CODE BEGIN PFP */
void VIRT_UART0_RxCpltCallback(VIRT_UART_HandleTypeDef *huart);
void VIRT_UART1_RxCpltCallback(VIRT_UART_HandleTypeDef *huart);
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

  if(IS_ENGINEERING_BOOT_MODE())
  {
    /* Configure the system clock */
    SystemClock_Config();
  }

  /* IPCC initialisation */
   MX_IPCC_Init();
  /* OpenAmp initialisation ---------------------------------*/
  MX_OPENAMP_Init(RPMSG_REMOTE, NULL);

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */
  printf("RPMsg user mode test\r\n");
   if (VIRT_UART_Init(&huart0) != VIRT_UART_OK) {
 	  printf("VIRT_UART_Init UART0 failed.\r\n");
     Error_Handler();
   }
   if (VIRT_UART_Init(&huart1) != VIRT_UART_OK) {
 	  printf("VIRT_UART_Init UART1 failed.\r\n");
     Error_Handler();
   }
   if(VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART0_RxCpltCallback) != VIRT_UART_OK)
   {
    Error_Handler();
   }
   if(VIRT_UART_RegisterCallback(&huart1, VIRT_UART_RXCPLT_CB_ID, VIRT_UART1_RxCpltCallback) != VIRT_UART_OK)
   {
    Error_Handler();
   }
   EXTI_ConfigTypeDef EXTI_ConfigStructure;
   EXTI_HandleTypeDef hexti62;
   /*
    * Set configuration of Exti line 62 (IPCC interrupt CPU2). It could be used to wakeup the
    * M4 from CStop mode when RPMsg received from Cortex-A7
    */
   EXTI_ConfigStructure.Line = EXTI_LINE_62;
   EXTI_ConfigStructure.Mode = EXTI_MODE_C2_INTERRUPT;
   //PERIPH_LOCK(EXTI);
   HAL_EXTI_SetConfigLine(&hexti62, &EXTI_ConfigStructure);
   //PERIPH_UNLOCK(EXTI);
   /*
    *     Enable RCC_IT_WKUP to exit M4 from CStop mode.
    *     Indeed, due to SOC issue, M4 firmware shall make sure
    *     RCC_WAKEUP interrupt is the first one used to exit M4 from CStop mode.
    *     Therefore, M4 masks all NVIC interrupts with priority higher than 0
    *     before entering CStop mode and unmasks them when moving from WFI.
    *     (in HAL_PWR_EnterSTOPMode function)
    *     Note: All other NVIC interrupts shall be set to a different value
    *     from 0 to make sure that this workaround works well.
    */
    __HAL_RCC_ENABLE_IT(RCC_IT_WKUP);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  OPENAMP_check_for_message();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (VirtUart0RxMsg)
	{
	  VirtUart0RxMsg = RESET;
	  if (!strncmp((char *)VirtUart0ChannelBuffRx, MSG_LED_ON, strlen(MSG_LED_ON)))
	  {
		  strcpy((char *)BuffTx, "m4:led on\n");
		  printf("%s\r", BuffTx);
		  VIRT_UART_Transmit(&huart0, BuffTx, strlen((const char *)BuffTx));
		  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	  }
	  if (!strncmp((char *)VirtUart0ChannelBuffRx, MSG_LED_OFF, strlen(MSG_LED_OFF)))
	  {
		  strcpy((char *)BuffTx, "m4:led off\n");
		  printf("%s\r", BuffTx);
		  VIRT_UART_Transmit(&huart0, BuffTx, strlen((const char *)BuffTx));
		  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
	  }
	  memset(VirtUart0ChannelBuffRx, 0 ,VirtUart0ChannelRxSize);
	  memset(BuffTx, 0 ,strlen((const char *)BuffTx));
	}
	if (VirtUart1RxMsg)
	{
	  VirtUart1RxMsg = RESET;
	  if (!strncmp((char *)VirtUart1ChannelBuffRx, MSG_STOP, strlen(MSG_STOP)))
	  {
		  strcpy((char *)BuffTx, "m4:stop\n");
		  printf("%s\r", BuffTx);
		  VIRT_UART_Transmit(&huart1, BuffTx, strlen((const char *)BuffTx));
		  //RCC_backupClocks();
		  /* Clear the MCU flags before going into CSTOP */
		  SET_BIT(PWR->MCUCR, PWR_MCUCR_CSSF);
		  printf("Going to CStop mode\r\n");
		  /* (C)STOP protection mechanism
		   * Only the IT with the highest priority (0 value) can interrupt.
		   * RCC_WAKEUP_IRQn IT is intended to have the highest priority and to be the
		   * only one IT having this value
		   * RCC_WAKEUP_IRQn is generated only when RCC is completely resumed from
		   * CSTOP */
		   __set_BASEPRI(1 << (8 - __NVIC_PRIO_BITS));
		   HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
		   /* To allow Systick to increment after CSTOP (Eg.: to not block during
			* TIMEOUT routines), TICK_INT_PRIORITY < BASEPRI
			* For this example as TICK_INT_PRIORITY = 1, BASEPRI should be 2  */
		   __set_BASEPRI(2 << (8 - __NVIC_PRIO_BITS));
		   printf("Leaving CStop mode\r\n");
		   /* Test if system was on STOP mode */
		   if( (PWR->MCUCR & PWR_MCUCR_STOPF) == PWR_MCUCR_STOPF)
		   {
			  printf("System was on STOP mode\r\n");
			  /* Clear the MCU flags */
			  SET_BIT(PWR->MCUCR, PWR_MCUCR_CSSF);
			  /* Restore clocks */
			  /*
			  if (RCC_restoreClocks() == HAL_OK)
			  {
				  printf("CM4 restored clocks successfully\r\n");
			  }
			  */
			}
			/* All level of ITs can interrupt */
			__set_BASEPRI(0U);
	  }
	  if (!strncmp((char *)VirtUart1ChannelBuffRx, MSG_DELAY, strlen(MSG_DELAY)))
	  {
		  printf("Waiting 20 secs before sending the answer message\r\n");
		  HAL_Delay(20 *1000);
		  strcpy((char *)BuffTx, "m4:wakeup A7\n");
		  printf("%s\r", BuffTx);
		  VIRT_UART_Transmit(&huart1, BuffTx, strlen((const char *)BuffTx));
	  }
	  memset(VirtUart1ChannelBuffRx, 0 ,VirtUart1ChannelRxSize);
	  memset(BuffTx, 0 ,strlen((const char *)BuffTx));
	}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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

/**
  * @brief IPCC Initialization Function
  * @param None
  * @retval None
  */
static void MX_IPCC_Init(void)
{

  /* USER CODE BEGIN IPCC_Init 0 */

  /* USER CODE END IPCC_Init 0 */

  /* USER CODE BEGIN IPCC_Init 1 */

  /* USER CODE END IPCC_Init 1 */
  hipcc.Instance = IPCC;
  if (HAL_IPCC_Init(&hipcc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IPCC_Init 2 */

  /* USER CODE END IPCC_Init 2 */

}

/**
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin : LED_YELLOW_Pin */
  GPIO_InitStruct.Pin = LED_YELLOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LED_YELLOW_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

PUTCHAR_PROTOTYPE
{

  HAL_UART_Transmit(&huart8, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
#endif

void VIRT_UART0_RxCpltCallback(VIRT_UART_HandleTypeDef *huart)
{
    printf("Msg received on VIRTUAL UART0 channel:  %s \r\n", (char *) huart->pRxBuffPtr);
    /* copy received msg in a variable to sent it back to master processor in main infinite loop*/
    VirtUart0ChannelRxSize = huart->RxXferSize < MAX_BUFFER_SIZE? huart->RxXferSize : MAX_BUFFER_SIZE-1;
    memcpy(VirtUart0ChannelBuffRx, huart->pRxBuffPtr, VirtUart0ChannelRxSize);
    VirtUart0RxMsg = SET;
}
void VIRT_UART1_RxCpltCallback(VIRT_UART_HandleTypeDef *huart)
{
    printf("Msg received on VIRTUAL UART1 channel:  %s \r\n", (char *) huart->pRxBuffPtr);
    /* copy received msg in a variable to sent it back to master processor in main infinite loop*/
    VirtUart1ChannelRxSize = huart->RxXferSize < MAX_BUFFER_SIZE? huart->RxXferSize : MAX_BUFFER_SIZE-1;
    memcpy(VirtUart1ChannelBuffRx, huart->pRxBuffPtr, VirtUart1ChannelRxSize);
    VirtUart1RxMsg = SET;
}
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
