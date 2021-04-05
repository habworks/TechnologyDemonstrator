/** ****************************************************************************************************
 * @file 			HAB_WAV_AUDIO_TASKS.c
 * @brief			This is the Source file used to support tasks associated with playing WAV audio files
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			4/3/21 \n
 * @version       	2.0
 *
 * @param Development_Environment \n
 * Hardware:		Agnostic
 * IDE:             Agnostic
 * Compiler:        Agnostic
 *
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            This is an embedded algorithm for play .wav audio files from disk
 *                  The application plays a 16bit PCM wave audio file
 *
 * 					The application is designed to be hardware and compiler agnostic though it requires the
 * 					use of:
 * 						At least one DAC (12 or 16 bit - must be the same) (Controller Hardware peripheral)
 * 						One timer peripheral (Controller Hardware peripheral)
 * 						Chan FATFS FAT file system (Middle-wear firmware)
 * 					Users of this algorithm will call only the base function call_play16Bit_WAVE()
 * 					The base function requires supporting functions which are hardware dependent.  Those functions are
 * 					defined weak here and left for the unique user hardware implementation.
 * 					For more details consult function header notes
 * 					For reference on .WAV files:
 * 						http://www.topherlee.com/software/pcm-tut-wavformat.html
 * 						https://en.wikipedia.org/wiki/WAV
 *
 * @copyright		IMR Engineering
 *   			    This algorithm was developed may not be reused without the express consent of IMR Engineering
 * **************************************************************************************************** */

// INCLUDES
#include <HAB_WAV_AUDIO_TASKS.H>
#include "fatfs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// FUNCTION SUPPORT INCLUDES
#include "SupportIO.h"  // TODO Hab remove testing


// GLOBALS
Type_CircularBuffer CircularBufferLeft, CircularBufferRight;
volatile uint8_t AudioChannel;
volatile bool PlayLeftChannel = false;
volatile bool AudioPlaying = false;
#ifdef DISPLAY_AUDIO_LEVEL_EXAMPLE
volatile uint8_t Last_LED_BarGraphState = 0;
#endif



/*******************************************************************************************************
* @brief  This function accepts a pointer to Type_AudioQueueStruct.  That struct contains the name of the
* 16bit .WAV file that is to be played.  The function does not handle a BitPerSample other than 16 which is
* the most common.  It can however accept 1 or 2 channels and varying Bytes Per Second.  It returns the
* status of its actions based on a status of Type_RetrunStatus.  The function opens and reads the file from
* the SD card, so it incorporates FAT FS features for reading a file.  The file is read, parsed for its parameters,
* the Play Back Rate is calculated, and the associated timer IRQ is set to match the Play Back Rate.  Two circular
* buffers are created (left / ight {if necessary}).  The circular (L/R) buffers are loaded from the file open Buffer....
* then the data stream is extracted (R/L) and scaled to the DAC (10 or 12 bit) for audio out. This function will
* perform the repeated routines necessary to support task of reading and filling the circular buffer.  The
* associated Timer IRQ Handler performs the actual playback function to DAC at the designated playback rate.
*
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note: File requires the calling function to have use Chan FATFS File System
* - The drive must be mounted before this function is called
* - The necessary and associated hardware peripherals should be dedicated for this function use
* - As this algorithm is hardware agnostic to the platform the user must provide the necessary functions
* - All needed functions are defined weak here for redefinition with user specific application
* - THIS AUDIO IS BEST IMPLEMENTED IF CALLED FROM A MSG QUEUE - OTHERWISE DO NOT CALL AGAIN UNTIL PLAY HAS STOPPED
* - Function works with the associate Timer IRQ handler for playback
* - Onlu supported format .WAV 16bit PCM
*
* @param Pointer to the AudioPlayBack struct
* @return Status that indicates OK or specific error condition
*
* STEP 1: Set the init conditions for playing audio
* STEP 2: Open file for reading and allocate the necessary memory to start
* STEP 3: Read a large block of the file to get started with processing - Continue to read as data is processed
* STEP 4: For first read of data extract WAV file parameters and verify the file type as WAV 16bit - Create Circular Buffers and calculate and set playback rate
* STEP 5: Read and fill Circular Buffers until present read block has been fully parsed to left / right CB channels
* STEP 6: Read and store to buffer Left Audio channel - Left right bytes are interleave
* STEP 7: Read and store to buffer Right Audio channel - Left right bytes are interleave
* STEP 8: Check for play or pause conditions
* STEP 9: Check for an end condition - all data from file has been read - close everything
****************************************************************************************************** */
Type_RetrunStatus call_play16Bit_WAVE(Type_AudioPlayBack *AudioPlayBack)
{

	FIL FileStream;
	UINT BytesRead;
	FRESULT FF_Result;
	BYTE *Buffer;
	bool FileVerified = false, FirstRead = true;
	char FullFileName[MAX_LENGTH_WAV_FILE];
	uint8_t FileType[sizeof(RIFF_FILE_TYPE) + 1];  	// ONE ADDITIONAL ELEMENT ADDED FOR SAFETY
	uint8_t RiffType[sizeof(WAVE_RIFF_TYPE) + 1]; 	// ONE ADDITIONAL ELEMENT ADDED FOR SAFETY
	uint8_t *ptr_ToRiffOffset, *ptr_ToBufferData;
	uint16_t DAC_Value;
	uint32_t AudioPlayBackRate;
	Union_AudioValue AudioValue;
	Union_DataChunkSize DataChunkSize;
	Union_BytesPerSecond BytesPerSecond;

	// STEP 1: Set the init conditions for playing audio
	AudioPlayBack->Playing = false;
	writeDAC_LeftChannel(0);
	writeDAC_RightChannel(0);
	stopAudioTimer();
	PlayLeftChannel = true;

	// STEP 2: Open file for reading and allocate the necessary memory to start
	// BUILD THE FILE NAME WITH PATH
	memset(FullFileName, '\0', sizeof(FullFileName));
	strcpy(FullFileName, AudioPlayBack->FileNameAndPath);
	FF_Result = f_open(&FileStream, FullFileName, FA_OPEN_EXISTING | FA_READ);
	if (FF_Result != FR_OK)
	{
		return(FILE_OPEN_ERROR);
	}
	// ALLOCATE MEMORY
	Buffer = (BYTE *)memoryAllocate(IN_COMMING_BUFFER_SIZE, sizeof(BYTE));
	if (Buffer ==  NULL)
	{
		f_close(&FileStream);
		return(MEMORY_ALLOCATION_ERROR);
	}

	// STEP 3: Read a large block of the file to get started with processing - Continue to read as data is processed
	while (true)
	{
		// READ A CHUNK OF THE WAV FILE
		FF_Result = f_read(&FileStream, Buffer, IN_COMMING_BUFFER_SIZE, &BytesRead);
		if (FF_Result != FR_OK)
		{
			f_close(&FileStream);
			memoryFree(Buffer);
			return(FILE_READ_ERROR);
		}
		if (BytesRead == 0)
		{
			break;
		}

		// STEP 4: For first read of data extract WAV file parameters and verify the file type as WAV 16bit - Create Circular Buffers and calculate and set playback rate
		if (!FileVerified)
		{
			// VERIFY THE FILE TYPE AS VALID - GET FILE TYPE AND RIFF TYPE
			strncpy((char *)FileType, (char *)Buffer, 4);
			FileType[4] = STRING_NULL;  // TERMINATE THE STRING
			ptr_ToRiffOffset = Buffer + RIFF_TYPE_OFFSET;
			strncpy((char *)RiffType, (char *)ptr_ToRiffOffset, 4);
			RiffType[4] = STRING_NULL;  // TERMINATE THE STRING
			uint8_t StringCompareTotal = strcmp(RIFF_FILE_TYPE, (char *)FileType) + strcmp(WAVE_RIFF_TYPE, (char *)RiffType);
			if ((StringCompareTotal != 0) || (Buffer[FORMAT_SIZE_OFFSET] != 16) || (Buffer[BIT_PER_SAMPLE_OFFSET] != 16))
			{
				f_close(&FileStream);
				memoryFree(Buffer);
				return (FILE_VERIFICATION_ERROR);
			}
			else
			{
				// FILE VERIFIED - LOAD WAVE FILE PARAMETERS
				FileVerified = true;
				// LOAD BYTE PER SECOND RATE
				BytesPerSecond.ByteValue[0] = Buffer[BYTE_RATE_OFFSET];
				BytesPerSecond.ByteValue[1] = Buffer[BYTE_RATE_OFFSET + 1];
				BytesPerSecond.ByteValue[2] = Buffer[BYTE_RATE_OFFSET + 2];
				BytesPerSecond.ByteValue[3] = Buffer[BYTE_RATE_OFFSET + 3];
				// LOAD DATA CHUNCK SIZE
				DataChunkSize.ByteValue[0] = Buffer[DATA_SIZE_OFFSET];
				DataChunkSize.ByteValue[1] = Buffer[DATA_SIZE_OFFSET + 1];
				DataChunkSize.ByteValue[2] = Buffer[DATA_SIZE_OFFSET + 2];
				DataChunkSize.ByteValue[3] = Buffer[DATA_SIZE_OFFSET + 3];
				// LOAD THE NUMBER OF AUDIO CHANNELS AND PLAYBACK INFORMAITON
				AudioChannel = Buffer[CHANNEL_NUMBER_OFFSET];
				AudioPlayBack->NumberOfChannels = AudioChannel;
				AudioPlayBack->PlayTimeInSeconds = (uint32_t) ((AudioChannel * DataChunkSize.Int32Value) / BytesPerSecond.Int32Value);
				AudioPlayBack->TimeRemainingInMinutes = (AudioPlayBack->PlayTimeInSeconds / 60);
				AudioPlayBack->TimeRemainingInSeconds = AudioPlayBack->PlayTimeInSeconds - (AudioPlayBack->TimeRemainingInMinutes * 60);
				// CREATE CIRCULAR BUFFERS - THERE IS ALWAYS A LEFT CHANNEL / RIGHT CHANNEL IF STERO
				init_CB(&CircularBufferLeft, CIRCULAR_BUFFER_SIZE, sizeof(uint16_t));
				if (AudioChannel == STEREO)
					init_CB(&CircularBufferRight, CIRCULAR_BUFFER_SIZE, sizeof(uint16_t));
				// SET THE TIMER AT THE AUDIO PLAY BACK RATE
				AudioPlayBackRate = (Buffer[CHANNEL_NUMBER_OFFSET] * BytesPerSecond.Int32Value) / (Buffer[CHANNEL_NUMBER_OFFSET] * 2); // PLAYBACK RATE IN BYTES PER SECOND FOR THE NUMBER OF CHANNELS
				// SET THE TIMER IRQ INTERVAL FOR PLAYBACK RATE
				if (!init_WavAudioTimer(AudioPlayBackRate))
				{
					f_close(&FileStream);
					memoryFree(Buffer);
					return(TIMER_FAIL_INIT_ERROR);
				}
				// ADJUST BYTES READ TO ACCOUNT FOR WAVE FILE HEADER FOR THIS FIRST PASS
				BytesRead -= DATA_OFFSET;
				// TURN ON AUDIO OUTPUT AND SET CONDITON TO PLAYING
				enableAudioOutput();
				delayInMiliseonds(TIME_TO_AUDIO_OUTPUT_STABLE);
				startAudioTimer();
				AudioPlayBack->Playing = true;
				AudioPlaying = true;
			}
		} // WAVE FILE PARAMETERS READ AND CONDITIONS READY TO BEGIN PLAING DATA

		// STEP 5: Read and fill Circular Buffers until present read block has been fully parsed to left / right CB channels
		// SET POINTER TO DATA LOCATION FOR THE FIRST READ - THE POINTER WILL BE UPDATED FORWARD OF THIS
		if (FirstRead)
		{
			ptr_ToBufferData = Buffer + DATA_OFFSET;
			FirstRead = false;
		}
		else
		{
			ptr_ToBufferData = Buffer;
		}

		// STEP 6: Read and store to buffer Left Audio channel - Left right bytes are interleave
		// LEFT AUDIO: BYTES ARE READ AS 16BIT (2 BYTES) IN LITTLE ENDIAN
		do
		{
			AudioValue.ByteValue[LOW_BYTE] = *ptr_ToBufferData;
			ptr_ToBufferData++;
			DataChunkSize.Int32Value--;
			BytesRead--;
			AudioValue.ByteValue[HIGH_BYTE] = *ptr_ToBufferData;
			ptr_ToBufferData++;
			DataChunkSize.Int32Value--;
			BytesRead--;
			// LOAD THE CIRCULAR BUFFER - WAIT IF FULL TO CONTINUE LOADING
			if ((AudioChannel == LEFT) || (AudioChannel == STEREO))
			{
#ifdef DAC_TYPE_10_BIT
				DAC_Value = call_S16Bit_To_10Bit(AudioValue.Signed16Bit_Value);
#endif
#ifdef DAC_TYPE_12_BIT
				DAC_Value = call_S16Bit_To_12Bit(AudioValue.Signed16Bit_Value);
#endif
				while (isFull_CB(&CircularBufferLeft));  // TODO Hab not thread safe - fix later
				//stopAudioTimer();
				write_CB(&CircularBufferLeft, &DAC_Value);
				//startAudioTimer();
			}

			// STEP 7: Read and store to buffer Right Audio channel - Left right bytes are interleave
			// RIGHT AUDIO: BYTES ARE READ AS 16BIT (2 BYTES) IN LITTLE ENDIAN
			if (AudioChannel == STEREO)
			{
				AudioValue.ByteValue[LOW_BYTE] = *ptr_ToBufferData;
				ptr_ToBufferData++;
				DataChunkSize.Int32Value--;
				BytesRead--;
				AudioValue.ByteValue[HIGH_BYTE] = *ptr_ToBufferData;
				ptr_ToBufferData++;
				DataChunkSize.Int32Value--;
				BytesRead--;
				// LOAD THE CIRCULAR BUFFER - WAIT IF FULL TO CONTINUE LOADING
#ifdef DAC_TYPE_10_BIT
				DAC_Value = call_S16Bit_To_10Bit(AudioValue.Signed16Bit_Value);
#endif
#ifdef DAC_TYPE_12_BIT
				DAC_Value = call_S16Bit_To_12Bit(AudioValue.Signed16Bit_Value);
#endif
				while (isFull_CB(&CircularBufferRight));	// TODO Hab not thread safe - fix later
				//stopAudioTimer();
				write_CB(&CircularBufferRight, &DAC_Value);
				//startAudioTimer();
			}
		} while ((DataChunkSize.Int32Value != 0) && (BytesRead != 0));

		// STEP 8: Check for play or pause conditions
		// STOP CONDITION
		if (!AudioPlayBack->Play)
		{
			stopActionsMusicPlaying();
			DataChunkSize.Int32Value = 0; // THIS WILL FORCE STOP PLAY AND CREATE AN END CONDITION
		}
		// CHECK FOR PAUSE - IF SO LED BAR GRAPH OFF AND WAIT TO PROCCEED
		while ((AudioPlayBack->Play) && (AudioPlayBack->Pause))
		{
			AudioPlaying = false;
			pauseActionsMusicPlaying();
			yieldControlToAnotherTask();
		}

		// STEP 9: Check for an end condition - all data from file has been read - close everything
		if (DataChunkSize.Int32Value == 0)
		{
			// FREE MEMORY
			memoryFree(Buffer);
			// WAIT FOR THE BUFFERS TO BE EMPTY: FREE MEMORY AND DISABLE TIMER IRQ
			while (!isEmpty_CB(&CircularBufferLeft)); // TODO Hab not thread safe - fix later
			free_CB(&CircularBufferLeft);
			if (AudioChannel == STEREO)
			{
				while (!isEmpty_CB(&CircularBufferRight));
				free_CB(&CircularBufferRight);
			}
			// SET CONDITIONS FOR EXITING CLEAN: Close file, Stop audio timer, Disable Output Driver, hault any userservices
			f_close(&FileStream);
			stopAudioTimer();
			disableAudioOutput();
			AudioPlayBack->Playing = false;
			AudioPlayBack->Pause = false;
			AudioPlayBack->Play = false;
			AudioPlaying = false;
			stopActionsMusicPlaying();
			break;
		}
	} // END OF WHILE

	// IF YOU GET TO THIS - THE FILE FINISHED PLAY OR WAS STOPPED - ALL IS GOOD
	return (STATUS_OK);

} // END OF call_play16Bit_WAVE



/*******************************************************************************************************
* @brief  ISR Handler for Timer associated with audio playback. This is the peripheral Timer for the Audio play back.
* It works with the function call_play16Bit_WAVE.  The ISR reads the value from one of two circular buffers
* (L/R) and outputs PCM Audio to the (singular) DAC.  The ISR is enabled, its timer set and disabled by
* call_play16Bit_WAVE.  PlayLeftChannel is set true by the end of call_play16Bit_WAVE such that this IRQ
* will always be in-sync to play the left channel first.
*
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note: If stereo, the channels alternate between left and right to the single DAC
* - Function works with the associate Timer IRQ handler for playback
* - Onlu supported format .WAV 16bit PCM
*
* @param void
* @return void
*
* STEP 1: Output data from Left / Right channel and load Left / Right channel DAC
* STEP 2: Alternate Audio Channel for playback on left right channel
****************************************************************************************************** */
void audioTimerIrqHandler(void)
{
	uint16_t PlayValue;

	// STEP 1: Output data from Left / Right channel and load Left / Right channel DAC
	// READ VALUE FROM ONE OF CIRCULAR BUFFERS AND OUTPUT TO AUDIO
	if ((!isEmpty_CB(&CircularBufferLeft)) && (PlayLeftChannel) && (AudioPlaying))
	{
		STATUS_MOD_IO0_TOGGLE();  // TODO Hab remove testing
		read_CB(&CircularBufferLeft, &PlayValue);
#ifdef DAC_TYPE_10_BIT
		PlayValue &= 0x03FF; // THE DAC VALUE IS 10BIT - JUST BEING SAFE
#endif
#ifdef DAC_TYPE_12_BIT
		PlayValue &= 0x0FFF;
#endif
		writeDAC_LeftChannel(PlayValue);
	}
	if ((!isEmpty_CB(&CircularBufferRight)) && (AudioChannel == STEREO) && (!PlayLeftChannel) && (AudioPlaying))
	{
		STATUS_MOD_IO1_TOGGLE();	// TODO Hab remove testing
		read_CB(&CircularBufferRight, &PlayValue);
#ifdef DAC_TYPE_10_BIT
		PlayValue &= 0x03FF; // THE DAC VALUE IS 10BIT - JUST BEING SAFE
#endif
#ifdef DAC_TYPE_12_BIT
		PlayValue &= 0x0FFF; // THE DAC VALUE IS 10BIT - JUST BEING SAFE
#endif
		writeDAC_RightChannel(PlayValue); // FOR true STERO WOULD OUTPUT TO 2ND AUDIO DAC
	}

	// STEP 2: Alternate Audio Channel for playback on left right channel
	// ALTERNATE AUDIO CHANNEL IF IN STERO
	if (AudioChannel == STEREO)
	{
		if (PlayLeftChannel)
			PlayLeftChannel = false;
		else
			PlayLeftChannel = true;
	}
} // END audioTimerIrqHandler



/*******************************************************************************************************
* @brief Accepts 16bit Signed PCM Audio data.  Scales and converts that data to a 10bit unsigned value.
* The high end is represented by 1024 the mid point by 512 and the low end (the negative) as 0.  This way the
* audio is centered about the mid point of the DAC output.  The returned value is 10 bits in a uint16_t
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This is a 10Bit DAC
*
* @param Signed 16 bit audio from WAV file
* @return Unsigned 16 bit audio offset to mid point
*
* STEP 1: Offset and scale
****************************************************************************************************** */
uint16_t call_S16Bit_To_10Bit(int16_t Signed16BitValue)
{

	uint16_t ConvertedValue;

	// STEP 1: Offset and scale
	ConvertedValue = (uint16_t) ((Signed16BitValue + 32768) / 64);
	return (ConvertedValue);

} // END OF call_S16Bit_To_10Bit



/*******************************************************************************************************
* @brief Accepts 16bit Signed PCM Audio data.  Scales and converts that data to a 12bit unsigned value.
* The high end is represented by 1024 the mid point by 512 and the low end (the negative) as 0.  This way the
* audio is centered about the mid point of the DAC output.  The returned value is 10 bits in a uint16_t
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This is a 12Bit DAC
*
* @param Signed 16 bit audio from WAV file
* @return Unsigned 16 bit audio offset to mid point
*
* STEP 1: Offset and scale
****************************************************************************************************** */
uint16_t call_S16Bit_To_12Bit(int16_t Signed16BitValue)
{

	uint16_t ConvertedValue;

	// STEP 1: Offset and scale
	ConvertedValue = (uint16_t) ((Signed16BitValue + 32768) / 16);
	return (ConvertedValue);

} // END OF call_S16Bit_To_12Bit



void updateMusicTime(void)
{
	REDEFINE_SUPPRESS_WARNING(updateMusicTime);
} // END OF updateMusicTime



/*******************************************************************************************************
* @brief Init of Circular Buffer - Sets start and end to index 0 (same location) and allocates memory for
* use as the buffer size + 1 (based on type).  The function accepts a pointer to the Circular Buffer type
* and size.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Circular Buffer
*
* @param Pointer to circular buffer, Size of Buffer to allocate, Type to allocate
* @return True or False based on memory allocation status
*
 * STEP 1: Set start and end of buffer to 0 - set size 
 * STEP 2: Allocate memory
****************************************************************************************************** */
bool init_CB(Type_CircularBuffer *CircularBuffer, uint16_t BufferSize, uint8_t NumberOfBytesInType)
{
	// STEP 1: Set start and end of buffer to 0 - set size
	CircularBuffer->Size = BufferSize + 1; // INCLUDES AND EMPTY LOCATION
	CircularBuffer->Start = 0;
	CircularBuffer->End = 0;

	// STEP 2: Allocate memory
	CircularBuffer->Elems = (uint16_t *) memoryAllocate(NumberOfBytesInType * CircularBuffer->Size, sizeof(int16_t));
	if (CircularBuffer->Elems ==  NULL)
		return(false);
	else
		return(true);

} // END OF init_CB



/*******************************************************************************************************
* @brief The Circular Buffer allocates memory as needed.  When you are done with the Circular Buffer the
* memory can be returned to the stack.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Circular Buffer
*
* @param Pointer to circular buffer
* @return void
*
 * STEP 1: Return memory to heap
****************************************************************************************************** */
void free_CB(Type_CircularBuffer *CircularBuffer)
{

	// STEP 1: Return memory to heap
	// OK IF NULL
	memoryFree(CircularBuffer->Elems);

} // END OF free_CB



/*******************************************************************************************************
* @brief Checks if the Circular Buffer is full.  Accepts a pointer to the Circular Buffer returns true if
* full, else returns false
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Circular Buffer
*
* @param Pointer to circular buffer
* @return True or False
*
 * STEP 1: Check if full
****************************************************************************************************** */
uint8_t isFull_CB(Type_CircularBuffer *CircularBuffer)
{

	// STEP 1: Check if full
	return ((CircularBuffer->End + 1) % CircularBuffer->Size == CircularBuffer->Start);

} // END OF isFull_CB



/*******************************************************************************************************
* @brief Checks if the Circular Buffer is empty.  Accepts a pointer to  the Circular Buffer returns true if
* empty, else returns false
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Circular Buffer
*
* @param Pointer to circular buffer
* @return True or False
*
 * STEP 1: Check if empty
****************************************************************************************************** */
uint8_t isEmpty_CB(Type_CircularBuffer *CircularBuffer)
{
	// STEP 1: Check if empty
	return (CircularBuffer->End == CircularBuffer->Start);

} // END OF isEmpty_CB



/*******************************************************************************************************
* @brief Writes a single element to the Circular Buffer.  The data is written to the end (oldest) element
* in the Circular Buffer array.  It is overwritten if the buffer is full.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Circular Buffer
* - Application can choose to avoid over writing the buffer by checking first to see if it is full isFull_CB.
* - Element and Circular Buffer elems type must be the same type
*
* @param Pointer to circular buffer, and element to be added
* @return void
*
 * STEP 1: Store element
 * STEP 2: Advance the end storage location
****************************************************************************************************** */
void write_CB(Type_CircularBuffer *CircularBuffer, uint16_t *Element)
{

	// STEP 1: Store element
	CircularBuffer->Elems[CircularBuffer->End] = *Element;

	// STEP 2: Advance the end storage location
	CircularBuffer->End = (CircularBuffer->End + 1) % CircularBuffer->Size;
	if (CircularBuffer->End == CircularBuffer->Start)
		CircularBuffer->Start = (CircularBuffer->Start + 1) % CircularBuffer->Size; /* full, overwrite */

} // END OF write_CB



/*******************************************************************************************************
* @brief Reads a single element from the Circular Buffer.  The data is passed by reference to the var Element.
* The data is read from the start location.  After reading the start location is advanced to the next location.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Application must first ensure the Circular Buffer is not empty before reading isEmpty_CB
* - Element and Circular Buffer elems type must be the same type
*
* @param Pointer to circular buffer, and pointer to return element
* @return void
*
 * STEP 1: Store element
 * STEP 2: Advance the end storage location
****************************************************************************************************** */
void read_CB(Type_CircularBuffer *Type_CircularBuffer, uint16_t *Element)
{

	// STEP 1: Recall element
	*Element = Type_CircularBuffer->Elems[Type_CircularBuffer->Start];

	// STEP 2: Advance the start storage location
	Type_CircularBuffer->Start = (Type_CircularBuffer->Start + 1) % Type_CircularBuffer->Size;

} // END OF read_CB





/***********************************************************************************************************************/
// ******************************* USER PROVIDED REPLACMENT - FUNCTIONS MUST BE REPLACED *******************************
/***********************************************************************************************************************/

/*******************************************************************************************************
* @brief OS implementation of the allocation of memory.  For a bear metal application the suggestion would
* be to use calloc
*
* @date				4/5/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Memory allocation is typically OS dependent if bear metal use calloc
*
*
* @param Number of Elements, Size of the individual element in bytes
* @return void pointer
*
* STEP 1: User steps to allocate memory
****************************************************************************************************** */
__weak void * memoryAllocate(size_t NumberOfElements, size_t SizeOfElement)
{
	REDEFINE_SUPPRESS_WARNING(memoryAllocate);
	return(NULL);
} // END OF memoryAllocate



/*******************************************************************************************************
* @brief OS implementation of the freeing of allocated  memory.  For a bear metal application the suggestion would
* be to use free
*
* @date				4/5/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Memory allocation / dealocaiton is typically OS dependent if bear metal use free
*
*
* @param Pointer to Memory
* @return void
*
* STEP 1: User steps to allocate memory
****************************************************************************************************** */
__weak void memoryFree(void *PointerToMemory)
{
	REDEFINE_SUPPRESS_WARNING(memoryFree);
} // END OF memoryFree



/*******************************************************************************************************
* @brief Init of the Timer dedicated for use with Hab WAV Audio API.  This function must init a timer
* IRQ for use.  The timer must IRQ at the period of the InteruptFrequency that is passed.  This function
* should only init the timer, not start it.  Starting and stopping of the timer IRQ is done elsewhere. The
* IRQ handler must call the Hab WAV API function audoTimerIrqHandler()
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Timer IRQ callback function must call audoTimerIrqHandler()
*
*
* @param Interrupt Frequency
* @return True or False on success of timer IRQ init
*
* STEP 1: User steps to init the IRQ timer
****************************************************************************************************** */
__weak bool init_WavAudioTimer(uint32_t InteruptFrequency)
{
	REDEFINE_SUPPRESS_WARNING(init_WavAudioTimer);
	return(false);
} // END OF init_WavAudioTimer



/*******************************************************************************************************
* @brief Disable audio output driver.  This is used to disable audio output driver.  It is only necessary to
* conserver power - user need not interface if the intent is to have the audio driver always on.  Otherwise
* use this function to enable audio output driver
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Recommended for embedded applications that need to conserve power
* - Not implemented here.  User must redefine to hardware and OS specific platform
*
* @param void
* @return void
*
* STEP 1: User steps to disable the output audio driver
****************************************************************************************************** */
__weak void disableAudioOutput(void)
{
	REDEFINE_SUPPRESS_WARNING(disableAudioOutput);
} // END OF disableAudioOutput



/*******************************************************************************************************
* @brief Enable audio output driver.  This is used to enable audio output driver.  It is only necessary to
* conserver power - user need not interface if the intent is to have the audio driver always on.  Otherwise
* use this function to disable audio output driver
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Recommended for embedded applications that need to conserve power
* - Not implemented here.  User must redefine to hardware and OS specific platform
*
* @param void
* @return void
*
* STEP 1: User steps to enable the output audio driver
****************************************************************************************************** */
__weak void enableAudioOutput(void)
{
	REDEFINE_SUPPRESS_WARNING(enableAudioOutput);
} // END OF enableAudioOutput



/*******************************************************************************************************
* @brief Write audio output to DAC Left channel.  User must implement a function that writes a value to
* the DAC output.  The reference voltage of the DAC is only important to driving the output audio - what is
* important here is the value of the DAC output which is a parameter.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note The correct DAC resolution 10b or 12b must be defined in HAB_WAV_AUDIO_TASKS.H
* - Do nothing to scale this value - it may be passed directly to the audio driver
* - There is always a left channel - not always a right
*
* @param DAC value for left channel audio
* @return void
*
* STEP 1: User steps to write to Left DAC Channel
****************************************************************************************************** */
__weak void writeDAC_LeftChannel(uint16_t DAC_Value)
{
	REDEFINE_SUPPRESS_WARNING(writeDAC_LeftChannel);
} // END OF writeDAC_LeftChannel



/*******************************************************************************************************
* @brief Write audio output to DAC Right channel.  User must implement a function that writes a value to
* the DAC output.  The reference voltage of the DAC is only important to driving the output audio - what is
* important here is the value of the DAC output which is a parameter.  Note if no second DAC is implemented
* for right channel audio - it is recommended that user calls the same function that writes to the left audio.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note The correct DAC resolution 10b or 12b must be defined in HAB_WAV_AUDIO_TASKS.H
* - Do nothing to scale this value - it may be passed directly to the audio driver
* - There is always a left channel - not always a right
*
* @param DAC value for left channel audio
* @return void
*
* STEP 1: User steps to write to Right DAC Channel
****************************************************************************************************** */
__weak void writeDAC_RightChannel(uint16_t DAC_Value)
{
	REDEFINE_SUPPRESS_WARNING(writeDAC_RightChannel);
}  // END OF writeDAC_RightChannel



/*******************************************************************************************************
* @brief Start the Audio Timer IRQ ticking
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This function is only intended to start the audio timer IRQ.  The Timer should have been previously setup
* - This is the timer interrupt that is associated with the audio timer
*
* @param void
* @return void
*
* STEP 1: User steps to start the timer associated with the timer IRQ
****************************************************************************************************** */
__weak void startAudioTimer(void)
{
	REDEFINE_SUPPRESS_WARNING(startAudioTimer);
} // END OF startAudioTimer



/*******************************************************************************************************
* @brief Stop the Audio Timer IRQ ticking
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This function is only intended to stop not de_init the audio timer IRQ.  After a stop has been
* issued a start should be able to restart the timer IRQ without an init
* @param void
* @return void
*
* STEP 1: User steps to start the timer associated with the timer IRQ
****************************************************************************************************** */
__weak void stopAudioTimer(void)
{
	REDEFINE_SUPPRESS_WARNING(stopAudioTimer);
} // END OF stopAudioTimer



/*******************************************************************************************************
* @brief A delay in mili-seconds.  It is important that should Hab Wav Audio be used in an RTOS that the
* delay implemented here is non-blocking.  Using a blocking function in an RTOS is typically non-thread safe
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note Function should not block if this API is used within an RTOS
*
* @param The nuumber of mili-seconds to delay
* @return void
*
* STEP 1: User steps to implement a mili-second delay
****************************************************************************************************** */
__weak void delayInMiliseonds(uint32_t DelayTime)
{
	REDEFINE_SUPPRESS_WARNING(delayInMiliseonds);
} // END OF delayInMiliseonds



/***********************************************************************************************************************/
// ******************************* USER OPTIONAL FUNCTIONS - FUNCTIONS MUST BE REPLACED *******************************
/***********************************************************************************************************************/

/*******************************************************************************************************
* @brief Use this function to set the actions necessary when the music playing has been stopped.  This is
* totally at the user discretion and is not necessary for the implementation of Hab Wav Audio
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note User specific implementation
*
* @param void
* @return void
*
* STEP 1: User steps when audio has stopped
****************************************************************************************************** */
__weak void stopActionsMusicPlaying(void)
{
	REDEFINE_SUPPRESS_WARNING(stopActionsMusicPlaying);
} // END OF stoptActionsMusicPlaying



/*******************************************************************************************************
* @brief Use this function to set the actions necessary when the music playing has been paused.  This is
* totally at the user discretion and is not necessary for the implementation of Hab Wav Audio
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note User specific implementation
*
* @param void
* @return void
*
* STEP 1: User steps when audio has paused
****************************************************************************************************** */
__weak void pauseActionsMusicPlaying(void)
{
	REDEFINE_SUPPRESS_WARNING(pauseActionsMusicPlaying);
} // END OF pauseActionsMusicPlaying


/*******************************************************************************************************
* @brief For use in RTOS applications - used when audio has been paused.  Call this function to yield
* priority to the RTOS for a time (user determined) but a few milliseconds is suggested
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note For use in RTOS based applications only
*
* @param void
* @return void
*
* STEP 1: User steps to yield control to the RTOS when in pause condition
****************************************************************************************************** */
__weak void yieldControlToAnotherTask(void)
{
	REDEFINE_SUPPRESS_WARNING(yieldControlToAnotherTask);
} // END OF yieldControlToAnotherTask





/*******************************************************************************************************
* @brief This is a sample EXAMPLE implementation for use of a playback display.  It IS NOT USED HERE JUST
* A SAMPLE.  Hardware requires 7 LED tied to output GPIO.  This function uses the LEDs to indicate an Audio
* Graph of the audio output level.  The output LEDs are driven to a dB level as described in the .h file.
* Logs are used since hearing is in logs.  The code is written to be fast and only updates the LED graph upon change.
* The function accepts the converted 10Bit Audio (see function call_S16Bit_To_10Bit), converts it back to signed 16 bit.
* Since audio is center set - it changes channels to  the absolute value.
*
* @date				4/3/21 \n
* @author 			Hab S. Collector \n
* Last Edited By:  	Hab S. Collector \n
*
* @note This is a 10Bit DAC
* -  Uses 7 LEDs
*
* @param Scaled Audio value - value placed into DAC within Audio IRQ Handler
* @return void
*
 * STEP 1: Convert
 * STEP 2: Only deal in absolutes
 * STEP 3: This is a 10Bit DAC
****************************************************************************************************** */
#ifdef DISPLAY_AUDIO_LEVEL_EXAMPLE
void audioLED_BarGraph(uint16_t ScaledAudio)
{
	int16_t Signed16BitValue;

	// STEP 1: Convert
	Signed16BitValue = (int16_t) (64 * ScaledAudio - 32768);

	// STEP 2: This is a 10Bit DAC
	if (Signed16BitValue < 0)
		Signed16BitValue *= -1;

	// STEP 3: Display Bar Graph
	if ((Signed16BitValue < LEVEL_30DB) && (Last_LED_BarGraphState != 1))
	{
		//switchLED(LED_OFF, (LED_BAR0|LED_BAR1|LED_BAR2|LED_BAR3|LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 1;
	}
	if ((Signed16BitValue >= LEVEL_30DB) && (Signed16BitValue < LEVEL_26DB) && (Last_LED_BarGraphState != 2))
	{
		//switchLED(LED_ON, LED_BAR0);
		//switchLED(LED_OFF, (LED_BAR1|LED_BAR2|LED_BAR3|LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 2;
	}
	if ((Signed16BitValue >= LEVEL_26DB) && (Signed16BitValue < LEVEL_22DB) && (Last_LED_BarGraphState != 3))
	{
		//switchLED(LED_ON, (LED_BAR0|LED_BAR1));
		//switchLED(LED_OFF, (LED_BAR2|LED_BAR3|LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 3;
	}
	if ((Signed16BitValue >= LEVEL_22DB) && (Signed16BitValue < LEVEL_18DB) && (Last_LED_BarGraphState != 4))
	{
		//switchLED(LED_ON, (LED_BAR0|LED_BAR1|LED_BAR2));
		//switchLED(LED_OFF, (LED_BAR3|LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 4;
	}
	if ((Signed16BitValue >= LEVEL_18DB) && (Signed16BitValue < LEVEL_14DB) && (Last_LED_BarGraphState != 5))
	{
		//switchLED(LED_ON, (LED_BAR0|LED_BAR1|LED_BAR2|LED_BAR3));
		//switchLED(LED_OFF, (LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 5;
	}
	if ((Signed16BitValue >= LEVEL_14DB) && (Signed16BitValue < LEVEL_10DB) && (Last_LED_BarGraphState != 6))
	{
		//switchLED(LED_ON, (LED_BAR0|LED_BAR1|LED_BAR2|LED_BAR3|LED_BAR4));
		//switchLED(LED_OFF, LED_BAR5);
		Last_LED_BarGraphState = 6;
	}
	if ((Signed16BitValue >= LEVEL_10DB) && (Last_LED_BarGraphState != 7))
	{
		//switchLED(LED_ON, (LED_BAR0|LED_BAR1|LED_BAR2|LED_BAR3|LED_BAR4|LED_BAR5));
		Last_LED_BarGraphState = 7;
	}

} // END OF audioLED_BarGraph
#endif



