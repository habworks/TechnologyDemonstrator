/*
 * MainSupport.c
 *
 *  Created on: Jan 15, 2021
 *      Author: IMR HabPC
 */

#include "MainSupport.h"
#include "SupportIO.h"
#include "AdcSupport.h"
#include "UartSupport.h"
#include "TimerSupport.h"
// ST DRIVRS
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "fmc.h"
// GENERIC LIBS
#include "string.h"
#include "stdio.h"
// MIDDLEWEAR
#include "fatfs.h"
#include "usbd_cdc_if.h"

// STATIC PROTOTYPES
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
//static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);
//static void CPU_CACHE_Enable(void);

// GLOBAL VARS DEFINE HERE
volatile Type_BoardStatus BoardStatus;
// SERIAL PORT GLOBALS
uint8_t DebugUartInputMsg[10];
uint8_t DebugUarOutputtMsg[100];
// ADC GLOBALS
uint16_t ADC_DMA_Buffer[ADC_DMA_SIZE];
// SDRAM
// BUFFERS FOR MEMORY TEST
uint32_t aTxBuffer[BUFFER_SIZE];
uint32_t aRxBuffer[BUFFER_SIZE];
FMC_SDRAM_CommandTypeDef command;
uint32_t uwWriteReadStatus = 0;
uint32_t MemoryTestSeed = 1122330000;

// GLOBAL VARS DEFINE ELSEWHERE
// USB CDC VCP
extern USBD_HandleTypeDef hUsbDeviceFS;




void main_Init(void)
{
	// Init Board Status struct
	SysTick_Config(SystemCoreClock/1000);
	BoardStatus.GreenLedMsCounter = 0;
	BoardStatus.RedLedMsCounter = 0;
	BoardStatus.StatusLedMsCounter = 0;
	BoardStatus.MiliSecondCounter = 0;
	BoardStatus.UartCmdChar = 'z';

	// Set GPIO Output init conditions
	STATUS_LED_OFF();
	PWR_5V_OFF();
	WIFI_OFF();
	AUDIO_AMP_OFF();
	VUSB_PWR_OFF();

	// Init Uart 3 for use
	memset(DebugUarOutputtMsg, 0, sizeof(DebugUarOutputtMsg));
	HAL_UART_Receive_IT(&huart3, DebugUartInputMsg, 1);

	// ADC INIT: ADC-DMA-TIMER1
#ifdef ADC_DEFINED_FOR_USE
	memset(ADC_DMA_Buffer, 0, ADC_DMA_SIZE);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_DMA_Buffer, ADC_DMA_SIZE);
	HAL_TIM_Base_Start_IT(&htim1);
#endif

	// INIT SDRAM
	//BSP_SDRAM_Initialization_Sequence(&hsdram1, &command);
	SDRAM_InitSequence();

	// PRINT DEBUG MSG
#ifdef USE_USB_VCP_AS_DEBUG_PORT
	while ((hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED));
#endif
	memset(DebugUarOutputtMsg, 0, sizeof(DebugUarOutputtMsg));
	sprintf((char *)DebugUarOutputtMsg, "\r\n\n\n\n\n\n\n\nHab Technology Demonstrator\r\nDebug Monitor On line...\r\n\n\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	SendDebugUartMsg(DebugUarOutputtMsg);
	SendDebugUartMsg(DebugUarOutputtMsg);
	SendDebugUartMsg(DebugUarOutputtMsg);

} //END OF main_Init




void main_WhileLoop(void)
{

	// TOGGLE LEDS
	// STATUS LED
	if (BoardStatus.StatusLedMsCounter >= 500)
	{
		STATUS_LED_TOGGLE();
		BoardStatus.StatusLedMsCounter = 0;
	}
	// GREEN USER LED
	if (BoardStatus.GreenLedMsCounter >= 500)
	{
		UI_GREEN_LED_TOGGLE();
		BoardStatus.GreenLedMsCounter = 0;
	}
	// RED USER LED
	if (BoardStatus.RedLedMsCounter >= 250)
	{
		UI_RED_LED_TOGGLE();
		BoardStatus.RedLedMsCounter = 0;
	}

	// Write any input from USB CDC to Debug Port
	if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
	{
		uint8_t UsbRxBuffer[100];
		uint16_t UsbRxDataSize = 0;
		if (VCP_retrieveInputData(UsbRxBuffer, &UsbRxDataSize) != 0)
		{
#ifndef USE_USB_VCP_AS_DEBUG_PORT
			SendDebugUartMsg(UsbRxBuffer);
#else
			BoardStatus.UartCmdChar = UsbRxBuffer[0];
#endif
		}


	}

	// Check for commands
	if (BoardStatus.UartCmdChar != 'z')
	{
		switch(BoardStatus.UartCmdChar)
		{
		case '?':
			printHelpMenu();
			break;

		case '0':
			sprintf((char *)DebugUarOutputtMsg, "Hello Hab 0\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;

		case '1':
		{
			sprintf((char *)DebugUarOutputtMsg, "5V PWR: ");
			SendDebugUartMsg(DebugUarOutputtMsg);
			PWR_5V_TOGGLE();
			delayMiliSecond(10); // GIVE IT TIME TO RESPOND
			GPIO_PinState StatusPwr_5V;
			StatusPwr_5V = PWR_5V_STATUS();
			if (StatusPwr_5V == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '2':
		{
			sprintf((char *)DebugUarOutputtMsg, "WIFI IS: ");
			SendDebugUartMsg(DebugUarOutputtMsg);
			WIFI_TOGGLE();
			GPIO_PinState WifiStatus;
			WifiStatus = WIFI_STATUS();
			if (WifiStatus == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '3':
		{
			sprintf((char *)DebugUarOutputtMsg, "AUDIO AMP IS: ");
			SendDebugUartMsg(DebugUarOutputtMsg);
			AUDIO_AMP_TOGGLE();
			GPIO_PinState AudioAmpStatus;
			AudioAmpStatus = AUDIO_AMP_STATUS();
			if (AudioAmpStatus == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '4':
		{
			uint8_t HexSum = 0;
			if (HEX_1_STATUS() == GPIO_PIN_RESET)
				HexSum++;
			if (HEX_2_STATUS() == GPIO_PIN_RESET)
				HexSum += 2;
			if (HEX_4_STATUS() == GPIO_PIN_RESET)
				HexSum += 4;
			if (HEX_8_STATUS() == GPIO_PIN_RESET)
				HexSum += 8;
			sprintf((char *)DebugUarOutputtMsg, "HEX SWITCH: 0x%02X\r\n",HexSum);
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '5':
		{
			// UI ADC
#ifndef ADC_DEFINED_FOR_USE
			sprintf((char *)DebugUarOutputtMsg, "The ADC has been disabled in code\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
#endif
			float ADC_AverageFilterReading;
			float ADC_Voltage;
			float ADC_Percentage;
			float BatteryVoltage;
			ADC_AverageFilterReading = (float)(BoardStatus.ADC_UiReadSum / ADC_SAMPLES_PER_ACQUISION);
			ADC_Voltage = (VADC_REF * ADC_AverageFilterReading/ADC_FullScale());
			ADC_Percentage = (float)(100.0 * ADC_AverageFilterReading / ADC_FullScale());
			sprintf((char *)DebugUarOutputtMsg, "UI ADC: %4.3fV  %2.1f%c\r\n", ADC_Voltage, ADC_Percentage, '%');
			SendDebugUartMsg(DebugUarOutputtMsg);
			// Battery ADC
			ADC_AverageFilterReading = (float)(BoardStatus.ADC_BatReadSum / ADC_SAMPLES_PER_ACQUISION);
			ADC_Voltage = (VADC_REF * ADC_AverageFilterReading/ADC_FullScale());
			BatteryVoltage = ADC_Voltage / BAT_DIVIDER_RATIO;
			sprintf((char *)DebugUarOutputtMsg, "Battery ADC: %4.3fV\r\n", BatteryVoltage);
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '6':
		{
			sprintf((char *)DebugUarOutputtMsg, "BACKLIGHT IS: ");
			SendDebugUartMsg(DebugUarOutputtMsg);
#ifdef BACKLIGHT_AS_OUTPUT
			BACKLIGHT_TOGGLE();
			GPIO_PinState BacklightStatus;
			BacklightStatus = BACKLIGHT_STATUS();
			if (BacklightStatus == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
#else
			static bool_t DisplayOn = FALSE;
			if (DisplayOn)
			{
				BACKLIGHT_OFF();
				DisplayOn = FALSE;
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
			}
			else
			{
				BACKLIGHT_ON();
				DisplayOn = TRUE;
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			}
#endif
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '7':
		{
			sprintf((char *)DebugUarOutputtMsg, "DISPLAY IS: ");
			SendDebugUartMsg(DebugUarOutputtMsg);
			DISPLAY_EN_TOGGLE();
			GPIO_PinState DisplayEnStatus;
			DisplayEnStatus = DISPLAY_EN_STATUS();
			if (DisplayEnStatus == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "ON\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "OFF\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '8':
		{
			sprintf((char *)DebugUarOutputtMsg, "Tech Demo is:");
			SendDebugUartMsg(DebugUarOutputtMsg);
			VUSB_PWR_TOGGLE();
			GPIO_PinState VbusStatus;
			VbusStatus = VUSB_PWR_STATUS();
			if (VbusStatus == GPIO_PIN_SET)
				sprintf((char *)DebugUarOutputtMsg, "Not Powering USB\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "Powering USB\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case '9':
		{
			sprintf((char *)DebugUarOutputtMsg, "VUSB HAS ");
			SendDebugUartMsg(DebugUarOutputtMsg);
			GPIO_PinState SupplyVusbFaultStatus;
			SupplyVusbFaultStatus = SUPPLY_VUSB_FAULT_STATUS();
			if (SupplyVusbFaultStatus == GPIO_PIN_RESET)
				sprintf((char *)DebugUarOutputtMsg, "**FAULT**\r\n");
			else
				sprintf((char *)DebugUarOutputtMsg, "No Fault\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case 'a':
		case 'A':
		{
			char Line1[] = "This is line 1 - OK\r\n";
			char Line2[] = "This is line 2 - OK\r\n";
			char Line3[] = "This is line 3 - OK\r\n";
			UINT BytesWritten;

			// READ FROM DISK
			sprintf((char *)DebugUarOutputtMsg, "Testing uSD Read File: \r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			uint8_t line[100]; /* Line buffer */
			FRESULT fr;     /* FatFs return code */

			/* Gives a work area to the default drive */
			fr = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
			if (fr != FR_OK)
			{
				sprintf((char *)DebugUarOutputtMsg, "ERROR: Drive failed to mount\r\n");
				SendDebugUartMsg(DebugUarOutputtMsg);
				break;
			}
			/* Open a text file */
			fr = f_open(&SDFile, "0:SrcFile.txt", FA_READ);
			if (fr != FR_OK)
			{
				sprintf((char *)DebugUarOutputtMsg, "ERROR: File Read Test: uSD file failed to open\r\n");
				SendDebugUartMsg(DebugUarOutputtMsg);
				f_mount(0,"0:",0);
				break;
			}
			/* Read every line and display it */
			while (f_gets((char *)line, sizeof line, &SDFile))
			{
				SendDebugUartMsg(line);
				sprintf((char *)DebugUarOutputtMsg, "\r");
				SendDebugUartMsg(DebugUarOutputtMsg);
			}

			/* Close the file */
			f_close(&SDFile);

			// WRITE TO FILE
			sprintf((char *)DebugUarOutputtMsg, "\r\nRead PASS OK\r\n\nTesting uSD Write File: \r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			fr = f_open(&SDFile, "0:DstFileA.txt", FA_CREATE_ALWAYS | FA_WRITE);
			if (fr != FR_OK)
			{
				sprintf((char *)DebugUarOutputtMsg, "ERROR: File Write Test: File failed to create\r\n");
				SendDebugUartMsg(DebugUarOutputtMsg);
				break;
			}
			// Write 3 lines only test the first
			f_write(&SDFile, Line1, sizeof(Line1), &BytesWritten);
			if ((BytesWritten == 0) || (BytesWritten < sizeof(Line1)))
			{
				sprintf((char *)DebugUarOutputtMsg, "ERROR: File Write Test: Data failed to write\r\n");
				SendDebugUartMsg(DebugUarOutputtMsg);
				break;
			}
			f_write(&SDFile, Line2, sizeof(Line1), &BytesWritten);
			f_write(&SDFile, Line3, sizeof(Line1), &BytesWritten);
			f_close(&SDFile);
			sprintf((char *)DebugUarOutputtMsg, "PASS OK\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);

			break;
		}

		case 'b':
		case 'B':
		case 'c':
		case 'C':
		{
			// TEST MEMORY BY WRITE / READ / VERIFY
			if ((BoardStatus.UartCmdChar == 'b') || (BoardStatus.UartCmdChar == 'B'))
			{
				sprintf((char *)DebugUarOutputtMsg, "Testing SDRAM By Counting: Write >> Read >> Verify\r\n");
				Fill_Buffer(aTxBuffer, BUFFER_SIZE, MemoryTestSeed);
			}
			else
			{
				sprintf((char *)DebugUarOutputtMsg, "Testing SDRAM By Bit Walking: Write >> Read >> Verify\r\n");
				fillBufferWalkBit(aTxBuffer, BUFFER_SIZE);
			}
			SendDebugUartMsg(DebugUarOutputtMsg);
			// Write data to the SDRAM memory
			for (uint32_t uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
			{
				*(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex) = aTxBuffer[uwIndex];
			}
			// Read back data from the SDRAM memory
			for (uint32_t uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
			{
				aRxBuffer[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex);
			}
			// Checking data integrity #
			for (uint32_t uwIndex = 0; (uwIndex < BUFFER_SIZE); uwIndex++)
			{
				if (aRxBuffer[uwIndex] != aTxBuffer[uwIndex])
				{
					uwWriteReadStatus++;
				}
			}
			if (uwWriteReadStatus != 0)
			{
				sprintf((char *)DebugUarOutputtMsg, "Memory Failures: %d\r\n", (int)uwWriteReadStatus);
				SendDebugUartMsg(DebugUarOutputtMsg);
			}
			else
			{
				sprintf((char *)DebugUarOutputtMsg, "Memory Test Passed\r\n");
				SendDebugUartMsg(DebugUarOutputtMsg);
			}
			break;
		}

		case 'd':
		case 'D':
		{
			sprintf((char *)DebugUarOutputtMsg, "Testing SDRAM 1MB Address Space via Bit Walking: Write >> Read >> Verify\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			// NOTE BUFFER SIZE IS 0x1000
			uint16_t RepeatNumberOfTimes = 0x100000 / BUFFER_SIZE;
			uint32_t Offset = WRITE_READ_ADDR;
			uint32_t DataErrors = 0;
			fillBufferWalkBit(aTxBuffer, BUFFER_SIZE);
			for (uint16_t Repeat = 0; Repeat < RepeatNumberOfTimes; Repeat++)
			{
				sprintf((char *)DebugUarOutputtMsg, "Now at Offset: %d\r\n", (int)Offset);
				SendDebugUartMsg(DebugUarOutputtMsg);
				// Write data to the SDRAM memory
				for (uint32_t uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
				{
					*(__IO uint32_t*) (SDRAM_BANK_ADDR + Offset + (4*uwIndex)) = aTxBuffer[uwIndex];
				}
				// Read data from the SDRAM memory
				for (uint32_t uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
				{
					aRxBuffer[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + Offset + 4*uwIndex);
				}
				// Check for errors
				for (uint32_t uwIndex = 0; (uwIndex < BUFFER_SIZE); uwIndex++)
				{
					if (aRxBuffer[uwIndex] != aTxBuffer[uwIndex])
					{
						DataErrors++;
					}
				}
				Offset += BUFFER_SIZE;
			}
			if (uwWriteReadStatus != 0)
				sprintf((char *)DebugUarOutputtMsg, "Memory Failures: %d\r\n", (int)uwWriteReadStatus);
			else
				sprintf((char *)DebugUarOutputtMsg, "Memory Test Passed\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);

			break;
		}

		case 'e':
		case 'E':
		{
			sprintf((char *)DebugUarOutputtMsg, "Sending test message\r\n");
			SendDebugUartMsg(DebugUarOutputtMsg);
			uint8_t TestMsg[] = "Hello Hab from the virtual comm port\r\n";
			if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
			{
				CDC_Transmit_FS(TestMsg, sizeof(TestMsg));
				sprintf((char *)DebugUarOutputtMsg, "Message Sent\r\n");
			}
			else
			{
				sprintf((char *)DebugUarOutputtMsg, "Virtual Com port not ready or not connected\r\n");
			}
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case 'f':
		case 'F':
		{
			static bool_t DisplayEnable = FALSE;

			if (DisplayEnable)
			{
				DISPLAY_OFF();
				BACKLIGHT_OFF();
				DisplayEnable = FALSE;
				sprintf((char *)DebugUarOutputtMsg, "LCD Display OFF\r\n");
			}
			else
			{
				DISPLAY_ON();
				BACKLIGHT_ON();
				DisplayEnable = TRUE;
				sprintf((char *)DebugUarOutputtMsg, "LCD Display ON\r\n");
			}
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		case 'g':
		case 'G':
		{
			static uint32_t PixelColor = 0x55221155;
			static uint8_t DelayDisplay = 10;
			uint32_t Index = 0;
			uint32_t WIDTH = 480;

			for (uint16_t LineY = 0; LineY < 272; LineY++)
			{
				for (uint16_t LineX = 0; LineX < 480; LineX++)
				{
					*(__IO uint32_t*) (SDRAM_BANK_ADDR + (((LineY * WIDTH) + LineX)*sizeof(uint32_t))) = PixelColor;
					Index++;
				}
				delayMiliSecond(DelayDisplay);
			}

			sprintf((char *)DebugUarOutputtMsg, "Background Color Set");
			SendDebugUartMsg(DebugUarOutputtMsg);
			break;
		}

		default:
			break;
		}
		BoardStatus.UartCmdChar = 'z';
	}



} //END OF main_WhileLoop



void printHelpMenu(void)
{
	sprintf((char *)DebugUarOutputtMsg, "Help Menu:\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "Use '?' to access:\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  0: Say Hello\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  1: Toggle 5V Power\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  2: WIFI Enable Toggle:\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  3: Audio Amp Enable Toggle\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  4: Read Hex Switch\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  5: Read ADC Channels\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  6: Backlight Toggle\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  7: Display Toggle\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  8: VUSB Source Power\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  9: VUSB Fault Read Status\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  A: Micro-SD Test FATFS\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  B: SDRAM Test Counting Number\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  C: SDRAM Test Data Bit Walk\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  D: SDRAM Test 1M of data\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  E: Write test to USB Virtual Com Port\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  F: Toggle LCD\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
	sprintf((char *)DebugUarOutputtMsg, "  G: Change LCD Background color\r\n");
	SendDebugUartMsg(DebugUarOutputtMsg);
}



#define ADD_INCREMENT_TO_MEMORY_TEST
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
#ifdef ADD_INCREMENT_TO_MEMORY_TEST
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
#else
    pBuffer[tmpIndex] = uwOffset;
#endif
  }
}



//
///**
//  * @brief  Perform the SDRAM external memory initialization sequence
//  * @param  hsdram: SDRAM handle
//  * @param  Command: Pointer to SDRAM command structure
//  * @retval None
//  */
//static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
//{
//  __IO uint32_t tmpmrd =0;
//  /* Step 3:  Configure a clock configuration enable command */
//  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
//  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
//  Command->AutoRefreshNumber = 1;
//  Command->ModeRegisterDefinition = 0;
//
//  /* Send the command */
//  if (HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT) != HAL_OK)
//	  Error_Handler();
//
//  /* Step 4: Insert 100 us minimum delay */
//  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
//  HAL_Delay(1);
//
//  /* Step 5: Configure a PALL (precharge all) command */
//  Command->CommandMode = FMC_SDRAM_CMD_PALL;
//  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
//  Command->AutoRefreshNumber = 1;
//  Command->ModeRegisterDefinition = 0;
//
//  /* Send the command */
//  if (HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT) != HAL_OK)
//	  Error_Handler();
//
//  /* Step 6 : Configure a Auto-Refresh command */
//  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
//  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
//  Command->AutoRefreshNumber = 8;
//  Command->ModeRegisterDefinition = 0;
//
//  /* Send the command */
//  if (HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT) != HAL_OK)
//	  Error_Handler();
//
//  /* Step 7: Program the external memory mode register */
//  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
//                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
//                     SDRAM_MODEREG_CAS_LATENCY_2           |
//                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
//                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
//
//  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
//  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
//  Command->AutoRefreshNumber = 1;
//  Command->ModeRegisterDefinition = tmpmrd;
//
//  /* Send the command */
//  if (HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT) != HAL_OK)
//	  Error_Handler();
//
//  /* Step 8: Set the refresh rate counter */
//  /* (15.62 us x Freq) - 20 */
//  /* Set the device refresh counter */
//  hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));
//
//}


//static void CPU_CACHE_Enable(void)
//{
//  /* Enable I-Cache */
//  SCB_EnableICache();
//
//  /* Enable D-Cache */
//  SCB_EnableDCache();
//}




#define REFRESH_COUNT 1262
void SDRAM_InitSequence(void)
{

	FMC_SDRAM_CommandTypeDef FMC_SDRAMCommandStructure;
	uint32_t tmpr = 0;

	/* Step 1 --------------------------------------------------------------------*/
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 2 --------------------------------------------------------------------*/
	/* Configure a PALL (precharge all) command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_PALL;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 3 --------------------------------------------------------------------*/
	/* Configure a Auto-Refresh command */
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 4; // ******HAB THERE IS A DIFFERENCE HERE *******
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 4 --------------------------------------------------------------------*/
	/* Program the external memory mode register */
	tmpr = (uint32_t)	SDRAM_MODEREG_BURST_LENGTH_1     |
					SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL  |
					SDRAM_MODEREG_CAS_LATENCY_3      |   // ******HAB THERE IS A DIFFERENCE HERE *******
					SDRAM_MODEREG_OPERATING_MODE_STANDARD |
					SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* Step 5 --------------------------------------------------------------------*/
	/* Configure a load Mode register command*/
	FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.ModeRegisterDefinition = tmpr;
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	if (HAL_SDRAM_SendCommand(&hsdram1, &FMC_SDRAMCommandStructure, SDRAM_TIMEOUT) != HAL_OK)
		Error_Handler();

	/* Step 6 --------------------------------------------------------------------*/
	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 */
	/* Set the device refresh counter */
	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before sending command
	HAL_SDRAM_ProgramRefreshRate(&hsdram1, REFRESH_COUNT); // local

	while( (HAL_SDRAM_GetState(&hsdram1) & HAL_SDRAM_STATE_BUSY) );	// Check Busy Flag before returning
}



void fillBufferWalkBit(uint32_t *Buffer, uint32_t SizeOfBuffer)
{
	uint32_t Data = 0x00000000;

	for (uint32_t Index = 0; Index < SizeOfBuffer; Index++)
	{
		Buffer[Index] = Data;
		if (Data == 0)
			Data = 1;
		else
			Data <<= 1;
		if (Data == 0xFFFFFFFF)
			Data = 0;
	}

}
