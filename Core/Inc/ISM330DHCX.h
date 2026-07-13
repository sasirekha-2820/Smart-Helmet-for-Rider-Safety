/*
 * ISM330DHCX.h
 *
 *  Created on: 05-Jul-2026
 *      Author: navee
 */

#ifndef __ISM330DHCX_H
#define __ISM330DHCX_H

#include "main.h"
/*=====================================================================
 * ISM330DHCX I2C ADDRESS
 *====================================================================*/
#define ISM330_I2C_ADDR_LOW     (0x6A << 1)
#define ISM330_I2C_ADDR_HIGH    (0x6B << 1)

/*=====================================================================
 * WHO_AM_I
 *====================================================================*/
#define ISM330_WHO_AM_I         0x0F
#define ISM330_ID               0x6B
/*=====================================================================
 * CONTROL REGISTERS
 *====================================================================*/
#define ISM330_CTRL1_XL         0x10
#define ISM330_CTRL2_G          0x11
#define ISM330_CTRL3_C          0x12
/*=====================================================================
 * STATUS REGISTERS
 *====================================================================*/
#define ISM330_STATUS_REG       0x1E
/*=====================================================================
 * OUTPUT DATA REGISTERS
 *====================================================================*/
#define ISM330_OUTX_L_A         0x28
#define ISM330_OUTX_H_A         0x29

#define ISM330_OUTY_L_A         0x2A
#define ISM330_OUTY_H_A         0x2B

#define ISM330_OUTZ_L_A         0x2C
#define ISM330_OUTZ_H_A         0x2D



#define ISM330_OUTX_L_G         0x22
#define ISM330_OUTX_H_G         0x23

#define ISM330_OUTY_L_G         0x24
#define ISM330_OUTY_H_G         0x25

#define ISM330_OUTZ_L_G         0x26
#define ISM330_OUTZ_H_G         0x27


#define ISM330_OUT_TEMP_L       0x20
#define ISM330_OUT_TEMP_H       0x21
/*=====================================================================
 * FUNCTION PROTOTYPES
 *====================================================================*/


/*=====================================================================
 * Data Structure handles
 * =====================================================================*/
typedef struct
{
    I2C_HandleTypeDef *hi2c;
    uint16_t Address;

} ISM330DHCX_Handle_t;

typedef struct
{
    float AccelX;
    float AccelY;
    float AccelZ;

    float GyroX;
    float GyroY;
    float GyroZ;

    float Temperature;

} ISM330DHCX_Data_t;

typedef enum
{
    ISM330DHCX_OK = 0,
    ISM330DHCX_ERROR,
    ISM330DHCX_TIMEOUT,
    ISM330DHCX_INVALID_PARAM

} ISM330DHCX_Status_t;

ISM330DHCX_Status_t ISM330DHCX_Init(ISM330DHCX_Handle_t *dev);

ISM330DHCX_Status_t ISM330DHCX_CheckID(ISM330DHCX_Handle_t *dev);

ISM330DHCX_Status_t ISM330DHCX_ReadRegister(ISM330DHCX_Handle_t *dev,uint8_t reg,uint8_t *data);

ISM330DHCX_Status_t ISM330DHCX_WriteRegister(ISM330DHCX_Handle_t *dev,uint8_t reg,uint8_t data);

ISM330DHCX_Status_t ISM330DHCX_ReadRegisters(ISM330DHCX_Handle_t *dev,uint8_t reg,uint8_t *buffer,uint16_t length);
/**
 * @brief Read accelerometer data.
 *
 * @param dev Pointer to the ISM330DHCX handle.
 * @param data Pointer to the data structure.
 *
 * @retval ISM330DHCX_Status_t
 */
ISM330DHCX_Status_t ISM330DHCX_ReadAccel(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data);

/**
 * @brief Read gyroscope data.
 *
 * @param dev Pointer to the ISM330DHCX handle.
 * @param data Pointer to the data structure.
 *
 * @retval ISM330DHCX_Status_t
 */
ISM330DHCX_Status_t ISM330DHCX_ReadGyro(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data);

/**
 * @brief Read temperature.
 *
 * @param dev Pointer to the ISM330DHCX handle.
 * @param data Pointer to the data structure.
 *
 * @retval ISM330DHCX_Status_t
 */
ISM330DHCX_Status_t ISM330DHCX_ReadTemperature(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data);

/**
 * @brief Read accelerometer, gyroscope and temperature.
 *
 * @param dev Pointer to the ISM330DHCX handle.
 * @param data Pointer to the data structure.
 *
 * @retval ISM330DHCX_Status_t
 */
ISM330DHCX_Status_t ISM330DHCX_ReadAll(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data);

#endif /* __ISM330DHCX_H */
