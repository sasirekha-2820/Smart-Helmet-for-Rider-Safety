/*
 * GSM.h
 *
 *  Created on: Jul 10, 2026
 *      Author: sas
 */
#ifndef INC_GSM_H_
#define INC_GSM_H_

#include "main.h"
#include <stdint.h>

#define GSM_RX_BUFFER_SIZE     256
#define GSM_TX_BUFFER_SIZE     256
#define GSM_TIMEOUT            5000

typedef enum
{
    GSM_OK = 0,
    GSM_ERROR,
    GSM_TIMEOUT_ERROR,
    GSM_NETWORK_ERROR,
    GSM_SMS_ERROR

} GSM_Status_t;

typedef struct
{
    UART_HandleTypeDef *huart;

    uint8_t rx_buffer[GSM_RX_BUFFER_SIZE];
    uint8_t tx_buffer[GSM_TX_BUFFER_SIZE];

    uint8_t network_registered;
    uint8_t signal_strength;

} GSM_Handle_t;


/* Initialization */
GSM_Status_t GSM_Init(
        GSM_Handle_t *gsm,
        UART_HandleTypeDef *huart);

/* AT Command Interface */
GSM_Status_t GSM_SendCommand(
        GSM_Handle_t *gsm,
        char *command,
        char *response,
        uint32_t timeout);

/* Network Functions */
GSM_Status_t GSM_CheckNetwork(
        GSM_Handle_t *gsm);

GSM_Status_t GSM_CheckSignalStrength(
        GSM_Handle_t *gsm);

/* SMS Functions */
GSM_Status_t GSM_SendSMS(
        GSM_Handle_t *gsm,
        char *number,
        char *message);

GSM_Status_t GSM_SendEmergencySMS(
        GSM_Handle_t *gsm,
        char *number,
        float latitude,
        float longitude);

#endif
