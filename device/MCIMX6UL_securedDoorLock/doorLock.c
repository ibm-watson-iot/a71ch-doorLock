/*******************************************************************************
 * Copyright (c) 2018 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ranjan Dasgupta - Initial drop of deviceSample.c
 * 
 *******************************************************************************/

/*
 * Secured Door Lock Demo device code:
 *
 * This sample device uses iot-nxpa71ch-c client library to connect to IBM Watson IoT Platform
 * and, send and receive messages from back end Secured Door Lock demo application.
 *
 */

#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>

/* 
 * DEV_NOTES:
 * Include for this sample to use WIoTP iot-nxpa71ch-c device client APIs.
 */ 
#include "iotfclient.h"

char *configFilePath = NULL;
volatile int interrupt = 0;
char *progname = "securedDoorLock";
int lockStatus = 0;
iotfclient client;
char *deviceId = NULL;
char *typeId = NULL;

/* Usage text */
void usage(void) {
    fprintf(stderr, "Usage: %s --config config_file_path\n", progname);
    exit(1);
}

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    fprintf(stdout, "Received signal: %d\n", signo);
    interrupt = 1;
}

/* Get and process command line options */
void getopts(int argc, char** argv)
{
    int count = 1;

    while (count < argc)
    {
        if (strcmp(argv[count], "--config") == 0)
        {
            if (++count < argc)
                configFilePath = argv[count];
            else
                usage();
        }
        count++;
    }
}

/*
 * Publish lock status
 */
void publishLockStatus(void)
{
    int rc = 0;
    char payload[1024];
    fprintf(stdout, "Send status event\n");
    if ( lockStatus == 0 ) {
        sprintf(payload, "{\"deviceId\":\"%s\", \"typeId\":\"%s\", \"lockStatus\": \"Closed\"}", deviceId, typeId);
        fprintf(stdout, "PAYLOAD: %s\n", payload);
        rc = publishEvent(&client,"status","json", payload, QoS0);
    } else {
        sprintf(payload, "{\"deviceId\":\"%s\", \"typeId\":\"%s\", \"lockStatus\": \"Opened\"}", deviceId, typeId);
        fprintf(stdout, "PAYLOAD: %s\n", payload);
        rc = publishEvent(&client,"status","json", payload, QoS0);
    }
    fprintf(stdout, "RC from publishEvent(): %d\n", rc);

}

/* 
 * Device command callback function
 * Device developers can customize this function based on their use case
 * to handle device commands sent by WIoTP.
 * Set this callback function using API setCommandHandler().
 */
void  deviceCommandCallback (char* type, char* id, char* commandName, char *format, void* payload, size_t payloadSize)
{
    if ( type == NULL && id == NULL ) {
        fprintf(stdout, "Received device management status:");

    } else {
        fprintf(stdout, "Received device command:\n");
        fprintf(stdout, "Type=%s ID=%s CommandName=%s Format=%s Len=%d\n", type, id, commandName, format, (int)payloadSize);
        fprintf(stdout, "Payload: %s\n", (char *)payload);
    }

    /*  process device commands */
    if ( commandName && *commandName != '\0' ) {
        if (!strcmp(commandName, "openDoor")) {
            fprintf(stdout, "Change lock status to open\n");
            lockStatus = 1;
            publishLockStatus();
        } else if (!strcmp(commandName, "closeDoor")) {
            fprintf(stdout, "Change lock status to close\n");
            lockStatus = 0;
            publishLockStatus();
        } else if (!strcmp(commandName, "disconnect")) {
            fprintf(stdout, "Change lock status to disconnect\n");
            lockStatus = -1;
            publishLockStatus();
            interrupt = 1;
        } else if (!strcmp(commandName, "sendStatus")) {
            fprintf(stdout, "Send current lock status\n");
            publishLockStatus();
        }
    }
}


/* Main program */
int main(int argc, char *argv[])
{
    int rc = 0;

    /* check for args */
    if ( argc < 2 )
        usage();

    /* Set signal handlers */
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    /* get argument options */
    getopts(argc, argv);

    /* 
     * Initialize logging 
     * Specify logging level and log file path.
     * Supported log levels are LOGLEVEL_ERROR, LOGLEVEL_WARN, LOGLEVEL_INFO, LOGLEVEL_DEBUG, LOGLEVEL_TRACE
     * If log file path is NULL, the library use default log file path ./iotfclient.log 
     */
    initLogging(LOGLEVEL_INFO, NULL);

    /* 
     * Initialize iotf configuration using configuration options defined in the configuration file.
     * The variable configFilePath is set in getopts() function.
     * The initialize_configfile() function can be used to initialize both device and gateway client. 
     * Since this sample is for device client, set isGatewayClient to 0.
     */
    int isGatewayClient = 0;
    rc = initialize_configfile(&client, configFilePath, isGatewayClient);
    if ( rc != 0 ) {
        fprintf(stderr, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
        exit(1);
    }

    /* 
     * Invoke connection API connectiotf() to connect to WIoTP.
     */
    int loop=1;
    while (loop) {
        rc = connectiotf(&client);
        if ( rc != 0 ) {
            /* check if authorizarion failure - device may not be registered yet. Wait for some time and retry */
            if ( rc == 5 ) {
                fprintf(stderr, "WARN: Connect call to Watson IoT Platform returned authorization error. Wait for 10 seconds and retry\n");
                sleep(10);
                continue;
            } else {
                fprintf(stderr, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
                exit(1);
            }
        } else {
            loop = 0;
        }
    }

    deviceId = client.cfg.id;
    typeId = client.cfg.type;

    /*
     * Set device command callback using API setCommandHandler().
     * Refer to deviceCommandCallback() function DEV_NOTES for details on
     * how to process device commands received from WIoTP.
     */
    setCommandHandler(&client, deviceCommandCallback);

    /*
     * Invoke device command subscription API subscribeDeviceCommands().
     * The arguments for this API are commandName, format, QoS
     * If you want to subscribe to all commands of any format, set commandName and format to "+"
     * Note that client library also provides a convinence function subscribeCommands(iotfclient *client)
     * to subscribe to all commands. The QoS used in this convience function is QoS0. 
     */
    char *commandName = "+";
    char *format = "+";
    subscribeCommand(&client, commandName, format, QoS0);


    /*
     * Use publishEvent() API to send device events to Watson IoT Platform.
     */
    while(!interrupt)
    {
        publishLockStatus();
        sleep(10);
    }

    fprintf(stdout, "Received a signal - exiting publish event cycle.\n");

    /*
     * Disconnect device
     */
    disconnect(&client);

    return 0;

}

