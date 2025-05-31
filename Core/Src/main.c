/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mcp3204.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CS1_PORT GPIOC
#define CS1_PIN GPIO_PIN_15

#define CS2_PORT GPIOC
#define CS2_PIN GPIO_PIN_14

#define CS3_PORT GPIOB
#define CS3_PIN GPIO_PIN_7

#define ADC_VREF 3.3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
MCP3204 MCP1;
MCP3204 MCP2;
MCP3204 MCP3;

uint16_t rawADCBuffer[8];
float voltageBuffer[8];

const float voltage_table[33] = {
		2.44, 2.42, 2.40, 2.38, 2.35, 2.32, 2.27, 2.23, 2.17, 2.11, 2.05, 1.99, 1.92, 1.86, 1.8, 1.74, 1.68,
		1.63, 1.59, 1.55, 1.51, 1.48, 1.45, 1.43, 1.40, 1.38, 1.37, 1.35, 1.34, 1.33, 1.32, 1.31, 1.30
};

const float temp_table[33] = {
		-40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
		55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120
};

float temp_conversions[20];

#define TABLE_SIZE 33

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void computeSTM_ADC_Voltages();
float voltageToTemp(float V);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void computeSTM_ADC_Voltages() {
	for (size_t i = 0; i < 8; i++) {
		voltageBuffer[i] = computeVoltage(rawADCBuffer[i]);
	}
}

float voltageToTemp(float V) {
	if (V > voltage_table[0] || V < voltage_table[TABLE_SIZE - 1]) {
		return 999.0; // Out of range
	}

	for (size_t i = 0; i < TABLE_SIZE - 1; i++) {
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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI3_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */

  initMCP(&MCP1, &hspi3, CS1_PORT, CS1_PIN);
  initMCP(&MCP2, &hspi3, CS2_PORT, CS2_PIN);
  initMCP(&MCP3, &hspi3, CS3_PORT, CS3_PIN);

//  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcBuffer, 8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

	   HAL_Delay(50);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
