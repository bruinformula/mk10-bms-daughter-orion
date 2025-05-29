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

	mcp->txData[1] = 0;
	mcp->txData[2] = 0;
	mcp->txData[3] = 0;

	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_SET); // Assert it high
}

void getADCValue(MCP3204* mcp) {
	// Assume desired channel has been selected already...
	mcp->txData[0] = mcp->config.byte;

	mcp->rxData[0] = 0;
	mcp->rxData[1] = 0;
	mcp->rxData[2] = 0;
	mcp->rxData[3] = 0;

	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(mcp->spiInstance, mcp->txData, mcp->rxData, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(mcp->CS_PORT, mcp->CS_PIN, GPIO_PIN_SET);

	uint16_t value = ((uint16_t)mcp->rxData[1] << 8) | mcp->rxData[2];
	mcp->rawADC = (value >> 3); // Use RSHIFT-3
}

void computeCH0(MCP3204* mcp) {
	enableCH0(mcp);
	getADCValue(mcp);
	mcp->CH0_Voltage = computeVoltage(mcp->rawADC);
}

void computeCH1(MCP3204* mcp) {
	enableCH1(mcp);
	getADCValue(mcp);
	mcp->CH1_Voltage = computeVoltage(mcp->rawADC);
}

void computeCH2(MCP3204* mcp) {
	enableCH2(mcp);
	getADCValue(mcp);
	mcp->CH2_Voltage = computeVoltage(mcp->rawADC);
}

void computeCH3(MCP3204* mcp) {
	enableCH3(mcp);
	getADCValue(mcp);
	mcp->CH3_Voltage = computeVoltage(mcp->rawADC);
}

float computeVoltage(uint16_t adc_reading) {
	return (adc_reading * MCP_VREF)/4095.0;
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

//void enablePN_0_1(MCP3204* mcp) {
//	mcp->config.bits.CH_SELECT = PN_0_1;
//}
//
//void enablePN_1_0(MCP3204* mcp) {
//	mcp->config.bits.CH_SELECT = PN_1_0;
//}
//
//void enablePN_2_3(MCP3204* mcp) {
//	mcp->config.bits.CH_SELECT = PN_2_3;
//}
//
//void enablePN_3_2(MCP3204* mcp) {
//	mcp->config.bits.CH_SELECT = PN_3_2;
//}
