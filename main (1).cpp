#include "mbed.h"
#include "LocalFileSystem.h"
#include "C12832.h"
#include "LM75B.h"

LocalFileSystem local("local");
C12832 lcd(p5, p7, p6, p8, p11);
LM75B temp_sensor(p28, p27);

int main() {
    FILE *fp = fopen("/local/temperature.txt", "a");
    if(fp == NULL) {
        error("Could not open file!");
    }
    fclose(fp);
    
    float min_temp = 100;
    float max_temp = -100;
    float avg_temp = 0;
    int count = 0;
    
    while(1) {
        // Read temperature from sensor
        float temp = temp_sensor.temp();
        
        // Log temperature to file
        fp = fopen("/local/temperature.txt", "a");
        if(fp == NULL) {
            error("Could not open file!");
        }
        fprintf(fp, "%.2f\n", temp);
        fclose(fp);
        
        // Update statistics
        count++;
        avg_temp = avg_temp + (temp - avg_temp) / count;
        if (temp < min_temp) {
            min_temp = temp;
        }
        if (temp > max_temp) {
            max_temp = temp;
        }
        
        // Display statistics on LCD
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Min: %.2f", min_temp);
        lcd.locate(0, 10);
        lcd.printf("Max: %.2f", max_temp);
        lcd.locate(0, 20);
        lcd.printf("Avg: %.2f", avg_temp);
        
        // Wait for 5 minutes
        wait(5 * 60);
    }
}
