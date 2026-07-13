/**
 ******************************************************************************
 * @file    ADXL335.c
 * @author  Naveen Yogaraj M
 * @brief   ADXL335 Driver Source File
 ******************************************************************************
 */

#include "ADXL335.h"

/* Private Function Prototypes ----------------------------------------------*/

static ADXL335_Status_t ADXL335_ReadADCChannel(ADXL335_Handle_t *hadxl,
                                               uint32_t Channel,
                                               uint16_t *ADC_Value);

static float ADXL335_ADCToVoltage(ADXL335_Handle_t *hadxl,
                                  uint16_t ADC_Value);

static float ADXL335_VoltageToAcceleration(float Voltage,
                                           float ZeroG,
                                           float Sensitivity);

/*============================================================================
 * @brief  Initialize ADXL335 Driver
 *===========================================================================*/
ADXL335_Status_t ADXL335_Init(ADXL335_Handle_t *hadxl,
                              ADC_HandleTypeDef *hadc)
{
    if((hadxl == NULL) || (hadc == NULL))
    {
        return ADXL335_INVALID_PARAM;
    }

    hadxl->hadc = hadc;

    hadxl->Vref = ADXL335_DEFAULT_VREF;

    hadxl->Sensitivity = ADXL335_DEFAULT_SENSITIVITY;

    hadxl->ZeroGX = ADXL335_DEFAULT_ZERO_G;
    hadxl->ZeroGY = ADXL335_DEFAULT_ZERO_G;
    hadxl->ZeroGZ = ADXL335_DEFAULT_ZERO_G;

    return ADXL335_OK;
}

/*============================================================================
 * @brief  Configure Calibration Parameters
 *===========================================================================*/
ADXL335_Status_t ADXL335_SetCalibration(ADXL335_Handle_t *hadxl,
                                        float ZeroGX,
                                        float ZeroGY,
                                        float ZeroGZ,
                                        float Sensitivity)
{
    if(hadxl == NULL)
    {
        return ADXL335_INVALID_PARAM;
    }

    hadxl->ZeroGX = ZeroGX;
    hadxl->ZeroGY = ZeroGY;
    hadxl->ZeroGZ = ZeroGZ;

    hadxl->Sensitivity = Sensitivity;

    return ADXL335_OK;
}

/*============================================================================
 * @brief  Read ADC Channel
 *===========================================================================*/
static ADXL335_Status_t ADXL335_ReadADCChannel(ADXL335_Handle_t *hadxl,
                                               uint32_t Channel,
                                               uint16_t *ADC_Value)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    if((hadxl == NULL) || (ADC_Value == NULL))
    {
        return ADXL335_INVALID_PARAM;
    }

    sConfig.Channel      = Channel;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

    if(HAL_ADC_ConfigChannel(hadxl->hadc, &sConfig) != HAL_OK)
    {
        return ADXL335_ERROR;
    }

    if(HAL_ADC_Start(hadxl->hadc) != HAL_OK)
    {
        return ADXL335_ERROR;
    }

    if(HAL_ADC_PollForConversion(hadxl->hadc, HAL_MAX_DELAY) != HAL_OK)
    {
        HAL_ADC_Stop(hadxl->hadc);
        return ADXL335_TIMEOUT;
    }

    *ADC_Value = (uint16_t)HAL_ADC_GetValue(hadxl->hadc);

    HAL_ADC_Stop(hadxl->hadc);

    return ADXL335_OK;
}

/*============================================================================
 * @brief  Convert ADC Counts to Voltage
 *===========================================================================*/
static float ADXL335_ADCToVoltage(ADXL335_Handle_t *hadxl,
                                  uint16_t ADC_Value)
{
    return ((float)ADC_Value * hadxl->Vref) /
            ADXL335_ADC_RESOLUTION;
}

/*============================================================================
 * @brief  Convert Voltage to Acceleration
 *===========================================================================*/
static float ADXL335_VoltageToAcceleration(float Voltage,
                                           float ZeroG,
                                           float Sensitivity)
{
    return (Voltage - ZeroG) / Sensitivity;
}

/*============================================================================
 * @brief  Read Complete Sensor Data
 *===========================================================================*/
ADXL335_Status_t ADXL335_ReadAll(ADXL335_Handle_t *hadxl,
                                 ADXL335_Data_t *data)
{
    ADXL335_Status_t status;

    if((hadxl == NULL) || (data == NULL))
    {
        return ADXL335_INVALID_PARAM;
    }

    /*---------------- Read X Axis ----------------*/

    status = ADXL335_ReadADCChannel(hadxl,
                                    hadxl->X_Channel,
                                    &data->RawX);

    if(status != ADXL335_OK)
    {
        return status;
    }

    data->VoltageX = ADXL335_ADCToVoltage(hadxl,
                                          data->RawX);

    data->AccelX = ADXL335_VoltageToAcceleration(
                        data->VoltageX,
                        hadxl->ZeroGX,
                        hadxl->Sensitivity);

    /*---------------- Read Y Axis ----------------*/

    status = ADXL335_ReadADCChannel(hadxl,
                                    hadxl->Y_Channel,
                                    &data->RawY);

    if(status != ADXL335_OK)
    {
        return status;
    }

    data->VoltageY = ADXL335_ADCToVoltage(hadxl,
                                          data->RawY);

    data->AccelY = ADXL335_VoltageToAcceleration(
                        data->VoltageY,
                        hadxl->ZeroGY,
                        hadxl->Sensitivity);

    /*---------------- Read Z Axis ----------------*/

    status = ADXL335_ReadADCChannel(hadxl,
                                    hadxl->Z_Channel,
                                    &data->RawZ);

    if(status != ADXL335_OK)
    {
        return status;
    }

    data->VoltageZ = ADXL335_ADCToVoltage(hadxl,
                                          data->RawZ);

    data->AccelZ = ADXL335_VoltageToAcceleration(
                        data->VoltageZ,
                        hadxl->ZeroGZ,
                        hadxl->Sensitivity);

    return ADXL335_OK;
}

/************************ (C) END OF FILE ************************************/
