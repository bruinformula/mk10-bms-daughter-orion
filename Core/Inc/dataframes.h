/*
 * dataframes.h
 *
 *  Created on: May 31, 2025
 *      Author: ishanchitale
 */

#ifndef INC_DATAFRAMES_H_
#define INC_DATAFRAMES_H_

#define MODULE_NUMBER 1
#define NUM_THERMISTORS 20

typedef union J1939_ADDRESS_BROADCAST_DF {
	struct {
		uint8_t j1931_address_b1 = 0xF3;
		uint8_t j1931_address_b2 = 0x00;
		uint8_t j1931_address_b3 = 0x80;
		uint8_t bms_address = 0xF3;
		uint8_t thermistor_module_number_shifted = (MODULE_NUMBER << 3);
		uint8_t c1 = 0x40;
		uint8_t c2 = 0x1E;
		uint8_t c3 = 0x90;
	} data;
	uint8_t array[8];
};

typedef union THERMISTOR_BMS_BROADCAST_DF {
	struct {
		uint8_t thermistor_module_number = MODULE_NUMBER;
		uint8_t lowest_temp_value;
		uint8_t highest_temp_value;
		uint8_t average_temp_value;
		uint8_t num_thermistors = NUM_THERMISTORS;
		uint8_t lowest_thermistor_id;
		uint8_t highest_thermistor_id;
		uint8_t checksum;
	} data;
	uint8_t array[8];
};

#endif /* INC_DATAFRAMES_H_ */
