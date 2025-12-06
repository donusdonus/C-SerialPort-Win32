
#ifndef __LIB__SERIALPORT_WIN32__H
#define __LIB__SERIALPORT_WIN32__H



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <control.h>
#include <stdarg.h>

#define _INCLUDE_FREERTOS_
#ifdef _INCLUDE_FREERTOS_
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#define __SERIAL_START_BLOCK__(s,t) if (xSemaphoreTake(s,t)){
#define __SERIAL_END_BLOCK__(s)  xSemaphoreGive(_Blocking);}  
#endif

#define __Serial_RX_BUFFER_SIZE 128
#define __Serial_Path 32

#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class SerialPort_Win32 {
public:             
    SerialPort_Win32();
    ~SerialPort_Win32();

    bool begin(const char *PortName,int Baudrate,int Option = SERIAL_8N1);
    //bool begin(const char *PortName,int Baudrate,uint8_t Option);
    bool end();
    int available(void);
    void flush(void);
    size_t write(uint8_t data);
    size_t write(uint8_t *data,size_t size);

    /* Maximum Send output 120 char */
    size_t print(const char* fmt, ...);
    size_t read(uint8_t *data,size_t size);
    int Config(int Baudrate,int Option);
    void SetTimeout(int TimeoutInterval,int TimeoutMultiplier,int TimeoutConstant);
    int GetError() { return GetLastError();} 


private:
    bool tmp = false;
    uint8_t BufferReader[__Serial_RX_BUFFER_SIZE];
    char SerialPath[__Serial_Path];

    int Config(int Baudrate,uint8_t DataBits,uint8_t StopBits,uint8_t ParityBit);
    bool Ready();

    /******* SerialPort Parameter */
    HANDLE _SerialPort = nullptr;
    DCB _SerialOption = {0};
    COMMTIMEOUTS _SerialTimeout = {0};

#ifdef _INCLUDE_FREERTOS_
    xSemaphoreHandle _Blocking = xSemaphoreCreateMutex();
#endif
    
}; 
#endif // __LIB__SERIALPORT_WIN32__H
            