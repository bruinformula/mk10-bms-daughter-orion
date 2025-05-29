///*
// * mcp3204.h
// *
// *  Created on: May 23, 2025
// *      Author: ishanchitale
// */
//

#ifndef INC_MCP3204_H_
#define INC_MCP3204_H_

#include "stdint.h"
#include "spi.h"

#define START_BIT 1
#define RESV_BIT 0
#define MCP_VREF 3.3

typedef struct MCP3204 {
	// Fill in with voltage variables later

	SPI_HandleTypeDef* spiInstance;
	GPIO_TypeDef* CS_PORT;
	uint16_t CS_PIN;

	uint16_t rawADC;
	float voltageReading;

	union {
		uint8_t byte;
		struct {
			volatile unsigned int RESV: 1;
			volatile unsigned int CH_SELECT: 4;
			volatile unsigned int START: 1;
		} bits;
	} config;

} MCP3204;

typedef enum CHANNELS {
	CH_0 = 0b1000,
	CH_1 = 0b1001,
	CH_2 = 0b1010,
	CH_3 = 0b1011,
	PN_0_1 = 0b0000,
	PN_1_0 = 0b0001,
	PN_2_3 = 0b0010,
	PN_3_2 = 0b0011
} CHANNELS;

void initMCP(MCP3204* mcp, SPI_HandleTypeDef* spiInstance, GPIO_TypeDef* port, uint16_t pin);
void getADCValue(MCP3204* mcp);
void computeVoltage(MCP3204* mcp);

void enableCH0(MCP3204* mcp);
void enableCH1(MCP3204* mcp);
void enableCH2(MCP3204* mcp);
void enableCH3(MCP3204* mcp);
void enablePN_0_1(MCP3204* mcp);
void enablePN_1_0(MCP3204* mcp);
void enablePN_2_3(MCP3204* mcp);
void enablePN_3_2(MCP3204* mcp);

#endif /* INC_MCP3204_H_ */
