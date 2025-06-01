/*
 * dataframes.h
 *
 *  Created on: May 31, 2025
 *      Author: ishanchitale
 */

#ifndef INC_DATAFRAMES_H_
#define INC_DATAFRAMES_H_

#include "can.h"

#define J1939_ADDRESS_BROADCAST_ID 0x18EEFF80

typedef union J1939_ADDRESS_BROADCAST_DF {
	struct {
		uint8_t j1931_address_b1;
		uint8_t j1931_address_b2;
		uint8_t j1931_address_b3;
		uint8_t bms_address;
		uint8_t thermistor_module_number_shifted;
		uint8_t c1;
		uint8_t c2;
		uint8_t c3;
	} data;
	uint8_t array[8];
} J1939_ADDRESS_BROADCAST_DF;

#define THERMISTOR_BMS_BROADCAST_ID 0x1839F380

typedef union THERMISTOR_BMS_BROADCAST_DF {
	struct {
		uint8_t thermistor_module_number;
		uint8_t lowest_temp_value;
		uint8_t highest_temp_value;
		uint8_t average_temp_value;
		uint8_t num_thermistors;
		uint8_t highest_thermistor_id;
		uint8_t lowest_thermistor_id;
		uint8_t checksum;
	} data;
	uint8_t array[8];
} THERMISTOR_BMS_BROADCAST_DF;

typedef struct DAUGHTER_CAN_CONTEXT {
	J1939_ADDRESS_BROADCAST_DF address_broadcast_dataframe;
	CAN_TxHeaderTypeDef ADDRESS_CTXHeader;

	THERMISTOR_BMS_BROADCAST_DF thermistor_broadcast_dataframe;
	CAN_TxHeaderTypeDef THERMISTOR_CTXHeader;
} DAUGHTER_CAN_CONTEXT;

#endif /* INC_DATAFRAMES_H_ */
