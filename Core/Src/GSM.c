/*
 *GSM.c
 *
 *  Created on: Jul 10, 2026
 *      Author: sasii
 */


#include "GSM.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GSM_Status_t GSM_Init(
        GSM_Handle_t *gsm,
        UART_HandleTypeDef *huart)
{
    if(gsm == NULL || huart == NULL)
    {
        return GSM_ERROR;
    }

    memset(gsm,0,sizeof(GSM_Handle_t));

    gsm->huart = huart;

    HAL_Delay(2000);

    if(GSM_CheckNetwork(gsm) != GSM_OK)
    {
        return GSM_NETWORK_ERROR;
    }

    return GSM_OK;
}


GSM_Status_t GSM_SendCommand(
        GSM_Handle_t *gsm,
        char *command,
        char *response,
        uint32_t timeout)
{
    memset(gsm->rx_buffer,0,GSM_RX_BUFFER_SIZE);

    HAL_UART_Transmit(
            gsm->huart,
            (uint8_t*)command,
            strlen(command),
            timeout);

    HAL_UART_Receive(
            gsm->huart,
            gsm->rx_buffer,
            GSM_RX_BUFFER_SIZE,
            timeout);

    if(strstr((char*)gsm->rx_buffer,response))
    {
        return GSM_OK;
    }

    return GSM_ERROR;
}


GSM_Status_t GSM_CheckNetwork(
        GSM_Handle_t *gsm)
{
    if(GSM_SendCommand(
            gsm,
            "AT\r\n",
            "OK",
            1000) != GSM_OK)
    {
        return GSM_ERROR;
    }

    if(GSM_SendCommand(
            gsm,
            "AT+CREG?\r\n",
            "+CREG: 0,1",
            2000) == GSM_OK)
    {
        gsm->network_registered = 1;
        return GSM_OK;
    }

    if(GSM_SendCommand(
            gsm,
            "AT+CREG?\r\n",
            "+CREG: 0,5",
            2000) == GSM_OK)
    {
        gsm->network_registered = 1;
        return GSM_OK;
    }

    gsm->network_registered = 0;

    return GSM_NETWORK_ERROR;
}


GSM_Status_t GSM_CheckSignalStrength(
        GSM_Handle_t *gsm)
{
    if(GSM_SendCommand(
            gsm,
            "AT+CSQ\r\n",
            "OK",
            1000) != GSM_OK)
    {
        return GSM_ERROR;
    }

    char *ptr;

    ptr = strstr((char*)gsm->rx_buffer,"+CSQ:");

    if(ptr != NULL)
    {
        gsm->signal_strength = atoi(ptr + 6);
    }

    return GSM_OK;
}


GSM_Status_t GSM_SendSMS(
        GSM_Handle_t *gsm,
        char *number,
        char *message)
{
    char command[50];

    /* Set SMS text mode */
    if(GSM_SendCommand(
            gsm,
            "AT+CMGF=1\r\n",
            "OK",
            2000) != GSM_OK)
    {
        return GSM_SMS_ERROR;
    }

    /* Set recipient number */
    sprintf(command,
            "AT+CMGS=\"%s\"\r\n",
            number);

    HAL_UART_Transmit(
            gsm->huart,
            (uint8_t*)command,
            strlen(command),
            1000);

    HAL_Delay(1000);

    /* Send SMS body */
    HAL_UART_Transmit(
            gsm->huart,
            (uint8_t*)message,
            strlen(message),
            1000);

    /* CTRL+Z */
    uint8_t ctrl_z = 26;

    HAL_UART_Transmit(
            gsm->huart,
            &ctrl_z,
            1,
            1000);

    HAL_Delay(5000);

    return GSM_OK;
}
GSM_Status_t GSM_SendEmergencySMS(
        GSM_Handle_t *gsm,
        char *number,
        float latitude,
        float longitude)
{
    char message[250];

    sprintf(message,
            "SMART HELMET ALERT\r\n"
            "\r\n"
            "Accident Detected!\r\n"
            "\r\n"
            "Location:\r\n"
            "https://maps.google.com/?q=%.6f,%.6f\r\n"
            "\r\n"
            "Please reach immediately.",
            latitude,
            longitude);

    return GSM_SendSMS(
            gsm,
            number,
            message);
}
