/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define configAPPLICATION_ALLOCATED_HEAP 1

#define LED_GSM_Pin GPIO_PIN_13
#define LED_GSM_GPIO_Port GPIOC
#define RELAY1_Pin GPIO_PIN_0
#define RELAY1_GPIO_Port GPIOC
#define RELAY2_Pin GPIO_PIN_1
#define RELAY2_GPIO_Port GPIOC
#define RELAY3_Pin GPIO_PIN_2
#define RELAY3_GPIO_Port GPIOC
#define RELAY4_Pin GPIO_PIN_3
#define RELAY4_GPIO_Port GPIOC
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define VBAT_SENS_Pin GPIO_PIN_0
#define VBAT_SENS_GPIO_Port GPIOB
#define ADC_IN1_Pin GPIO_PIN_1
#define ADC_IN1_GPIO_Port GPIOB
#define L3_DECT_Pin GPIO_PIN_10
#define L3_DECT_GPIO_Port GPIOB
#define RF_ON_OFF_Pin GPIO_PIN_11
#define RF_ON_OFF_GPIO_Port GPIOB
#define RS485_DIR_Pin GPIO_PIN_8
#define RS485_DIR_GPIO_Port GPIOC
#define LED_SYS_Pin GPIO_PIN_9
#define LED_SYS_GPIO_Port GPIOC
#define GSM_RI_Pin GPIO_PIN_8
#define GSM_RI_GPIO_Port GPIOA
#define GSM_RI_EXTI_IRQn EXTI9_5_IRQn
#define SWITCH1_Pin GPIO_PIN_0
#define SWITCH1_GPIO_Port GPIOD
#define INPUT1_Pin GPIO_PIN_3
#define INPUT1_GPIO_Port GPIOD
#define INPUT2_Pin GPIO_PIN_4
#define INPUT2_GPIO_Port GPIOD
#define INPUT3_Pin GPIO_PIN_5
#define INPUT3_GPIO_Port GPIOD
#define INPUT4_Pin GPIO_PIN_6
#define INPUT4_GPIO_Port GPIOD
#define GSM_PWR_KEY_Pin GPIO_PIN_3
#define GSM_PWR_KEY_GPIO_Port GPIOB
#define GSM_PWR_ON_Pin GPIO_PIN_4
#define GSM_PWR_ON_GPIO_Port GPIOB
#define P_GOOD_Pin GPIO_PIN_5
#define P_GOOD_GPIO_Port GPIOB
#define CHARGE_EN_Pin GPIO_PIN_6
#define CHARGE_EN_GPIO_Port GPIOB
#define L1_DECT_Pin GPIO_PIN_8
#define L1_DECT_GPIO_Port GPIOB
#define L2_DECT_Pin GPIO_PIN_9
#define L2_DECT_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
