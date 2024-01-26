#include <windows.h>
#include <iostream>

int main() {
    // Open the serial port
    HANDLE hSerial = CreateFile("COM8",
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
    char buffer[256] = {0};
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port" << std::endl;
    } else {
        std::cout << "Data read from serial port: " << buffer << std::endl;
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
