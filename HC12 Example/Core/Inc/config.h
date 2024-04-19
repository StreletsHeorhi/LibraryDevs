#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"

#define FALSE 0
#define TRUE 	1

typedef enum{
	PRESSED,
	NOT_PRESSED,
	NOT_CONNECTED,
	UNDEFINE
}buttonStateTypeDef;

typedef union{
	uint8_t data[6];
	struct {
		uint8_t source;
		uint8_t destination;
		uint8_t id;
		uint8_t dataL;
		uint8_t dataH;
		uint8_t checkSum;	
	}byte;
}message;
	

typedef	union{
	uint8_t full;
	struct{
		uint8_t bit0 			:1;
		uint8_t bit1			:1;
		uint8_t bit2			:1;
		uint8_t bit3			:1;
		uint8_t bit4			:1;
	} value;
}id;


#endif /* INC_CONFIG_H_ */