# NXP A71CH Enabled Door Lock Application
## Device code for _NXP FRDM-K64F boards with IC A71CH Secure Element

This project contains source for device client for NXP A71CH Enabled Door Lock demo Application.

## Prerequisite

The client code code can be built MCUXpresso IDE. Refer to the Section 5 of the following documents for details on
how to build client code for FRDM-K64F:

[A71CH for secure connection to IBM Watson IoT](https://www.nxp.com/docs/en/application-note/AN12186.pdf) 


## Build, publish and run Door Lock demo application client

Copy the following files added in this github project in "mbedtls_frdm64f_freertos_ibm_watson_demo" project in MCUXpresso IDE:

* ibm_watson_demo.c
* ibm_iot_config.h

Edit *ibm_iot_config.h* file and change Watson IoT Platform service organization ID.

Use the steps as defined in Section 5 of "A71CH for Secure connection to IBM Watson IoT" document, 
to run and publish the demo application
