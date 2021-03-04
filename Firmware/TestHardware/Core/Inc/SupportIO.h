/*
 * SupportIO.h
 *
 *  Created on: Jan 15, 2021
 *      Author: IMR HabPC
 */

#ifndef INC_SUPPORTIO_H_
#define INC_SUPPORTIO_H_

#include "MainSupport.h"


// MACROS
// STATUS LED
#define STATUS_LED_ON()				HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET)
#define STATUS_LED_OFF()			HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET)
#define STATUS_LED_TOGGLE() 		HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin)

// 5V POWER
#define PWR_5V_ON()					HAL_GPIO_WritePin(PWR_5V_EN_GPIO_Port, PWR_5V_EN_Pin, GPIO_PIN_SET)
#define PWR_5V_OFF()				HAL_GPIO_WritePin(PWR_5V_EN_GPIO_Port, PWR_5V_EN_Pin, GPIO_PIN_RESET)
#define PWR_5V_TOGGLE() 			HAL_GPIO_TogglePin(PWR_5V_EN_GPIO_Port, PWR_5V_EN_Pin)
#define PWR_5V_STATUS()				HAL_GPIO_ReadPin(PWR_5V_GOOD_GPIO_Port, PWR_5V_GOOD_Pin)

// AUDIO EN
#define AUDIO_AMP_ON()				HAL_GPIO_WritePin(AUDIO_EN_GPIO_Port, AUDIO_EN_Pin, GPIO_PIN_SET)
#define AUDIO_AMP_OFF()				HAL_GPIO_WritePin(AUDIO_EN_GPIO_Port, AUDIO_EN_Pin, GPIO_PIN_RESET)
#define AUDIO_AMP_TOGGLE() 			HAL_GPIO_TogglePin(AUDIO_EN_GPIO_Port, AUDIO_EN_Pin)
#define AUDIO_AMP_STATUS()			HAL_GPIO_ReadPin(AUDIO_EN_GPIO_Port, AUDIO_EN_Pin)

// WIFI EN
#define WIFI_ON()					HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET)
#define WIFI_OFF()					HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_RESET)
#define WIFI_TOGGLE() 				HAL_GPIO_TogglePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin)
#define WIFI_STATUS()				HAL_GPIO_ReadPin(WIFI_EN_GPIO_Port, WIFI_EN_Pin)

// USER LEDS RED
#define UI_RED_LED_ON()				HAL_GPIO_WritePin(PWM_RLED_GPIO_Port, PWM_RLED_Pin, GPIO_PIN_SET)
#define UI_RED_LED_OFF()			HAL_GPIO_WritePin(PWM_RLED_GPIO_Port, PWM_RLED_Pin, GPIO_PIN_RESET)
#define UI_RED_LED_TOGGLE() 		HAL_GPIO_TogglePin(PWM_RLED_GPIO_Port, PWM_RLED_Pin)
// USER LEDS GREEN
#define UI_GREEN_LED_ON()			HAL_GPIO_WritePin(PWM_GLED_GPIO_Port, PWM_GLED_Pin, GPIO_PIN_SET)
#define UI_GREEN_LED_OFF()			HAL_GPIO_WritePin(PWM_GLED_GPIO_Port, PWM_GLED_Pin, GPIO_PIN_RESET)
#define UI_GREEN_LED_TOGGLE() 		HAL_GPIO_TogglePin(PWM_GLED_GPIO_Port, PWM_GLED_Pin)

// USER HEX SWITCH
#define HEX_1_STATUS()				HAL_GPIO_ReadPin(HEX_1_GPIO_Port, HEX_1_Pin)
#define HEX_2_STATUS()				HAL_GPIO_ReadPin(HEX_2_GPIO_Port, HEX_2_Pin)
#define HEX_4_STATUS()				HAL_GPIO_ReadPin(HEX_4_GPIO_Port, HEX_4_Pin)
#define HEX_8_STATUS()				HAL_GPIO_ReadPin(HEX_8_GPIO_Port, HEX_8_Pin)

// BACKLIGH_DISPLAY DEFINED AS OUTPUT OR PWM
#ifdef BACKLIGHT_AS_OUTPUT
#define BACKLIGHT_ON()				HAL_GPIO_WritePin(PWM_DISPLAY_GPIO_Port, PWM_DISPLAY_Pin, GPIO_PIN_SET)
#define BACKLIGHT_OFF()				HAL_GPIO_WritePin(PWM_DISPLAY_GPIO_Port, PWM_DISPLAY_Pin, GPIO_PIN_RESET)
#define BACKLIGHT_TOGGLE()			HAL_GPIO_TogglePin(PWM_DISPLAY_GPIO_Port, PWM_DISPLAY_Pin)
#define BACKLIGHT_STATUS()			HAL_GPIO_ReadPin(PWM_DISPLAY_GPIO_Port, PWM_DISPLAY_Pin)
#else
#define BACKLIGHT_ON()				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1)
#define BACKLIGHT_OFF()				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1)
#endif

// DISPLAY ENABLE
#define DISPLAY_ON()				HAL_GPIO_WritePin(DISPLAY_EN_GPIO_Port, DISPLAY_EN_Pin, GPIO_PIN_SET)
#define DISPLAY_OFF()				HAL_GPIO_WritePin(DISPLAY_EN_GPIO_Port, DISPLAY_EN_Pin, GPIO_PIN_RESET)
#define DISPLAY_EN_TOGGLE() 		HAL_GPIO_TogglePin(DISPLAY_EN_GPIO_Port, DISPLAY_EN_Pin)
#define DISPLAY_EN_STATUS()			HAL_GPIO_ReadPin(DISPLAY_EN_GPIO_Port, DISPLAY_EN_Pin)

// USB PWR AND CHARGE STATUS
#define VUSB_PWR_ON()				HAL_GPIO_WritePin(OTG_FS_VUSB_NEN_GPIO_Port, OTG_FS_VUSB_NEN_Pin, GPIO_PIN_RESET)
#define VUSB_PWR_OFF()				HAL_GPIO_WritePin(OTG_FS_VUSB_NEN_GPIO_Port, OTG_FS_VUSB_NEN_Pin, GPIO_PIN_SET)
#define VUSB_PWR_TOGGLE()			HAL_GPIO_TogglePin(OTG_FS_VUSB_NEN_GPIO_Port, OTG_FS_VUSB_NEN_Pin)
#define VUSB_PWR_STATUS()			HAL_GPIO_ReadPin(OTG_FS_VUSB_NEN_GPIO_Port, OTG_FS_VUSB_NEN_Pin)
#define SUPPLY_VUSB_FAULT_STATUS()	HAL_GPIO_ReadPin(OTG_FS_VUSB_NER_GPIO_Port, OTG_FS_VUSB_NER_Pin)

// TOUCH RST
#define TOUCH_RST_ENABLE()			HAL_GPIO_WritePin(TOUCH_NRST_GPIO_Port, TOUCH_NRST_Pin, GPIO_PIN_RESET)
#define TOUCH_RST_DISABLE()			HAL_GPIO_WritePin(TOUCH_NRST_GPIO_Port, TOUCH_NRST_Pin, GPIO_PIN_SET)
#define TOUCH_RST_STATUS()			HAL_GPIO_ReadPin(TOUCH_NRST_GPIO_Port, TOUCH_NRST_Pin)

#endif /* INC_SUPPORTIO_H_ */
