/** ****************************************************************************************************
 * @file 			HAB_WAV_AUDIO_TASKS.h
 * @brief			This is the Header file used to support HAB_WAV_AUDIO_TASKS.c
 * ****************************************************************************************************
 * @author			Hab S. Collector \n
 * Last Edited By:	Hab S. Collector \n
 *
 * @date			4/3/21 \n
 * @version       	See HAB_WAV_AUDIO_TASKS.c version
 *
 * @param Development_Environment \n
 * Hardware:		Agnostic
 * IDE:             Agnostic
 * Compiler:        Agnostic
 *
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            See source file for notes
 * 					This is an embedded algorithm for play .wav audio files from disk
 *
 * @copyright		IMR Engineering
 *   			    This algorithm was developed may not be reused without the express consent of IMR Engineering
 * **************************************************************************************************** */

#ifndef HAB_WAV_AUDIO_TASKS_H_
#define HAB_WAV_AUDIO_TASKS_H_
#ifdef __cplusplus
extern"C" {
#endif

// INCLUDES
#include "MainSupport.h"

// WAVE AUDIO OFFSETS - COMMENTS ARE FOR 16BIT PCM WAV AUDIO
#define RIFF_CHUNCK_OFFSET      0U  // EXPECTED "RIFF"
#define RIFF_TYPE_OFFSET        8U  // EXPECTED "WAVE"
#define FORMAT_CHUNCK_OFFSET   12U  // EXPECTED "fmt "
#define FORMAT_SIZE_OFFSET     16U  // EXPECTED 16 FOR PCM
#define COMPRESSION_OFFSET     20U  // EXPECTED 1 FOR PCM
#define CHANNEL_NUMBER_OFFSET  22U  // 1 OR 2
#define SAMPLE_RATE_OFFSET     24U  // 8000, 44100, etc.
#define BYTE_RATE_OFFSET       28U  // SampleRate * NumChannels * BitsPerSample/8
#define BLOCK_ALIGN_OFFSET     32U  // NumChannels * BitsPerSample/8
#define BIT_PER_SAMPLE_OFFSET  34U  // 8 bits = 8, 16 bits = 16
#define DATA_CHUNCK_OFFSET     36U  // EXPECTED "data"
#define DATA_SIZE_OFFSET       40U  // NumSamples * NumChannels * BitsPerSample/8
#define DATA_OFFSET            44U  // LEFT IS THE FIST CHANNEL READ

// WAVE AUDIO CHUNK NAMES
#define RIFF_FILE_TYPE "RIFF"
#define WAVE_RIFF_TYPE "WAVE"
#ifndef STRING_NULL
#define STRING_NULL     ('\0')
#endif

// BUFFER SIZES
#define MAX_LENGTH_WAV_FILE    50U
#define IN_COMMING_BUFFER_SIZE 2048U
#define CIRCULAR_BUFFER_SIZE   512U

// WAVE DIRECTORIES AND FILE NAMES - FILE NAME LIMIT 30 CHARS
#define ROOT_DIR          "0:"
#define AUDIO_PATH        "0:\\HC15C_AUDIO"
#define MUSIC_PATH        "0:\\MUSIC_FILES"

//#define DISPLAY_AUDIO_LEVEL_EXAMPLE
//#define DAC_TYPE_10_BIT
#define DAC_TYPE_12_BIT
#define DISPLAY_COLUMN_TOTAL		20U
#define TIME_TO_AUDIO_OUTPUT_STABLE	10U		// DELAY TO AUDIO AMP OUTPUT DRIVER STABLE IN MILI-SECONDS

// OPTIONAL USER SHOW HOW BASED ON 10BIT DAC
// LED BAR GRAPH LEVELS
#define LEVEL_30DB   1036u          // BASED ON 32768 FULL SCALE -30dB
#define LEVEL_26DB   1642u          // BASED ON 32768 FULL SCALE -26dB
#define LEVEL_22DB   2603u          // BASED ON 32768 FULL SCALE -22dB
#define LEVEL_18DB   4125u          // BASED ON 32768 FULL SCALE -18dB
#define LEVEL_14DB   6537u          // BASED ON 32768 FULL SCALE -14dB
#define LEVEL_10DB   10361u         // BASED ON 32768 FULL SCALE -10dB


// STRUCTURES UNIONS AND ENUMS TYPEDEF
typedef union
{
	int32_t Int32Value;
	uint8_t ByteValue[sizeof(int32_t)];
} Union_BytesPerSecond;

typedef union
{
	int32_t Int32Value;
	uint8_t ByteValue[sizeof(int32_t)];
} Union_DataChunkSize;

typedef union
{
	int16_t Signed16Bit_Value;
	uint8_t ByteValue[sizeof(int16_t)];
} Union_AudioValue;

typedef struct
{
	uint16_t Size;   // MAX NUMBER OF ELEMENTS
	uint16_t Start;  // INDEX OF OLDEST ELEMENT
	uint16_t End;    // INDEX AT WHICH TO WRITE NEW ELEMENT
	uint16_t *Elems; // VECTOR OF ELEMENTS
} Type_CircularBuffer;

enum ChannelDirection
{
	NONE, LEFT, STEREO
};

enum ByteOrder
{
	LOW_BYTE, HIGH_BYTE
};

typedef enum
{
	STATUS_OK,
	FILE_OPEN_ERROR,
	MEMORY_ALLOCATION_ERROR,
	FILE_READ_ERROR,
	FILE_VERIFICATION_ERROR,
	TIMER_FAIL_INIT_ERROR
}Type_RetrunStatus;

typedef struct
{
	char FileNameAndPath[MAX_LENGTH_WAV_FILE];
	bool_t Play;
	bool_t Playing;
	bool_t Pause;
	uint8_t NumberOfChannels;
	volatile uint32_t PlayTimeInSeconds;
	volatile uint8_t TimeRemainingInMinutes;
	volatile uint8_t TimeRemainingInSeconds;
} Type_AudioPlayBack;


// FUNCTION PROTOTYPES
Type_RetrunStatus call_play16Bit_WAVE(Type_AudioPlayBack *);
void audioTimerIrqHandler(void);
uint16_t call_S16Bit_To_10Bit(int16_t);
uint16_t call_S16Bit_To_12Bit(int16_t);
void updateMusicTime(void);
bool_t init_CB(Type_CircularBuffer *, uint16_t , uint8_t);
void free_CB(Type_CircularBuffer *);
uint8_t isFull_CB(Type_CircularBuffer *);
uint8_t isEmpty_CB(Type_CircularBuffer *);
void write_CB(Type_CircularBuffer *, uint16_t *);
void read_CB(Type_CircularBuffer *, uint16_t *);
// USER PROVIDED REPLACMENT FUNCTIONS
void disableAudioOutput(void);
void enableAudioOutput(void);
void writeDAC_LeftChannel(uint16_t DAC_Value);
void writeDAC_RightChannel(uint16_t DAC_Value);
void startAudioTimer(void);
void stopAudioTimer(void);
void delayInMiliseonds(uint32_t);
// USER OPTIONAL FUNCTIONS
void haultActionsWithMusicPlaying(void);
void pauseActionsWithMusicPlaying(void);
void yieldControlToAnotherTask(void);

#ifdef __cplusplus
}
#endif
#endif
