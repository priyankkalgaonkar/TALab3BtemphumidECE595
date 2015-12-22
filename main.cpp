 /* Copyright (c) 2015 NXP Semiconductors. MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "FXAS21002.h"
#include "FXOS8700.h"  
#include "mbed.h"
#include "M2XStreamClient.h"
#include "EthernetInterface.h"

// Initialize Serial port
Serial pc(USBTX, USBRX);

// Initialize pins for I2C communication for sensors. Set jumpers J6,J7 in FRDM-STBC-AGM01 board accordingly.
FXOS8700 accel(D14,D15);
FXOS8700 mag(D14,D15);
FXAS21002 gyro(D14,D15);

// Set Sensor Stream details
char deviceId[] = "8b34bc421abf15b7ec6471fa19513a98"; // Device you want to push to
char streamAcc[] = "acc_rms"; // Stream you want to push to
char streamMag[] = "mag_rms"; // Stream you want to push to
char streamGyr[] = "gyr_rms"; // Stream you want to push to
char m2xKey[] = "737018ea33de7760ab346c85ae2d9d27"; // Your M2X API Key or Master API Key

int main()
{
    // Intialize Ethernet connection
EthernetInterface eth;
eth.init();
eth.connect();
printf("Success. Connected!. Device IP Address is %s\r\n", eth.getIPAddress());

 // Initialize the M2X client
Client client;
M2XStreamClient m2xClient(&client, m2xKey);
int ret;


    // Configure Accelerometer FXOS8700, Magnetometer FXOS8700 & Gyroscope FXAS21002
    accel.accel_config();
    mag.mag_config();
    gyro.gyro_config();

    float accel_data[3]; float accel_rms=0.0;
    float mag_data[3];   float mag_rms=0.0;
    float gyro_data[3];  float gyro_rms=0.0;
       
    printf("Begin Data Acquisition from FXOS8700 and FXAS21002....\r\n\r\n");
    wait(0.5);
    
    while(1)
    {
      accel.acquire_accel_data_g(accel_data);
      accel_rms = sqrt(((accel_data[0]*accel_data[0])+(accel_data[1]*accel_data[1])+(accel_data[2]*accel_data[2]))/3);
      printf("%4.2f,\t%4.2f,\t%4.2f,\t",accel_data[0],accel_data[1],accel_data[2]);
      wait(0.01);
      
      mag.acquire_mag_data_uT(mag_data);
      printf("%4.2f,\t%4.2f,\t%4.2f,\t",mag_data[0],mag_data[1],mag_data[2]);
      mag_rms = sqrt(((mag_data[0]*mag_data[0])+(mag_data[1]*mag_data[1])+(mag_data[2]*mag_data[2]))/3);
      wait(0.01);
      
      gyro.acquire_gyro_data_dps(gyro_data);
      printf("%4.2f,\t%4.2f,\t%4.2f\r\n",gyro_data[0],gyro_data[1],gyro_data[2]);
      gyro_rms = sqrt(((gyro_data[0]*gyro_data[0])+(gyro_data[1]*gyro_data[1])+(gyro_data[2]*gyro_data[2]))/3);
      wait(0.01);
      
      ret = m2xClient.updateStreamValue(deviceId, streamAcc, accel_rms);
      printf("send() returned %d\r\n", ret);
      ret = m2xClient.updateStreamValue(deviceId, streamMag, mag_rms);
      printf("send() returned %d\r\n", ret);
      ret = m2xClient.updateStreamValue(deviceId, streamGyr, gyro_rms);
      printf("send() returned %d\r\n", ret);
      wait(1);
    }
      
}