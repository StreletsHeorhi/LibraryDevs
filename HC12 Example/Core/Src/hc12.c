
#include "hc12.h"
#include "main.h"
#include "usart.h"
#include <string.h>


const char *cmdTest 			= "AT\r\n";
const char *cmdSleep 			= "AT+SLEEP\r\n";
const char *cmdMode[] 		= {"AT+FU3\r\n", "AT+FU2\r\n", "AT+FU1\r\n"};
const char *cmdPower[] 		= {"AT+P8\r\n", "AT+P7\r\n", "AT+P6\r\n", "AT+P5\r\n", "AT+P4\r\n", "AT+P3\r\n", "AT+P2\r\n", "AT+P1\r\n"};
const char *cmdBaudRate[] = {"AT+B1200\r\n", "AT+B2400\r\n", "AT+B4800\r\n","AT+B9600\r\n", "AT+B19200\r\n", "AT+B38400\r\n", "AT+B57600\r\n", "AT+B115200\r\n"};
const char *cmdChannel[] 	= {"AT+C001\r\n", "AT+C002\r\n", "AT+C003\r\n", "AT+C004\r\n", "AT+C005\r\n", "AT+C006\r\n", "AT+C007\r\n", "AT+C008\r\n", "AT+C009\r\n", \
 "AT+C010\r\n", "AT+C011\r\n", "AT+C012\r\n", "AT+C013\r\n", "AT+C014\r\n", "AT+C015\r\n", "AT+C016\r\n", "AT+C017\r\n", "AT+C018\r\n", "AT+C019\r\n", "AT+C020\r\n", "AT+C021\r\n",\
 "AT+C022\r\n", "AT+C023\r\n", "AT+C024\r\n", "AT+C025\r\n", "AT+C026\r\n", "AT+C027\r\n", "AT+C028\r\n", "AT+C029\r\n", "AT+C030\r\n", "AT+C031\r\n", "AT+C032\r\n", "AT+C033\r\n",\
 "AT+C034\r\n", "AT+C035\r\n", "AT+C036\r\n", "AT+C037\r\n", "AT+C038\r\n", "AT+C039\r\n", "AT+C040\r\n", "AT+C041\r\n", "AT+C042\r\n", "AT+C043\r\n", "AT+C044\r\n", "AT+C045\r\n",\
 "AT+C046\r\n", "AT+C047\r\n", "AT+C048\r\n", "AT+C049\r\n", "AT+C050\r\n", "AT+C051\r\n", "AT+C052\r\n", "AT+C053\r\n", "AT+C054\r\n", "AT+C055\r\n", "AT+C056\r\n", "AT+C057\r\n",\
 "AT+C058\r\n", "AT+C059\r\n", "AT+C060\r\n", "AT+C061\r\n", "AT+C062\r\n", "AT+C063\r\n", "AT+C064\r\n", "AT+C065\r\n", "AT+C066\r\n", "AT+C067\r\n", "AT+C068\r\n", "AT+C069\r\n",\
 "AT+C070\r\n", "AT+C071\r\n", "AT+C072\r\n", "AT+C073\r\n", "AT+C074\r\n", "AT+C075\r\n", "AT+C076\r\n", "AT+C077\r\n", "AT+C078\r\n", "AT+C079\r\n", "AT+C080\r\n", "AT+C081\r\n",\
 "AT+C082\r\n", "AT+C083\r\n", "AT+C084\r\n", "AT+C085\r\n", "AT+C086\r\n", "AT+C087\r\n", "AT+C088\r\n", "AT+C089\r\n", "AT+C090\r\n", "AT+C091\r\n", "AT+C092\r\n", "AT+C093\r\n",\
 "AT+C094\r\n", "AT+C095\r\n", "AT+C096\r\n", "AT+C097\r\n", "AT+C098\r\n", "AT+C099\r\n", "AT+C100\r\n", "AT+C101\r\n", "AT+C102\r\n", "AT+C103\r\n", "AT+C104\r\n", "AT+C105\r\n",\
 "AT+C106\r\n", "AT+C107\r\n", "AT+C108\r\n", "AT+C109\r\n", "AT+C110\r\n", "AT+C111\r\n", "AT+C112\r\n", "AT+C113\r\n", "AT+C114\r\n", "AT+C115\r\n", "AT+C116\r\n", "AT+C117\r\n",\
 "AT+C118\r\n", "AT+C119\r\n", "AT+C120\r\n", "AT+C121\r\n", "AT+C122\r\n", "AT+C123\r\n", "AT+C124\r\n", "AT+C125\r\n", "AT+C126\r\n", "AT+C127\r\n"};
;



static void UART_Def_Init(UART_HandleTypeDef *huart)
{
	UART_HandleTypeDef xhuart;
  xhuart.Instance = huart->Instance;
  xhuart.Init.BaudRate = 9600;
  xhuart.Init.WordLength = UART_WORDLENGTH_8B;
  xhuart.Init.StopBits = UART_STOPBITS_1;
  xhuart.Init.Parity = UART_PARITY_NONE;
  xhuart.Init.Mode = UART_MODE_TX_RX;
  xhuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  xhuart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&xhuart) != HAL_OK)
  {
    Error_Handler();
  }
}


HC12_StatusTypeDef HC12_Init(HC12_HandleTypeDef* hhc, UART_HandleTypeDef *huart, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	
	char receiveData[10] = {0,};
	const char *cmdArray[] = {cmdTest, cmdBaudRate[hhc->baudRate], cmdMode[hhc->mode], cmdPower[hhc->power], cmdChannel[hhc->chanal]};
	
	hhc->setting.huart = huart;
	hhc->setting.port = GPIOx;
	hhc->setting.pin = GPIO_Pin;
	
	UART_Def_Init(huart);
	HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_RESET);
	HAL_Delay(100);

	for (int i = 0; i < 5; i++) {
		
		memset(receiveData, 0, sizeof(receiveData));
		
		if (HAL_UART_Transmit(hhc->setting.huart, (uint8_t*)cmdArray[i], strlen(cmdArray[i]), UART_TIMEOUT) != HAL_OK) {
			HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
			UART_Def_Init(huart);
			MX_USART1_UART_Init();
			return HC12_ERROR_TX;
		}

		if (HAL_UART_Receive(hhc->setting.huart, (uint8_t*)receiveData, strlen(cmdArray[i]), UART_TIMEOUT) != HAL_OK ||
			receiveData[0] != 'O' || receiveData[1] != 'K') {
			HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
			UART_Def_Init(huart);
			MX_USART1_UART_Init();
			return HC12_ERROR_RX;
		}
	}
	
	HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
	UART_Def_Init(huart);
	MX_USART1_UART_Init();
	return HC12_OK;
}



HC12_StatusTypeDef HC12_SetEnergyMode(HC12_HandleTypeDef *hhc, HC12_EnergyModeTypeDef mode, uint16_t delayTime){
	char receiveData[10] = {0,};
	
	HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_RESET);

	if(mode == SLEEP){
		
		HAL_Delay(delayTime);

		if (HAL_UART_Transmit(hhc->setting.huart, (uint8_t*)cmdSleep, strlen(cmdSleep), UART_TIMEOUT) != HAL_OK) {
			HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);		
			return HC12_ERROR_TX;
		}

		HAL_UART_AbortReceive(hhc->setting.huart);
		
		if (HAL_UART_Receive(hhc->setting.huart, (uint8_t*)receiveData, strlen(cmdSleep), UART_TIMEOUT) != HAL_OK ||
			receiveData[0] != 'O' || receiveData[1] != 'K') {
			HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
			return HC12_ERROR_RX;
		}

		HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
		return HC12_SLEEP;
	}
	HAL_GPIO_WritePin(hhc->setting.port, hhc->setting.pin ,GPIO_PIN_SET);
	HAL_Delay(delayTime);
	return HC12_OK;
}
