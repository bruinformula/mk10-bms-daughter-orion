///*
// * mcp3204.c
// *
// *  Created on: May 23, 2025
// *      Author: ishanchitale
// */


#include "mcp3204.h"

void initMCP(MCP3204* mcp, SPI_HandleTypeDef* spi, GPIO_TypeDef* port, uint16_t pin) {
	mcp->spiInstance = spi;
	mcp->CS_PORT = port;
	mcp->CS_PIN = pin;

	mcp->config.bits.RESV = RESV_BIT; // Fixed at 0
	mcp->config.bits.START = START_BIT; // Fixed at 1

	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_SET); // Assert it high
}

void getADCValue(MCP3204* mcp) {
	// Assume desired channel has been selected already...
	uint8_t txData[4] = {mcp->config.byte, 0x00, 0x00, 0x00};
	uint8_t rxData[4] = {0x00, 0x00, 0x00, 0x00};

	// 010011010001

	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(mcp->spiInstance, txData, rxData, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_SET);

	uint16_t value = ((uint16_t)rxData[1] << 8) | rxData[2];
	mcp->rawADC = (value >> 4); // We only want the first 12 bits; use RSHIFT-4
}

void computeVoltage(MCP3204* mcp) {
	mcp->voltageReading = (mcp->rawADC * MCP_VREF)/4095.0;
}

void enableCH0(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = CH_0;
}

void enableCH1(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = CH_1;
}

void enableCH2(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = CH_2;
}

void enableCH3(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = CH_3;
}

void enablePN_0_1(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = PN_0_1;
}

void enablePN_1_0(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = PN_1_0;
}

void enablePN_2_3(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = PN_2_3;
}

void enablePN_3_2(MCP3204* mcp) {
	mcp->config.bits.CH_SELECT = PN_3_2;
}
