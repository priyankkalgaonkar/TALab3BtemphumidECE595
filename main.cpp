#include "mbed.h"
#include "DHT.h"
  
DHT sensor(D4, DHT11);

int main()
{
    int error = 0;
    float h = 0.0f, c = 0.0f, f = 0.0f, k = 0.0f, dp = 0.0f, dpf = 0.0f;

    while(1) {
        thread_sleep_for(2000);
        error = sensor.readData();
        if (0 == error) {
            c   = sensor.ReadTemperature(CELCIUS);
            f   = sensor.ReadTemperature(FARENHEIT);
            k   = sensor.ReadTemperature(KELVIN);
            h   = sensor.ReadHumidity();
            dp  = sensor.CalcdewPoint(c, h);
            dpf = sensor.CalcdewPointFast(c, h);
            printf("Temperature in Kelvin: %4.2f, Celcius: %4.2f, Farenheit %4.2f\n", k, c, f);
            printf("Humidity is %4.2f, Dewpoint: %4.2f, Dewpoint fast: %4.2f\n", h, dp, dpf);
        } else {
            printf("Error: %d\n", error);
        }
    }
}