/*
 * GPS.c
 *
 *  Created on: Jul 10, 2026
 *      Author: sas
 */

#include "GPS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GPS_Status_t GPS_Init(GPS_Handle_t *gps,
                      UART_HandleTypeDef *huart)
{
    if(gps == NULL || huart == NULL)
    {
        return GPS_ERROR;
    }

    memset(gps, 0, sizeof(GPS_Handle_t));

    gps->huart = huart;

    gps->old_position = 0;

    gps->data.fix = 0;
    gps->data.latitude = 0.0f;
    gps->data.longitude = 0.0f;
    gps->data.altitude = 0.0f;
    gps->data.satellites = 0;

    return GPS_OK;
}


GPS_Status_t GPS_StartDMAReception(GPS_Handle_t *gps)
{
    if(gps == NULL)
    {
        return GPS_ERROR;
    }

    if(HAL_UART_Receive_DMA(
            gps->huart,
            gps->dma_buffer,
            GPS_RX_BUFFER_SIZE) != HAL_OK)
    {
        return GPS_ERROR;
    }

    return GPS_OK;
}


uint8_t GPS_HasFix(GPS_Handle_t *gps)
{
    if(gps == NULL)
    {
        return 0;
    }

    return gps->data.fix;
}


GPS_Status_t GPS_GetCoordinates(GPS_Handle_t *gps,
                                GPS_Data_t *data)
{
    if(gps == NULL || data == NULL)
    {
        return GPS_ERROR;
    }

    if(gps->data.fix == 0)
    {
        return GPS_NO_FIX;
    }

    memcpy(data,
           &(gps->data),
           sizeof(GPS_Data_t));

    return GPS_OK;
}


float GPS_ConvertCoordinate(char *coordinate,
                            char direction)
{
    float raw;
    int degrees;
    float minutes;
    float decimal;

    raw = atof(coordinate);

    degrees = (int)(raw / 100);

    minutes = raw - (degrees * 100);

    decimal = degrees + (minutes / 60.0f);

    if(direction == 'S' || direction == 'W')
    {
        decimal *= -1.0f;
    }

    return decimal;
}


GPS_Status_t GPS_ParseGGA(GPS_Handle_t *gps,
                          char *sentence)
{
    char *token;
    uint8_t field = 0;

    char latitude[20];
    char longitude[20];

    char lat_dir = 'N';
    char lon_dir = 'E';

    token = strtok(sentence, ",");

    while(token != NULL)
    {
        switch(field)
        {
            case 1:
                strcpy(gps->data.utc_time, token);
                break;

            case 2:
                strcpy(latitude, token);
                break;

            case 3:
                lat_dir = token[0];
                break;

            case 4:
                strcpy(longitude, token);
                break;

            case 5:
                lon_dir = token[0];
                break;

            case 6:
                gps->data.fix = atoi(token);
                break;

            case 7:
                gps->data.satellites = atoi(token);
                break;

            case 9:
                gps->data.altitude = atof(token);
                break;
        }

        token = strtok(NULL, ",");
        field++;
    }

    if(gps->data.fix)
    {
        gps->data.latitude =
            GPS_ConvertCoordinate(latitude,
                                  lat_dir);

        gps->data.longitude =
            GPS_ConvertCoordinate(longitude,
                                  lon_dir);

        return GPS_OK;
    }

    return GPS_NO_FIX;
}

GPS_Status_t GPS_ParseRMC(GPS_Handle_t *gps,
                          char *sentence)
{
    if(strstr(sentence, ",A,"))
    {
        gps->data.fix = 1;
        return GPS_OK;
    }

    gps->data.fix = 0;

    return GPS_NO_FIX;
}

GPS_Status_t GPS_Process(GPS_Handle_t *gps)
{
    uint16_t current_position;
    static char sentence[GPS_SENTENCE_SIZE];
    static uint16_t sentence_index = 0;

    current_position =
        GPS_RX_BUFFER_SIZE -
        __HAL_DMA_GET_COUNTER(gps->huart->hdmarx);

    while(gps->old_position != current_position)
    {
        char c =
            gps->dma_buffer[gps->old_position];

        if(c == '$')
        {
            sentence_index = 0;
        }

        if(sentence_index < GPS_SENTENCE_SIZE - 1)
        {
            sentence[sentence_index++] = c;
        }

        if(c == '\n')
        {
            sentence[sentence_index] = '\0';

            if(strstr(sentence, "$GNGGA"))
            {
                GPS_ParseGGA(gps, sentence);
            }

            if(strstr(sentence, "$GPGGA"))
            {
                GPS_ParseGGA(gps, sentence);
            }

            if(strstr(sentence, "$GNRMC"))
            {
                GPS_ParseRMC(gps, sentence);
            }

            if(strstr(sentence, "$GPRMC"))
            {
                GPS_ParseRMC(gps, sentence);
            }

            sentence_index = 0;
        }

        gps->old_position++;

        if(gps->old_position >= GPS_RX_BUFFER_SIZE)
        {
            gps->old_position = 0;
        }
    }

    return GPS_OK;
}
