#ifndef IOX_LOG_H_
#define IOX_LOG_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#define LOG_USART			3
#define LOG_SENDBUF_LENGTH	2024
/*
 * If the log message does not fit into the remaining buffer space it is
 * discarded to not block program flow. For debugging purposes this behaviour
 * might be irritating. In this case uncomment the following line. Whenever the
 * buffer is too full, a call to log_write blocks until enough space is available.
 */
#define LOG_BLOCKING

#define USE_ASSERT
#define LOG_USE_MUTEXES

// log levels
#define LevelDebug 0x01
#define LevelInfo  0x02
#define LevelWarn  0x04
#define LevelError 0x08
#define LevelCrit  0x10
#define LevelAll   0x1F

// enabled levels for individual log sources
#define Log_System  	(/*LevelDebug|*/LevelInfo|LevelWarn|LevelError|LevelCrit)
#define Log_Exti		(/*LevelDebug|*/LevelInfo|LevelWarn|LevelError|LevelCrit)
#define Log_App		  	(/*LevelDebug|*/LevelInfo|LevelWarn|LevelError|LevelCrit)
#define Log_FFT			(LevelDebug|LevelInfo|LevelWarn|LevelError|LevelCrit)
#define Log_Dummy		(/*LevelDebug|LevelInfo|*/LevelWarn|LevelError|LevelCrit)

// if LevelDebug is omitted from this mask,
// debug message will not be logged regardless
// of individual source settings
#define Global_Level_Mask (LevelAll)

#define LOG(source, level, message, ...) do { \
    if (source & level & Global_Level_Mask) { \
       log_write(#source, level, message, ##__VA_ARGS__); \
    } \
} while (0)

#ifdef USE_ASSERT
#define ASSERT(x) do { \
	if(!(x)) { \
		log_write("    ASSERT", LevelCrit, "Assertion failed: %s, line %d", __FILE__, __LINE__); \
		log_flush(); \
		__BKPT(); \
	} \
} while (0)
#else
#define ASSERT(x)
#endif


void log_init();
void log_write(const char *module, uint8_t level, const char *fmt, ...);
void log_flush();

void log_force(const char *fmt, ...);

#endif
