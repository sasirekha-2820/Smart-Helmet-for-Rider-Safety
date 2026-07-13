/**
 ******************************************************************************
 * @file    ADXL335.h
 * @author  Naveen Yogaraj M
 * @brief   Driver for the Analog Devices ADXL335 3-Axis Analog Accelerometer
 *
 * @note
 * This driver is written completely from scratch using STM32 HAL.
 * No vendor libraries are used.
 *
 ******************************************************************************
 */

#ifndef __ADXL335_H
#define __ADXL335_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include <stdint.h>

/* Exported Macros -----------------------------------------------------------*/

/* ADC Resolution */

#define ADXL335_ADC_RESOLUTION          4095.0f

/* Default Sensor Parameters */

#define ADXL335_DEFAULT_VREF            3.3f
#define ADXL335_DEFAULT_ZERO_G          1.65f
#define ADXL335_DEFAULT_SENSITIVITY     0.330f      /* V/g */

/* Exported Types ------------------------------------------------------------*/

/**
 * @brief Driver Status
 */
typedef enum
{
    ADXL335_OK = 0,

    ADXL335_ERROR,

    ADXL335_TIMEOUT,

    ADXL335_INVALID_PARAM

} ADXL335_Status_t;


/**
 * @brief Driver Handle
 */
typedef struct
{
    ADC_HandleTypeDef *hadc;

    /* ADC Channels */

    uint32_t X_Channel;

    uint32_t Y_Channel;

    uint32_t Z_Channel;

    /* Calibration */

    float Vref;

    float Sensitivity;

    float ZeroGX;

    float ZeroGY;

    float ZeroGZ;

} ADXL335_Handle_t;


/**
 * @brief Complete Sensor Data
 */
typedef struct
{
    /* Raw ADC Counts */

    uint16_t RawX;

    uint16_t RawY;

    uint16_t RawZ;

    /* Voltages */

    float VoltageX;

    float VoltageY;

    float VoltageZ;

    /* Acceleration */

    float AccelX;

    float AccelY;

    float AccelZ;

} ADXL335_Data_t;


/* Exported Functions --------------------------------------------------------*/

/**
 * @brief Initialize ADXL335 Driver
 *
 * @param hadxl Pointer to ADXL335 Handle
 * @param hadc Pointer to ADC Handle
 *
 * @retval Driver Status
 */
ADXL335_Status_t ADXL335_Init(
        ADXL335_Handle_t *hadxl,
        ADC_HandleTypeDef *hadc);


/**
 * @brief Read Complete Sensor Data
 *
 * Reads:
 * - Raw ADC Counts
 * - Voltages
 * - Accelerations
 *
 * @param hadxl Pointer to Driver Handle
 * @param data Pointer to Data Structure
 *
 * @retval Driver Status
 */
ADXL335_Status_t ADXL335_ReadAll(
        ADXL335_Handle_t *hadxl,
        ADXL335_Data_t *data);


/**
 * @brief Configure Calibration Parameters
 *
 * @param hadxl Pointer to Driver Handle
 * @param ZeroGX Zero-g voltage of X-axis
 * @param ZeroGY Zero-g voltage of Y-axis
 * @param ZeroGZ Zero-g voltage of Z-axis
 * @param Sensitivity Sensor Sensitivity (V/g)
 *
 * @retval Driver Status
 */
ADXL335_Status_t ADXL335_SetCalibration(
        ADXL335_Handle_t *hadxl,
        float ZeroGX,
        float ZeroGY,
        float ZeroGZ,
        float Sensitivity);


#ifdef __cplusplus
}
#endif

#endif
