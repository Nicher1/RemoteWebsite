#include <windows.h>
#include <iostream>

#define IR_REMOTE_1_1 "FFA25D"
#define IR_REMOTE_2_1 "FF629D"

int main() {
    // Open the serial port
    HANDLE hSerial = CreateFile("COM3",
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                0,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    // Set parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    // Read data
    DWORD bytesRead;
    const DWORD bufSize = 256;
    char tmpChar;
    std::string receivedData;

    while (1) {
        if (ReadFile(hSerial, &tmpChar, 1, &bytesRead, NULL) && bytesRead > 0) {
            // Append the read character to the string
            receivedData += tmpChar;

            // Check for the delimiter
            if (tmpChar == '\n') { // Modify if your delimiter is different
                // Process the data
                std::cout << "Data read from serial port: " << receivedData;

                // Clear the received data for the next message
                receivedData.clear();
            }
        } else {
            // Handle error or no data
            std::cerr << "Error reading from serial port or no data available" << std::endl;
            Sleep(1000);
        }
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}