/*
 * ISM330DHCX.c
 *
 *  Created on: 05-Jul-2026
 *      Author: naveen
 */

#include"ISM330DHCX.h"
#include<stdio.h>
/**
 * @brief  Read a single register from the ISM330DHCX.
 * @param  dev   Pointer to the device handle.
 * @param  reg   Register address.
 * @param  data  Pointer to store the received data.
 * @retval ISM330DHCX_Status_t
 */

ISM330DHCX_Status_t ISM330DHCX_CheckID(
    ISM330DHCX_Handle_t *dev)
{
    uint8_t id;

    if (dev == NULL)
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_ReadRegister(dev, ISM330_WHO_AM_I, &id) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    if (id == ISM330_ID)
    {
        return ISM330DHCX_OK;
    }

    return ISM330DHCX_ERROR;
}

ISM330DHCX_Status_t ISM330DHCX_ReadRegister(
    ISM330DHCX_Handle_t *dev,
    uint8_t reg,
    uint8_t *data)
{
    /* Check for invalid parameters */
    if ((dev == NULL) || (data == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    /* Read one byte from the register */
    if (HAL_I2C_Mem_Read(dev->hi2c,
                         dev->Address,
                         reg,
                         I2C_MEMADD_SIZE_8BIT,
                         data,
                         1,
                         HAL_MAX_DELAY) != HAL_OK)
    {
        return ISM330DHCX_ERROR;
    }

    return ISM330DHCX_OK;
}

ISM330DHCX_Status_t ISM330DHCX_WriteRegister(
    ISM330DHCX_Handle_t *dev,
    uint8_t reg,
    uint8_t data)
{
    HAL_StatusTypeDef status;

    if (dev == NULL)
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    status = HAL_I2C_Mem_Write(dev->hi2c,
                               dev->Address,
                               reg,
                               I2C_MEMADD_SIZE_8BIT,
                               &data,
                               1,
                               HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        return ISM330DHCX_OK;
    }
    else if (status == HAL_TIMEOUT)
    {
        return ISM330DHCX_TIMEOUT;
    }

    return ISM330DHCX_ERROR;
}

ISM330DHCX_Status_t ISM330DHCX_ReadRegisters(
    ISM330DHCX_Handle_t *dev,
    uint8_t reg,
    uint8_t *buffer,
    uint16_t length)
{
    HAL_StatusTypeDef status;

    if ((dev == NULL) || (buffer == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    status = HAL_I2C_Mem_Read(dev->hi2c,
                              dev->Address,
                              reg,
                              I2C_MEMADD_SIZE_8BIT,
                              buffer,
                              length,
                              HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        return ISM330DHCX_OK;
    }
    else if (status == HAL_TIMEOUT)
    {
        return ISM330DHCX_TIMEOUT;
    }

    return ISM330DHCX_ERROR;
}

ISM330DHCX_Status_t ISM330DHCX_Init(ISM330DHCX_Handle_t *dev)
{
    if (dev == NULL)
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_CheckID(dev) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    /* Accelerometer
     * ODR = 104 Hz
     * FS = ±2g
     */
    if (ISM330DHCX_WriteRegister(dev, ISM330_CTRL1_XL, 0x40) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    /* Gyroscope
     * ODR = 104 Hz
     * FS = ±250 dps
     */
    if (ISM330DHCX_WriteRegister(dev, ISM330_CTRL2_G, 0x40) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    return ISM330DHCX_OK;
}

ISM330DHCX_Status_t ISM330DHCX_ReadAccel(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data)
{
    uint8_t buffer[6];
    int16_t rawX, rawY, rawZ;

    if ((dev == NULL) || (data == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_ReadRegisters(dev,
                                 ISM330_OUTX_L_A,
                                 buffer,
                                 6) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    rawX = (int16_t)((buffer[1] << 8) | buffer[0]);
    rawY = (int16_t)((buffer[3] << 8) | buffer[2]);
    rawZ = (int16_t)((buffer[5] << 8) | buffer[4]);

    data->AccelX = rawX * 0.061f / 1000.0f;
    data->AccelY = rawY * 0.061f / 1000.0f;
    data->AccelZ = rawZ * 0.061f / 1000.0f;

    return ISM330DHCX_OK;
}

ISM330DHCX_Status_t ISM330DHCX_ReadGyro(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data)
{
    uint8_t buffer[6];
    int16_t rawX, rawY, rawZ;

    if ((dev == NULL) || (data == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_ReadRegisters(dev,
                                 ISM330_OUTX_L_G,
                                 buffer,
                                 6) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    rawX = (int16_t)((buffer[1] << 8) | buffer[0]);
    rawY = (int16_t)((buffer[3] << 8) | buffer[2]);
    rawZ = (int16_t)((buffer[5] << 8) | buffer[4]);


    printf("RAW GYR: %d %d %d\r\n", rawX, rawY, rawZ);

    data->GyroX = rawX * 8.75f / 1000.0f;
    data->GyroY = rawY * 8.75f / 1000.0f;
    data->GyroZ = rawZ * 8.75f / 1000.0f;

    return ISM330DHCX_OK;
}

ISM330DHCX_Status_t ISM330DHCX_ReadTemperature(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data)
{
    uint8_t buffer[2];
    int16_t rawTemp;

    if ((dev == NULL) || (data == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_ReadRegisters(dev,
                                 ISM330_OUT_TEMP_L,
                                 buffer,
                                 2) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    rawTemp = (int16_t)((buffer[1] << 8) | buffer[0]);

    data->Temperature = 25.0f + (rawTemp / 256.0f);

    return ISM330DHCX_OK;
}

ISM330DHCX_Status_t ISM330DHCX_ReadAll(
    ISM330DHCX_Handle_t *dev,
    ISM330DHCX_Data_t *data)
{
    if ((dev == NULL) || (data == NULL))
    {
        return ISM330DHCX_INVALID_PARAM;
    }

    if (ISM330DHCX_ReadAccel(dev, data) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    if (ISM330DHCX_ReadGyro(dev, data) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    if (ISM330DHCX_ReadTemperature(dev, data) != ISM330DHCX_OK)
    {
        return ISM330DHCX_ERROR;
    }

    return ISM330DHCX_OK;
}





