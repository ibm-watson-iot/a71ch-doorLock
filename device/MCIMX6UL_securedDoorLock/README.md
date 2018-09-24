# NXP A71CH Enabled Door Lock Application
## Device code for _NXP i_._MX_ Platform with IC A71CH Secure Element

This project contains source for device client for NXP A71CH Enabled Door Lock demo
Application.

## Prerequisite

The client code code can be built only on _NXP i_._MX_ Platform. You need a work
combination of *MCIMX6UL-EVKB* and *A71CH* boards. Complete the steps described in 
the following GitHub project:

[IBM Watson™ IoT Platform C Client Library - For _NXP i_._MX_ Platform with IC A71CH Secure Element](https://github.com/ibm-watson-iot/iot-nxpimxa71ch-c)

## Build and Install Door Lock demo application client

Use the following steps to build the application client:

* Create a directory named "doorLock" in iot-nxpimxa71ch-c directory.
* Copy contents of this directory in "doorLock" directory.
* Use the following commands to build and install the demo application client.

```
make -C doorLock build
make -C doorLock install
```

## Run Door Lock demo application client

Use the following steps to run this client:

```
cd doorLock
bash startDoorLockClient.sh
```

This script performance the following tasks:

* Retrieves Secure Element UID
* Sends a registration request to the provisioing service provided by Door Lock Demo Application running in IBM Bluemix.
* Download client configuration data from provisioing service.
* Connect with Watson IoT Platform servce. 

Note that to connect the demo client with Watson IoT Platform service, Door Lock Demo 
application administrator will have to approve the device using device approval option provided 
by the application.


