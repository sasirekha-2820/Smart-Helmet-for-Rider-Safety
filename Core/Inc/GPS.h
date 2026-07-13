/*
 * GPS.h
 *
 *  Created on: Jul 9, 2026
 *      Author: sas
 */
#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "main.h"
#include <stdint.h>

#define GPS_RX_BUFFER_SIZE      512
#define GPS_SENTENCE_SIZE       128

typedef enum
{
    GPS_OK = 0,
    GPS_ERROR,
    GPS_NO_FIX,
    GPS_TIMEOUT

} GPS_Status_t;

typedef struct
{
    float latitude;
    float longitude;

    uint8_t fix;

    uint8_t satellites;

    float altitude;

    char utc_time[16];

} GPS_Data_t;

typedef struct
{
    UART_HandleTypeDef *huart;

    uint8_t dma_buffer[GPS_RX_BUFFER_SIZE];

    uint16_t old_position;

    GPS_Data_t data;

} GPS_Handle_t;


/* Initialization */
GPS_Status_t GPS_Init(
        GPS_Handle_t *gps,
        UART_HandleTypeDef *huart);

/* Start DMA Reception */
GPS_Status_t GPS_StartDMAReception(
        GPS_Handle_t *gps);

/* Process received NMEA packets */
GPS_Status_t GPS_Process(
        GPS_Handle_t *gps);

/* Check GPS Fix */
uint8_t GPS_HasFix(
        GPS_Handle_t *gps);

/* Get Coordinates */
GPS_Status_t GPS_GetCoordinates(
        GPS_Handle_t *gps,
        GPS_Data_t *data);

/* Internal Parser Functions */
GPS_Status_t GPS_ParseGGA(
        GPS_Handle_t *gps,
        char *sentence);

GPS_Status_t GPS_ParseRMC(
        GPS_Handle_t *gps,
        char *sentence);

float GPS_ConvertCoordinate(
        char *coordinate,
        char direction);

#endif
