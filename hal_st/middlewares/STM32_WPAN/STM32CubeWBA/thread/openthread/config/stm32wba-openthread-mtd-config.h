/* include/openthread-config.h.  Generated from openthread-config.h.in by configure.  */
/* include/openthread-config.h.in.  Generated from configure.ac by autoheader.  */

//#include "common.h"
#include <string.h>

/**********************************************************************************************/
/* Standard C functions encapsulation                                                         */
/*                                                                                            */
/**********************************************************************************************/
#define HAVE_STDBOOL_H 1
#define HAVE_STRNLEN 1
#define HAVE_STRLCAT 1

/**********************************************************************************************/
/* OVERALL STACK CONFIGURATION                                                                */
/**********************************************************************************************/

/**********************************************************************************************/
/* STANDARD DEFINES                                                                           */
/* Defines which are customized for STM32WBA                                                   */
/*                                                                                            */
/* An example of list of these defines is found inside the OpenThread source code             */
/* (refer to file openthread-windows.h)                                                       */
/**********************************************************************************************/

/* Define all OpenThread API specific flags */
#include "openthread_api_config_mtd.h"

/* Define to the platform name. */
/* The platform-specific string to insert into the OpenThread version string  */
#define OPENTHREAD_CONFIG_PLATFORM_INFO "STM32WBA V1.3.0 Thread_MTD"

/* Name of package */
#define PACKAGE "openthread"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "openthread-devel@googlegroups.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "OT"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "OPENTHREAD 0.01.00"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "openthread"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://github.com/openthread/openthread"

/* Define to the version of this package. */
#define PACKAGE_VERSION "SHA-1: 8bc25042ba5cde20605eec2a329c0dff575303f4"

/* Version number of package */
#define VERSION "MTD"

/**********************************************************************************************/
/* GENERIC OPENTHREAD DEFINES REDEFINITION                                                    */
/* Redefines which are overloading defines stores inside the stm32wb-openthread-config.h file */
/**********************************************************************************************/

/**
 * @def OPENTHREAD_CONFIG_MAX_STATECHANGE_HANDLERS
 *
 * The maximum number of state-changed callback handlers (set using `otSetStateChangedCallback()`).
 *
 */
#define OPENTHREAD_CONFIG_MAX_STATECHANGE_HANDLERS               2 

/**
 * @def OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS
 *
 * The number of message buffers in the buffer pool.
 *
 */
#ifdef STATIC_MODE
#define OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS                   64
#else
#define OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS                   128
#endif

/**********************************************************************************************/
/* FLASH DEFINES REDEFINITION                                                                 */
/* Redefines which are overloading defines stores inside the settings.cpp file                */
/**********************************************************************************************/

 /* @def SETTINGS_CONFIG_BASE_ADDRESS
 *  The base address where are stored the data in Flash.
 *  Note : For the moment, we are writing these data in RAM.
 *
 */
#define SETTINGS_CONFIG_BASE_ADDRESS                          0 

/**
 * @def SETTINGS_CONFIG_PAGE_SIZE
 *
 * The page size of settings.
 *
 */
#define SETTINGS_CONFIG_PAGE_SIZE                               2048

/**
 * @def SETTINGS_CONFIG_PAGE_NUM
 *
 * The page number of settings.
 *
 */
#define SETTINGS_CONFIG_PAGE_NUM                                2

/**
 * @def OPENTHREAD_CONFIG_LOG_PLATFORM
 *
 * Enable platform specific logging.
 *
 */
#define OPENTHREAD_CONFIG_LOG_PLATFORM                          1
