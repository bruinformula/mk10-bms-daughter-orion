/*
 * voltage_calculations.c
 *
 *  Created on: May 31, 2025
 *      Author: ishanchitale
 */

#include "voltage_calculations.h"

MCP3204 MCP1;
MCP3204 MCP2;
MCP3204 MCP3;

const float voltage_table[33] = {
		2.44, 2.42, 2.40, 2.38, 2.35, 2.32, 2.27, 2.23, 2.17, 2.11, 2.05, 1.99, 1.92, 1.86, 1.8, 1.74, 1.68,
		1.63, 1.59, 1.55, 1.51, 1.48, 1.45, 1.43, 1.40, 1.38, 1.37, 1.35, 1.34, 1.33, 1.32, 1.31, 1.30
};

const float temp_table[33] = {
		-40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
		55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120
};

float temp_conversions[20];

uint16_t rawADCBuffer[8];
float voltageBuffer[8];

float highestTemp;
float lowestTemp;
float averageTemp;

int numValidTemps;

// ALL VOLTAGE CALCULATIONS!!!!!!
void computeAllVoltages() {
	  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) rawADCBuffer, 8);
	  computeSTM_ADC_Voltages();

	  computeCH0(&MCP1);
	  computeCH1(&MCP1);
	  computeCH2(&MCP1);
	  computeCH3(&MCP1);

	  computeCH0(&MCP2);
	  computeCH1(&MCP2);
	  computeCH2(&MCP2);
	  computeCH3(&MCP2);

	  computeCH0(&MCP3);
	  computeCH1(&MCP3);
	  computeCH2(&MCP3);
	  computeCH3(&MCP3);
}

void computeAllTemps() {
	  // Index i corresponds to TEMP i+1
	  temp_conversions[0] = voltageToTemp(MCP1.CH0_Voltage);
	  temp_conversions[1] = voltageToTemp(MCP1.CH1_Voltage);
	  temp_conversions[2] = voltageToTemp(MCP1.CH2_Voltage);
	  temp_conversions[3] = voltageToTemp(MCP1.CH3_Voltage);

	  temp_conversions[4] = voltageToTemp(MCP2.CH0_Voltage);
	  temp_conversions[5] = voltageToTemp(MCP2.CH1_Voltage);
	  temp_conversions[6] = voltageToTemp(MCP2.CH2_Voltage);
	  temp_conversions[7] = voltageToTemp(MCP2.CH3_Voltage);

	  temp_conversions[8] = voltageToTemp(MCP3.CH0_Voltage);
	  temp_conversions[9] = voltageToTemp(MCP3.CH1_Voltage);
	  temp_conversions[10] = voltageToTemp(MCP3.CH2_Voltage);
	  temp_conversions[11] = voltageToTemp(MCP3.CH3_Voltage);

	  temp_conversions[12] = voltageToTemp(voltageBuffer[0]);
	  temp_conversions[13] = voltageToTemp(voltageBuffer[1]);
	  temp_conversions[14] = voltageToTemp(voltageBuffer[2]);
	  temp_conversions[15] = voltageToTemp(voltageBuffer[3]);
	  temp_conversions[16] = voltageToTemp(voltageBuffer[4]);
	  temp_conversions[17] = voltageToTemp(voltageBuffer[5]);
	  temp_conversions[18] = voltageToTemp(voltageBuffer[6]);
	  temp_conversions[19] = voltageToTemp(voltageBuffer[7]);
}

void computeSTM_ADC_Voltages() {
	for (size_t i = 0; i < 8; i++) {
		voltageBuffer[i] = (rawADCBuffer[i] * ADC_VREF)/4095.0;
	}
}

float voltageToTemp(float V) {
	if (V > voltage_table[0] || V < voltage_table[32]) {
		return 999.0; // Out of range
	}

	for (size_t i = 0; i < 32; i++) {
		float v_high = voltage_table[i];      // higher voltage, lower temp
		float v_low = voltage_table[i + 1];   // lower voltage, higher temp

		if (V <= v_high && V >= v_low) {
			float t_high = temp_table[i];
			float t_low = temp_table[i + 1];

			// Linear interpolation
			float temp = t_high + (V - v_high) * (t_low - t_high) / (v_low - v_high);
			return temp;
		}
	}

	return 999.0; // Should not reach here
}

void getLowestTemp() {
	float min = FLT_MAX;
	for (size_t i = 0; i < 20; i++) {
		if (temp_conversions[i] < min) min = temp_conversions[i];
	}
	lowestTemp = min;
}

void getHighestTemp() {
	float max = -FLT_MAX;
	for (size_t i = 0; i < 20; i++) {
		if (temp_conversions[i] > max && temp_conversions[i] < 999) max = temp_conversions[i];
	}
	highestTemp = max;
}

void getAverageTemp() {
	int count = 0;
	float sum = 0;
	for (size_t i = 0; i < 20; i++) {
		if (temp_conversions[i] < 999) {
			sum+=temp_conversions[i];
			count++;
		}
	}
	numValidTemps = count; // Reset the value of numValidTemps based on this new count
	averageTemp = (sum/numValidTemps);
}

// CAN DATAFRAME IMPLEMENTATIONS
void formAddressDataframe(J1939_ADDRESS_BROADCAST_DF* dataframe) {
	dataframe->data.j1931_address_b1 = 0xF3;
	dataframe->data.j1931_address_b2 = 0x00;
	dataframe->data.j1931_address_b3 = 0x80;
	dataframe->data.bms_address = 0xF3;
//	dataframe->data.thermistor_module_number_shifted = (MODULE_NUMBER << 3);
	dataframe->data.thermistor_module_number_shifted = ((MODULE_NUMBER-1) << 3);
	dataframe->data.c1 = 0x40;
	dataframe->data.c2 = 0x1E;
	dataframe->data.c3 = 0x90;
}

void formThermistorDataframe(THERMISTOR_BMS_BROADCAST_DF* dataframe) {
	// Do all the necessary calculations
	computeAllVoltages();
	computeAllTemps();

	getLowestTemp();
	getHighestTemp();
	getAverageTemp();

	uint8_t checksum = 0;

	// Now prepare the dataframe!
//	dataframe->data.thermistor_module_number = MODULE_NUMBER;
	dataframe->data.thermistor_module_number = (MODULE_NUMBER-1);
	dataframe->data.lowest_temp_value = (int8_t)lowestTemp;
	dataframe->data.highest_temp_value = (int8_t)highestTemp;
	dataframe->data.average_temp_value = (int8_t)averageTemp;
	dataframe->data.num_thermistors = NUM_THERMISTORS;
	dataframe->data.highest_thermistor_id = THERMISTOR_HIGHEST_INDEX;
	dataframe->data.lowest_thermistor_id = THERMISTOR_LOWEST_INDEX;

	for (size_t i = 0; i < 7; i++) {
		checksum+=dataframe->array[i];
	}
	checksum+=0x39;
	checksum+=8;

	dataframe->data.checksum = checksum;
}

