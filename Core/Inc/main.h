/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define MONOCHROME_COLOR 0x1c|0x03 // color256_green  0x1c, color256_blue    0x03
#define TIMEBOMB
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define SWITCH_Pin GPIO_PIN_15
#define SWITCH_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define SD_DI_Pin GPIO_PIN_6
#define SD_DI_GPIO_Port GPIOA
#define SD_DO_Pin GPIO_PIN_7
#define SD_DO_GPIO_Port GPIOA
#define CARDDETECT_Pin GPIO_PIN_4
#define CARDDETECT_GPIO_Port GPIOC
#define SSD2828_RST_Pin GPIO_PIN_5
#define SSD2828_RST_GPIO_Port GPIOC
#define SSD2828_CS_Pin GPIO_PIN_0
#define SSD2828_CS_GPIO_Port GPIOB
#define SSD2828_SCK_Pin GPIO_PIN_1
#define SSD2828_SCK_GPIO_Port GPIOB
#define SSD2828_SDO_Pin GPIO_PIN_2
#define SSD2828_SDO_GPIO_Port GPIOB
#define SSD2828_SDI_Pin GPIO_PIN_10
#define SSD2828_SDI_GPIO_Port GPIOB
#define SPI_RA8876_NSS_Pin GPIO_PIN_12
#define SPI_RA8876_NSS_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SD_CLK_Pin GPIO_PIN_8
#define SD_CLK_GPIO_Port GPIOB
#define RESET_RA8876_Pin GPIO_PIN_9
#define RESET_RA8876_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern char buttonClicked;
extern UART_HandleTypeDef huart3;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
