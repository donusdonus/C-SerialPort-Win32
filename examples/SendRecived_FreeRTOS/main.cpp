
#include <stdio.h>
#include "SerialPort_Win32.h"

#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <portmacro.h>
#include <task.h>
#include <event_groups.h>

SerialPort_Win32 Serial;

void SerialRecived(void *pvParameters)
{
    int bufferRecived = 0;
    for(;;)
    {
        bufferRecived = Serial.available();
        if(bufferRecived > 0)
        {
            uint8_t data[bufferRecived];
            Serial.read(&data[0],bufferRecived);
            printf("Recived %d Bytes : %s\n",bufferRecived,(char*)&data[0]);
            Serial.flush();
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void SerialSend(void *pvParameter)
{
    char input[120];
    int inputBuffer = 0;
    for(;;)
    {
       printf("Send Data : ");

       fgets(input,sizeof(input),stdin);

       inputBuffer = strlen(input);

       printf("User Input Buffer : %d Bytes\n",inputBuffer);

       Serial.write((uint8_t*)&input[0],inputBuffer);
    }
}

int main(void)
{
    printf("Software Started\r\n\r\n");
    Serial.begin("COM7",9600,SERIAL_8N1);

    Serial.SetTimeout(20,10,50);

    Serial.print("Hello World\r\n");

    xTaskCreate(
                    SerialRecived,
                    "SerialRecived",
                    2048,
                    NULL,
                    tskIDLE_PRIORITY,
                    NULL
               );

    xTaskCreate(
                    SerialSend,
                    "SerialSend",
                    2048,
                    NULL,
                    tskIDLE_PRIORITY,
                    NULL
               );


    vTaskStartScheduler();
    return 1 ;
}

        