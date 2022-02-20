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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "driver_ucan.h"
#include "usb_data.h"
#include "tool.h"
#include "inter_flash.h"
//#include "inter_flash.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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


USART_HandleTypeDef husart1;
USART_HandleTypeDef husart2;

/* USER CODE BEGIN PV */

unsigned char reciveFlag=0;
unsigned char can2Flag=0;
unsigned char can3Flag=0;
int RLeng=0;


/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_Init(void);
static void MX_USART2_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_FDCAN3_Init(void);
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

//	int rIndex=0;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	  __enable_irq();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_Device_Init();
  //MX_USART1_Init();
  //MX_USART2_Init();
	Driver_CANPara_Init();
	Usb_Data_Init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);
	Read_Update_Para();
	
  //MX_FDCAN1_Init();
 // MX_FDCAN2_Init();
  //MX_FDCAN3_Init();
  /* USER CODE BEGIN 2 */
	//FDCAN_Config();
	//FDCAN2_Config();
	
	
	/* Configure reception filter to Rx FIFO 0 on both FDCAN instances */
	/*--lilei-1122
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x666;
  sFilterConfig.FilterID2 = 0x000;
  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
  {
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    Error_Handler();
		
  }
  if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK)
  {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    Error_Handler();
		 
  }
	 */

  /* Configure global filter on both FDCAN instances:
     Filter all remote frames with STD and EXT ID
     Reject non matching frames with STD ID and EXT ID */
  /*
	 add by lilei-1122
	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    Error_Handler();
		 
  }
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan3, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    Error_Handler();
		
  }
  */
  /* Activate Rx FIFO 0 new message notification on both FDCAN instances */
	
	/*add bylilie-1122
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    Error_Handler();
		
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    Error_Handler();
		
  }
	
	*/
	
	

	
	
	

  /* Prepare Tx message Header */


	
  /* Configure and enable Tx Delay Compensation, required for BRS mode.
     TdcOffset default recommended value: DataTimeSeg1 * DataPrescaler
     TdcFilter default recommended value: 0 */
  /*if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan1, 5, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan2, 5, 0) != HAL_OK)
  {
    Error_Handler();
  }*/
	
	/*   add by lilei-1122
  if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FDCAN_EnableTxDelayCompensation(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }
  
    if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }*/
  USB_POWER_ON;
  /* Start the FDCAN module on both FDCAN instances */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
				
				USB_Data_Out();
		    Print_Test();
		    /*HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
				if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, TxData) != HAL_OK)
        {
		  	  CDC_Transmit_FS("can2 send fail",strlen("can2 send fail"));
        }
				else
				{
					CDC_Transmit_FS("can2 send success",strlen("can2 send success"));
				}
				 HAL_Delay(500);
				if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &TxHeader, TxData) != HAL_OK)
        {
		  	  CDC_Transmit_FS("can3 send fail",strlen("can3 send fail"));
        }
				else
				{
					CDC_Transmit_FS("can3 send success",strlen("can3 send success"));
				
				}
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
				 HAL_Delay(500);
				if(can2Flag==1)
				{
					CDC_Transmit_FS("recive can2 data",strlen("recive can2 data"));
					MyPrintf("length:%02X,id:%08X.data0:%02X,data1:%02X,data2:%02X,data3:%02X,data4:%02X,data5:%02X,data6:%02X,data7:%02X\r\n",RxHeader.DataLength,RxHeader.Identifier,RxData[0],RxData[1],RxData[2],RxData[3],RxData[4],RxData[5],RxData[6],RxData[7]);
					RLeng=0;
					if(RxHeader.DataLength==FDCAN_DLC_BYTES_8)
					{
						 RLeng=8;
					}
					can2Flag=0;
				}
			  if(can3Flag==1)
				{
					CDC_Transmit_FS("recive can3 data",strlen("recive can data"));
					MyPrintf("length:%02X,id:%08X.data0:%02X,data1:%02X,data2:%02X,data3:%02X,data4:%02X,data5:%02X,data6:%02X,data7:%02X\r\n",RxHeader.DataLength,RxHeader.Identifier,RxData[0],RxData[1],RxData[2],RxData[3],RxData[4],RxData[5],RxData[6],RxData[7]);
					RLeng=0;
					can3Flag=0;
				}*/
  }
  /* USER CODE END 3 */
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
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_FDCAN;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */
    
  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  /* add by lilei-1122
	hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 17;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 10;
  hfdcan2.Init.NominalTimeSeg2 = 9;
  hfdcan2.Init.DataPrescaler = 17;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 10;
  hfdcan2.Init.DataTimeSeg2 = 9;
  hfdcan2.Init.StdFiltersNbr = 1;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief FDCAN3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN3_Init(void)
{

  /* USER CODE BEGIN FDCAN3_Init 0 */

  /* USER CODE END FDCAN3_Init 0 */

  /* USER CODE BEGIN FDCAN3_Init 1 */

  /* USER CODE END FDCAN3_Init 1 */
  /* add by lilei-1122
	hfdcan3.Instance = FDCAN3;
  hfdcan3.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan3.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan3.Init.AutoRetransmission = DISABLE;
  hfdcan3.Init.TransmitPause = DISABLE;
  hfdcan3.Init.ProtocolException = DISABLE;
  hfdcan3.Init.NominalPrescaler = 17;
  hfdcan3.Init.NominalSyncJumpWidth = 1;
  hfdcan3.Init.NominalTimeSeg1 = 10;
  hfdcan3.Init.NominalTimeSeg2 = 9;
  hfdcan3.Init.DataPrescaler = 17;
  hfdcan3.Init.DataSyncJumpWidth = 1;
  hfdcan3.Init.DataTimeSeg1 = 10;
  hfdcan3.Init.DataTimeSeg2 = 9;
  hfdcan3.Init.StdFiltersNbr = 1;
  hfdcan3.Init.ExtFiltersNbr = 0;
  hfdcan3.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE BEGIN FDCAN3_Init 2 */

  /* USER CODE END FDCAN3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  husart1.Instance = USART1;
  husart1.Init.BaudRate = 115200;
  husart1.Init.WordLength = USART_WORDLENGTH_7B;
  husart1.Init.StopBits = USART_STOPBITS_1;
  husart1.Init.Parity = USART_PARITY_NONE;
  husart1.Init.Mode = USART_MODE_TX_RX;
  husart1.Init.CLKPolarity = USART_POLARITY_LOW;
  husart1.Init.CLKPhase = USART_PHASE_1EDGE;
  husart1.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  husart1.Init.ClockPrescaler = USART_PRESCALER_DIV1;
  husart1.SlaveMode = USART_SLAVEMODE_DISABLE;
  if (HAL_USART_Init(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetTxFifoThreshold(&husart1, USART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetRxFifoThreshold(&husart1, USART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_DisableFifoMode(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 115200;
  husart2.Init.WordLength = USART_WORDLENGTH_7B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_1EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  husart2.Init.ClockPrescaler = USART_PRESCALER_DIV1;
  husart2.SlaveMode = USART_SLAVEMODE_ENABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetTxFifoThreshold(&husart2, USART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetRxFifoThreshold(&husart2, USART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_DisableFifoMode(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_EnableSlaveMode(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PB11 PB12 PB13 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_7, GPIO_PIN_RESET);

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
