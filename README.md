# C-SerialPort-Win32

A Windows SerialPort class designed with an Arduino-like interface. Provides familiar functions such as `begin()`, `available()`, `read()`, and `write()`, implemented internally using the Win32 API.

## Features

- **Arduino-like API**: Familiar interface for developers coming from Arduino background
- **Win32 Implementation**: Direct use of Windows Serial Communication API
- **FreeRTOS Support**: Optional FreeRTOS integration with mutex synchronization
- **Configurable Serial Parameters**: Support for various data bits, stop bits, and parity options
- **Timeout Control**: Flexible timeout configuration for read operations
- **Print Function**: Printf-style serial printing with up to 120 character support

## Installation

1. Clone or download the repository
2. Include the header file in your project:
   ```cpp
   #include "SerialPort_Win32.h"
   ```
3. Link against Windows libraries (windows.h)

## Class Declaration

```cpp
class SerialPort_Win32 {
public:             
    SerialPort_Win32();
    ~SerialPort_Win32();

    // Initialize serial port
    bool begin(const char *PortName, int Baudrate, int Option = SERIAL_8N1);
    
    // Close serial port
    bool end();
    
    // Get number of available bytes
    int available(void);
    
    // Clear the buffer
    void flush(void);
    
    // Write single byte
    size_t write(uint8_t data);
    
    // Write multiple bytes
    size_t write(uint8_t *data, size_t size);

    // Printf-style printing (max 120 characters)
    size_t print(const char* fmt, ...);
    
    // Read data
    size_t read(uint8_t *data, size_t size);
    
    // Configure baud rate and options
    int Config(int Baudrate, int Option);
    
    // Set timeout parameters
    void SetTimeout(int TimeoutInterval, int TimeoutMultiplier, int TimeoutConstant);
    
    // Get last error code
    int GetError() { return GetLastError(); } 

private:
    // Internal members and private methods
};
```

## Serial Configuration Options

```cpp
// Data bits - Stop bits - Parity
#define SERIAL_5N1 0x00  // 5 data bits, 1 stop bit, no parity
#define SERIAL_6N1 0x02  // 6 data bits, 1 stop bit, no parity
#define SERIAL_7N1 0x04  // 7 data bits, 1 stop bit, no parity
#define SERIAL_8N1 0x06  // 8 data bits, 1 stop bit, no parity (default)
#define SERIAL_8N2 0x0E  // 8 data bits, 2 stop bits, no parity

// With Even Parity
#define SERIAL_8E1 0x26  // 8 data bits, 1 stop bit, even parity

// With Odd Parity
#define SERIAL_8O1 0x36  // 8 data bits, 1 stop bit, odd parity
// ... and many more combinations
```

## Usage Examples

### Basic Example: Send and Receive Data

```cpp
#include <stdio.h>
#include "SerialPort_Win32.h"

SerialPort_Win32 Serial;

char input[120];
int inputBuffer = 0;
int bufferRecived = 0;

int main(void)
{
    printf("Software Started\r\n\r\n");
    
    // Initialize serial port on COM7, 9600 baud, 8N1
    Serial.begin("COM7", 9600, SERIAL_8N1);
    
    // Set timeout: 20ms interval, 10 multiplier, 50ms constant
    Serial.SetTimeout(20, 10, 50);

    while(1)
    {
        printf("Send Data: ");
        fgets(input, sizeof(input), stdin);
        inputBuffer = strlen(input);
        printf("User Input Buffer: %d Bytes\n", inputBuffer);
        
        // Send data to serial port
        Serial.write((uint8_t*)&input[0], inputBuffer);

        Sleep(5000);

        // Check if data is available
        bufferRecived = Serial.available();
        if(bufferRecived > 0)
        {
            uint8_t data[bufferRecived];
            Serial.read(&data[0], bufferRecived);
            printf("Received %d Bytes: %s\n", bufferRecived, (char*)&data[0]);
            Serial.flush();
        }
    }

    return 1;
}
```

### Simple Write Example

```cpp
#include "SerialPort_Win32.h"

int main(void)
{
    SerialPort_Win32 Serial;
    Serial.begin("COM3", 115200, SERIAL_8N1);
    
    // Write single byte
    uint8_t byte = 0x41;  // 'A'
    Serial.write(byte);
    
    // Write string
    const char* msg = "Hello Serial!";
    Serial.write((uint8_t*)msg, strlen(msg));
    
    Serial.end();
    return 0;
}
```

### Printf-Style Printing

```cpp
#include "SerialPort_Win32.h"

int main(void)
{
    SerialPort_Win32 Serial;
    Serial.begin("COM3", 9600, SERIAL_8N1);
    
    int value = 42;
    float temperature = 23.5f;
    
    // Print formatted data (max 120 characters)
    Serial.print("Temperature: %d.%d C\n", (int)temperature, (int)((temperature - (int)temperature) * 10));
    Serial.print("Value: %d\n", value);
    
    Serial.end();
    return 0;
}
```

### Reading Data

```cpp
#include "SerialPort_Win32.h"

int main(void)
{
    SerialPort_Win32 Serial;
    Serial.begin("COM3", 9600, SERIAL_8N1);
    Serial.SetTimeout(100, 10, 50);
    
    while(1)
    {
        int available = Serial.available();
        if(available > 0)
        {
            uint8_t buffer[available];
            Serial.read(buffer, available);
            
            // Process received data
            printf("Received %d bytes\n", available);
            Serial.flush();
        }
    }
    
    Serial.end();
    return 0;
}
```

## Method Details

### `bool begin(const char *PortName, int Baudrate, int Option)`

Opens and initializes the serial port.

**Parameters:**
- `PortName`: Port identifier (e.g., "COM1", "COM3", "COM7")
- `Baudrate`: Communication speed (e.g., 9600, 115200)
- `Option`: Serial configuration (default: SERIAL_8N1)

**Returns:** `true` if successful, `false` otherwise

**Example:**
```cpp
if (Serial.begin("COM7", 9600, SERIAL_8N1)) {
    printf("Port opened successfully\n");
} else {
    printf("Failed to open port\n");
}
```

### `int available(void)`

Returns the number of bytes available to read from the input buffer.

**Returns:** Number of bytes available

**Example:**
```cpp
int bytesAvailable = Serial.available();
if (bytesAvailable > 0) {
    // Data is available to read
}
```

### `size_t write(uint8_t data)` and `size_t write(uint8_t *data, size_t size)`

Writes data to the serial port.

**Parameters (single byte):**
- `data`: Byte to send

**Parameters (multiple bytes):**
- `data`: Pointer to data buffer
- `size`: Number of bytes to send

**Returns:** Number of bytes written

**Example:**
```cpp
uint8_t buffer[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
size_t written = Serial.write(buffer, 5);
printf("Wrote %zu bytes\n", written);
```

### `size_t read(uint8_t *data, size_t size)`

Reads data from the serial port.

**Parameters:**
- `data`: Pointer to receive buffer
- `size`: Number of bytes to read

**Returns:** Number of bytes read

**Example:**
```cpp
uint8_t buffer[128];
size_t read = Serial.read(buffer, 128);
printf("Read %zu bytes\n", read);
```

### `void SetTimeout(int TimeoutInterval, int TimeoutMultiplier, int TimeoutConstant)`

Configures timeout behavior for read operations.

**Parameters:**
- `TimeoutInterval`: Base timeout interval in milliseconds
- `TimeoutMultiplier`: Multiplier for each byte
- `TimeoutConstant`: Constant timeout in milliseconds

**Example:**
```cpp
Serial.SetTimeout(20, 10, 50);  // Flexible timeout configuration
```

### `void flush(void)`

Clears the receive buffer.

**Example:**
```cpp
Serial.flush();  // Clear any remaining data in buffer
```

## FreeRTOS Support

The class includes optional FreeRTOS integration for thread-safe operations:

```cpp
#define _INCLUDE_FREERTOS_
#ifdef _INCLUDE_FREERTOS_
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif
```

When FreeRTOS is included, the serial port operations are protected by a mutex semaphore for safe concurrent access from multiple tasks.

## Building the Project

The project includes CMake configuration:

```bash
cd build
cmake ..
make
```

Or use the provided batch files on Windows:

```batch
cmd/build.bat      # Build the project
cmd/run.bat        # Run the application
cmd/clean.bat      # Clean build artifacts
```

## Files Structure

- `lib/SerialPort_Win32/`: Core serial port implementation
  - `SerialPort_Win32.h`: Class header
  - `SerialPort_Win32.cpp`: Implementation
  - `CmakeLists.txt`: CMake configuration
  
- `examples/SendRecived/`: Basic send/receive example
- `examples/SendRecived_FreeRTOS/`: FreeRTOS integrated example
- `src/main.cpp`: Main application

## Error Handling

```cpp
int error = Serial.GetError();
if (error != 0) {
    printf("Serial port error code: %d\n", error);
}
```
