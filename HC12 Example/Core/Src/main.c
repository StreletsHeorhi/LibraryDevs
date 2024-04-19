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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"
#include "hc12.h"
#define connFilter 	100
#define btnFilterH 	25
#define btnFilterL 	0
//extern DMA_HandleTypeDef hdma_usart1_rx;
//extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;

HC12_HandleTypeDef hhc12;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t adcData[10] = {0,};
volatile message recieve;
volatile message trancieve = {0x01, 0x00, 0x01, 0x00, 0x00, 0x00};
id myID = {0,};
volatile uint8_t newMessage = 0;

uint8_t checkSum = 0;
float VDC = 0;
uint16_t TEMP = 0;


uint8_t ADC_ConvCpltFlag = FALSE;
uint8_t mustSendDataHandle = FALSE;


buttonStateTypeDef stateBtn[8] = {
	UNDEFINE,UNDEFINE,UNDEFINE,UNDEFINE,
	UNDEFINE,UNDEFINE,UNDEFINE,UNDEFINE
};


uint8_t ctrPress[8] = {0,};
uint8_t ctrConnect[8] = {0,};
uint8_t changeBtnState[8] = {0,};
buttonStateTypeDef bufferTransmit[8] = {0,};
	

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
HC12_StatusTypeDef stausHC12;
uint16_t timeDelayHC12 = 100;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
	hhc12.baudRate = B115200;
	hhc12.chanal = C001;
	hhc12.mode = FU3;
	hhc12.power = P20db;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin, GPIO_PIN_SET);
	stausHC12 = HC12_Init(&hhc12, &huart1, HC12_Mode_GPIO_Port, HC12_Mode_Pin);
	stausHC12 = HC12_SetEnergyMode(&hhc12, SLEEP, timeDelayHC12);


	
	
	//HC12_Init(&hhc12, );
	
	
	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 10);
	
	HAL_TIM_Base_Start(&htim3);
	
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)&recieve, sizeof(recieve));
	//__HAL_DMA_DISABLE_IT(&huart1,DMA_IT_HT);

	
	myID.value.bit0 = !HAL_GPIO_ReadPin(ID0_GPIO_Port, ID0_Pin);
	myID.value.bit1 = !HAL_GPIO_ReadPin(ID1_GPIO_Port, ID1_Pin);
	myID.value.bit2 = !HAL_GPIO_ReadPin(ID2_GPIO_Port, ID2_Pin);
	myID.value.bit3 = !HAL_GPIO_ReadPin(ID3_GPIO_Port, ID3_Pin);
	myID.value.bit4 = !HAL_GPIO_ReadPin(ID4_GPIO_Port, ID4_Pin);
	myID.full == 0  ?  HAL_GPIO_WritePin(R_GPIO_Port,R_Pin,GPIO_PIN_SET) : \
										 HAL_GPIO_WritePin(B_GPIO_Port,B_Pin,GPIO_PIN_SET);
	myID.full == 0  ? myID.full = 1 : myID.full;
	//HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin, GPIO_PIN_SET);
	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if( ADC_ConvCpltFlag != FALSE ){

					
			for(uint8_t i = 0; i < 8; i++ ){
				if(adcData[i] > 2100 ){
					ctrConnect[i] = 0;
					ctrPress[i] = ctrPress[i] > btnFilterL ? ctrPress[i]-1 : btnFilterL;
					if (ctrPress[i] == btnFilterL & stateBtn[i] != NOT_PRESSED){	
						stateBtn[i] = NOT_PRESSED;
					}
				}
	
				else if(adcData[i] > 1000){
					ctrConnect[i] = ctrConnect[i] < connFilter ? ctrConnect[i]+1 : connFilter;
					if (ctrConnect[i] == connFilter & stateBtn[i] != NOT_CONNECTED){	
						stateBtn[i] = NOT_CONNECTED;
					}
				}
				else{
					ctrPress[i] = ctrPress[i] < btnFilterH ? ctrPress[i]+1 : btnFilterH;
					if (ctrPress[i] == btnFilterH & stateBtn[i] != PRESSED){	
						stateBtn[i] = PRESSED;
						changeBtnState[i] = 1;
						newMessage = 1;
						
						mustSendDataHandle = TRUE;
					}
				}
			}
			VDC = 1.180 * 4095 / adcData[9];
     }
		
		 
		 
		 
		 	if(mustSendDataHandle == TRUE ){
				uint8_t ctr = 0x0A;
				
				if(stausHC12 == HC12_SLEEP){
					
					stausHC12 = HC12_SetEnergyMode(&hhc12, RUN, timeDelayHC12);
					
				}

				while (newMessage && ctr > 0){

					for(uint8_t i = 0; i < 8; i++ ){
						bufferTransmit[i] = changeBtnState[i] == 1 ? PRESSED : stateBtn[i];
					}
					
					ctr--;
					uint8_t checkSum = 0;
					trancieve.byte.dataL = 0;
					trancieve.byte.dataH = 0;

					trancieve.byte.source = myID.full;
					trancieve.byte.destination = 0x00;
					
					for(uint8_t i = 0; i < 4; i++){
						trancieve.byte.dataL += bufferTransmit[i] << i*2;
						trancieve.byte.dataH += bufferTransmit[i+4] << i*2;
					}

					for(uint8_t j = 0; j < 5; j++){
						checkSum +=  trancieve.data[j];
					}
					trancieve.byte.checkSum = (~checkSum)+1;
					

					HAL_Delay(myID.full*2);
					HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin ,GPIO_PIN_SET);
					

					
					while (HAL_UART_Transmit_IT(&huart1, (uint8_t*)&trancieve , sizeof(trancieve)) != HAL_OK);
					
					HAL_GPIO_TogglePin(SYNC_GPIO_Port,SYNC_Pin);
					HAL_Delay(200);
					
				}
				mustSendDataHandle = FALSE;
				if (ctr == 0) HAL_GPIO_WritePin(R_GPIO_Port,R_Pin, GPIO_PIN_SET);
				HAL_Delay(1000);
				stausHC12 = HC12_SetEnergyMode(&hhc12, SLEEP, timeDelayHC12);
				HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)&recieve, sizeof(recieve));				
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART1){
		if (recieve.byte.source == 0x00 & recieve.byte.destination == myID.full){

			 checkSum = 0;	
			
			for(uint8_t i = 0; i < 5; i++){
				checkSum += recieve.data[i];
			}	
			checkSum = (~checkSum)+1;
			
			if(checkSum == recieve.byte.checkSum){
				switch (recieve.byte.id){
				
					case 0x01: 
						newMessage = 0;
						HAL_GPIO_WritePin(R_GPIO_Port,R_Pin, GPIO_PIN_RESET);

					
						for(uint8_t i = 0; i < 8; i++ ){
							changeBtnState[i] =  NOT_PRESSED;
						}
					break;
						
					case 0x02: 
						newMessage = 1;
						mustSendDataHandle = TRUE;
					break;
					
					default: 
					break;
			
			  }
			}
		}
//		else if( recieve.data[0] == 'O' && recieve.data[1] == 'K'){
//			 hcStatus = HC12_OK;
//			//hcStatus = HC12_OK;
//		}
		
		
			HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)&recieve, sizeof(recieve));
		//__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		//HAL_GPIO_WritePin(USART1_DE_GPIO_Port, USART1_DE_Pin, GPIO_PIN_RESET);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)&recieve, sizeof(recieve));
		//__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
	}
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 10);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	if(hadc->Instance == ADC1){	
		ADC_ConvCpltFlag = TRUE;	
	}
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
