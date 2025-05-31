/*
 * calculations.h
 *
 *  Created on: May 31, 2025
 *      Author: ishanchitale
 */

#ifndef INC_VOLTAGE_CALCULATIONS_H_
#define INC_VOLTAGE_CALCULATIONS_H_

#include "adc.h"
#include "float.h"
#include "mcp3204.h"
#include "dataframes.h"

// External MCP3204 ADCs
#define CS1_PORT GPIOC
#define CS1_PIN GPIO_PIN_15

#define CS2_PORT GPIOC
#define CS2_PIN GPIO_PIN_14

#define CS3_PORT GPIOB
#define CS3_PIN GPIO_PIN_7

extern MCP3204 MCP1;
extern MCP3204 MCP2;
extern MCP3204 MCP3;

// For the onboard STM32 ADCs
#define ADC_VREF 3.3
extern uint16_t rawADCBuffer[8];
extern float voltageBuffer[8];
void computeSTM_ADC_Voltages();

// ALL TEMP COMPUTATIONS!!!!
extern const float voltage_table[33];
extern const float temp_table[33];
extern float temp_conversions[20];
void computeAllVoltages();
void computeAllTemps();
float voltageToTemp(float V);

extern float highestTemp;
extern float lowestTemp;
extern float averageTemp;
void getLowestTemp();
void getHighestTemp();
void getAverageTemp();

// Preparing the CAN Dataframes
#define MODULE_NUMBER 1
#define NUM_THERMISTORS 20
#define THERMISTOR_LOWEST_INDEX ((MODULE_NUMBER - 1)*128)
#define THERMISTOR_HIGHEST_INDEX (THERMISTOR_LOWEST_INDEX + NUM_THERMISTORS-1)
void formAddressDataframe(uint32_t* lastReadMS,  J1939_ADDRESS_BROADCAST_DF* dataframe);
void formThermistorDataframe(uint32_t* lastReadMS,  THERMISTOR_BMS_BROADCAST_DF* dataframe);

#endif /* INC_VOLTAGE_CALCULATIONS_H_ */
