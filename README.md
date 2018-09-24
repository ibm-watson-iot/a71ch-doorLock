# NXP A71CH - Ready for IBM Watson IoT
## Door Lock Demo Application

NXP A71CH Ready for IBM Watson IoT is an A71CH device that has been trust provisioned 
by NXP with credentials (one EC key pair, one Device X509 certificate and one Gateway 
X509 certificate) supporting TLS access to Watson IoT Platform service.

The Door Lock demo application demonstrates:

* How to provision a device with NXP A71CH Ready for IBM Watson IoT?
* Connect the device securely with IBM Watson IoT Platform service.
* Invoke remote control actions on the device.

## Deploy the application on Bluemix
You can deploy your own instance of this application in IBM Cloud.
To do this, you can either use the [_Create Toolchain_](http://ibm.biz/BdiKSi) button for an automated deployment or follow the steps below to create and deploy manually.

### Using the Create Toolchain  button.
Use the *"Create Toolchain"* button below, to deploy this app to IBM Cloud using IBM DevOps.
If you are in the US South region, use the below button

[![Create Toolchain](https://console.ng.bluemix.net/devops/graphics/create_toolchain_button.png)](http://ibm.biz/BdiKSi)

For other regions, please click the below *Deploy to Bluemix*

[![Deploy to Bluemix](https://console.ng.bluemix.net/devops/setup/deploy/button_x2.png)](http://ibm.biz/Bdi8VY)

### Manually deploying the App in Bluemix

1. Create a Bluemix Account.

  [Sign up][bluemix_signup_url] for IBM Cloud, or use an existing account.

2. Download and install the [Cloud-foundry CLI][cloud_foundry_url] tool.

3. Clone the app to your local environment from your terminal using the following command:

  ```
  git clone https://github.com/ibm-watson-iot/a71ch-doorLock-demo.git
  ```

4. `cd` into this newly created directory.

5. Edit the `manifest.yml` file and change the `<name>` and `<host>` to something unique.

  ```
  applications:
  - path: .
    memory: 768M
    instances: 1
    domain: mybluemix.net
    name: a71ch-doorlock-1234
    host: a71ch-doorlock-1234
    disk_quota: 1024M
  ```
  The host you use will determinate your application URL initially, for example, `<host>.mybluemix.net`.

6. Connect to Bluemix in the command line tool and follow the prompts to log in:

  ```
  $ cf api https://api.ng.bluemix.net
  $ cf login
  ```
7. Create the required services in Bluemix.

  ```
  $ cf create-service iotf-service iotf-service-free a71chDoorLockDemo-iot
  $ cf create-service cloudantNoSQLDB Lite a71chDoorLockDemo-cloudantNoSQLDB
  ```

8. Push the app to Bluemix.

  ```
  $ cf push
  ```

You now have your own instance of the app running on Bluemix.  



