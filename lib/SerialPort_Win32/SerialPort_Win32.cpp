
#include "SerialPort_Win32.h"

bool SerialPort_Win32::begin(const char *PortName, int Baudrate, int Option = SERIAL_8N1)
{
    sprintf(&SerialPath[0], "\\\\.\\%s", PortName);

    /*
        reference function link
        https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
     */
    _SerialPort = CreateFileA(
        SerialPath,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    tmp = (_SerialPort == INVALID_HANDLE_VALUE);
    if (tmp)
        return false;

    tmp = (Config(Baudrate,Option) == 0 );
    if(tmp)
        return false;
    return true;
}

int SerialPort_Win32::Config(int Baudrate, uint8_t DataBits, uint8_t StopBits, uint8_t ParityBit)
{
    _SerialOption.DCBlength = sizeof(_SerialOption);

    _SerialOption.BaudRate = Baudrate;
    _SerialOption.ByteSize = DataBits;
    _SerialOption.StopBits = StopBits;
    _SerialOption.Parity = ParityBit;

    tmp = (_SerialPort == nullptr);
    if (tmp)
        return -1;

    tmp = (_SerialPort == INVALID_HANDLE_VALUE);
    if (tmp == false)
    {
        return GetLastError();
    }

    return SetCommState(_SerialPort, &_SerialOption);
}

int SerialPort_Win32::Config(int Baudrate, int Option)
{
    switch (Option)
    {
    case SERIAL_5N1:
        return Config(Baudrate, 5, ONESTOPBIT, NOPARITY);
        break;
    case SERIAL_6N1:
        return Config(Baudrate, 6, ONESTOPBIT, NOPARITY);
        break;
    case SERIAL_7N1:
        return Config(Baudrate, 7, ONESTOPBIT, NOPARITY);
        break;
    case SERIAL_8N1:
        return Config(Baudrate, 8, ONESTOPBIT, NOPARITY);
        break;
    case SERIAL_5N2:
        return Config(Baudrate, 5, TWOSTOPBITS, NOPARITY);
        break;
    case SERIAL_6N2:
        return Config(Baudrate, 6, TWOSTOPBITS, NOPARITY);
        break;
    case SERIAL_7N2:
        return Config(Baudrate, 7, TWOSTOPBITS, NOPARITY);
        break;
    case SERIAL_8N2:
        return Config(Baudrate, 8, TWOSTOPBITS, NOPARITY);
        break;
    case SERIAL_5E1:
        return Config(Baudrate, 5, ONESTOPBIT, EVENPARITY);
        break;
    case SERIAL_6E1:
        return Config(Baudrate, 6, ONESTOPBIT, EVENPARITY);
        break;
    case SERIAL_7E1:
        return Config(Baudrate, 7, ONESTOPBIT, EVENPARITY);
        break;
    case SERIAL_8E1:
        return Config(Baudrate, 8, ONESTOPBIT, EVENPARITY);
        break;
    case SERIAL_5E2:
        return Config(Baudrate, 5, TWOSTOPBITS, EVENPARITY);
        break;
    case SERIAL_6E2:
        return Config(Baudrate, 6, TWOSTOPBITS, EVENPARITY);
        break;
    case SERIAL_7E2:
        return Config(Baudrate, 7, TWOSTOPBITS, EVENPARITY);
        break;
    case SERIAL_8E2:
        return Config(Baudrate, 8, TWOSTOPBITS, EVENPARITY);
        break;
    case SERIAL_5O1:
        return Config(Baudrate, 5, ONESTOPBIT, ODDPARITY);
        break;
    case SERIAL_6O1:
        return Config(Baudrate, 6, ONESTOPBIT, ODDPARITY);
        break;
    case SERIAL_7O1:
        return Config(Baudrate, 7, ONESTOPBIT, ODDPARITY);
        break;
    case SERIAL_8O1:
        return Config(Baudrate, 8, ONESTOPBIT, ODDPARITY);
        break;
    case SERIAL_5O2:
        return Config(Baudrate, 5, TWOSTOPBITS, ODDPARITY);
        break;
    case SERIAL_6O2:
        return Config(Baudrate, 6, TWOSTOPBITS, ODDPARITY);
        break;
    case SERIAL_7O2:
        return Config(Baudrate, 7, TWOSTOPBITS, ODDPARITY);
        break;
    case SERIAL_8O2:
        return Config(Baudrate, 8, TWOSTOPBITS, ODDPARITY);
        break;
    }
}

bool SerialPort_Win32::end()
{
    return false;
}

int SerialPort_Win32::available(void)
{
    COMSTAT comStat;
    DWORD errors;
    ClearCommError(_SerialPort, &errors, &comStat);

    // comStat.cbInQue = จำนวน byte ที่รออยู่ใน input buffer
    DWORD availableBytes = comStat.cbInQue;
    return (int)availableBytes;
}

int SerialPort_Win32::peek(void)
{
    return 0;
}

void SerialPort_Win32::flush(void)
{
     PurgeComm(_SerialPort,PURGE_RXCLEAR);   
}

size_t SerialPort_Win32::write(uint8_t data)
{
    return 0;
}

size_t SerialPort_Win32::write(uint8_t *data, size_t size)
{
    return 0;
}

int SerialPort_Win32::read(void)
{
    return 0;
}

size_t SerialPort_Win32::read(uint8_t *data, size_t size)
{
    return 0;
}