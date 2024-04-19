#ifndef HC_12_H_
#define HC_12_H_

#include "main.h"

#define UART_TIMEOUT 1000


typedef enum{
	HC12_OK,
	HC12_BUSY,
	HC12_SLEEP,
	HC12_ERROR_TX,
	HC12_ERROR_RX,
	HC12_TIMEOUT
}HC12_StatusTypeDef;


typedef enum {
	B1200,
	B2400,
	B4800,
	B9600,
	B19200,
	B38400,
	B57600,
	B115200
}HC12_BaudRateTypeDef;


typedef enum {
	C001 = 0,	C002,	C003,	C004,	C005,	C006,	C007,	
	C008,	C009,	C010,	C011,	C012,	C013,	C014,	C015,
	C016, C017,	C018,	C019,	C020,	C021,	C022,	C023,	
	C024,	C025,	C026,	C027,	C028,	C029,	C030,	C031,	
	C032,	C033,	C034,	C035,	C036,	C037,	C038,	C039,	
	C040,	C041,	C042,	C043,	C044,	C045,	C046,	C047,	
	C048,	C049,	C050,	C051,	C052,	C053,	C054,	C055,	
	C056,	C057,	C058,	C059,	C060,	C061,	C062,	C063,	
	C064,	C065,	C066,	C067,	C068,	C069,	C070,	C071,	
	C072,	C073,	C074,	C075,	C076,	C077,	C078,	C079,	
	C080,	C081,	C082,	C083,	C084,	C085,	C086,	C087,	
	C088,	C089,	C090,	C091,	C092,	C093,	C094,	C095,	
	C096,	C097,	C098,	C099,	C100,	C101,	C102,	C103,	
	C104,	C105,	C106,	C107,	C108,	C109,	C110,	C111,	
	C112,	C113,	C114,	C115,	C116,	C117,	C118,	C119,	
	C120,	C121,	C122,	C123,	C124,	C125,	C126,	C127
}HC12_ChanalTypeDef;


typedef enum {
	FU3,
	FU2,
	FU1,
}HC12_ModeTypeDef;


typedef enum{
	P20db,
	P17db,
	P14db,
	P11db,
	P8db,
	P5db,
	P2db,
	P0db
}HC12_PowerTypeDef;


typedef enum {
	RUN,
	SLEEP
}HC12_EnergyModeTypeDef;


typedef struct{
	UART_HandleTypeDef *huart;
	GPIO_TypeDef *port;
	uint16_t pin;
}HC12_SettingTypeDef;


typedef struct{
	HC12_SettingTypeDef setting;
	HC12_BaudRateTypeDef baudRate;
	HC12_ChanalTypeDef chanal;
	HC12_PowerTypeDef power;
	HC12_ModeTypeDef mode;
}HC12_HandleTypeDef;


HC12_StatusTypeDef HC12_Init(HC12_HandleTypeDef*, UART_HandleTypeDef*, GPIO_TypeDef*, uint16_t);
HC12_StatusTypeDef HC12_SetEnergyMode(HC12_HandleTypeDef*, HC12_EnergyModeTypeDef, uint16_t);

#endif /* HC_12_H_ */
