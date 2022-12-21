/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
char deviceId[] = "5ea9eda79acc71ca425faba5ac3ba85a"; // Device you want to push to
char streamAcc[] = "acc_rms"; // Stream you want to push to
char streamMag[] = "mag_rms"; // Stream you want to push to
char streamGyr[] = "gyr_rms"; // Stream you want to push to
char m2xKey[] = "5d52815be48c739fdd0f2a743bee3309"; // Your M2X API Key or Master API Key

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