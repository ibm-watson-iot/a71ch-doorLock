#!/bin/bash
#
#
# NXP A71CH enabled door lock application demo client
# 
# Script to configure and start Watson IoT Platform client 
# to connect to Watson IoT Platform service provisioned and 
# configured by a71ch-doorLock-demo application. 
#
# Prereq:
# Complete steps described in github project before running this script:
# https://github.com/ibm-watson-iot/iot-nxpimxa71ch-c
#
# For details on a71ch-doorLock-demo application, refer to
# https://github.com/ibm-watson-iot/a71ch-doorLock-demo
#
# This script needs doorLock client to be installed.
# 

# Change demo application name deployed in IBM Cloud
DOOR_LOCK_APP=a71chdoorlockdemo
export DOOR_LOCK_APP


# 
# Do not make any changes beyond this line
#
CURDIR=`pwd`
export CURDIR

# Use a71ch config tool to get UUID of Secure Element 
UUID=$(/home/root/tools/a71chConfig_i2c_imx info device | tail -1 | sed 's/://g')
export UUID

if [ "${UUID}" == "" ]
then
    echo "ERROR: Could not retrieve Secure Element UID"
    exit 1
fi


# Invoke a registration request to provisoing service provided 
# by the Door Lock application. Device registration request 
# is reviewed, and approved by the Administrator. On Approval, the
# device is registered with the IBM Watson IoT Platform.

echo "Send registeration request to provisioing service:"
echo "Device Type: NXP-A71CH-D"
echo "Device ID: ${UUID}"

counter=0
while [ $counter -eq 0 ]; do
    rm -f ${CURDIR}/configFromProvisioingService.cfg
    curl -k -X POST https://${DOOR_LOCK_APP}.mybluemix.net/registerMe \
        --output ${CURDIR}/configFromProvisioingService.cfg --silent \
        --header "Content-Type: application/json" \
        --data "{\"typeId\":\"NXP-A71CH-D\",\"deviceId\":\"${UUID}\"}"

    # Check if device registartion request is received by registartion service
    grep "id=${UUID}" ${CURDIR}/configFromProvisioingService.cfg > /dev/null
    if [ $? -eq 0 ]
    then
        echo "Registartion request is received by provisioing service provided by Demo Application."
        counter=1
    else
        echo "Registration request is not received by provisioing service"
        echo "Try again after 10 seconds"
        sleep 10
    fi
done

# Start the device
echo "Wait for some time for Administartor to approve registration request."
sleep 10
echo "Configure Door Lock device"

cp ${CURDIR}/configFromProvisioingService.cfg ${CURDIR}/doorLock.cfg

/opt/iotnxpimxclient/bin/doorLock --config ${CURDIR}/doorLock.cfg

