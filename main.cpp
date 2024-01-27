#include <windows.h>
#include <iostream>
#include <string>

#define IR_REMOTE_1_1 0xFFA25D
#define IR_REMOTE_2_1 0xFF629D


class Remote {
  public:
    uint8_t mode; // 1: Calculator, 2: Current Time, 3: Switch LED
    unsigned long heldTime; // Time since the button is pressed

    Remote() : mode(0), heldTime(0) {} // Constructor to initialize variables

    void updateMode(uint8_t newMode) {
      mode = newMode;
      heldTime = millis(); // Update the time when mode changes
    }

    unsigned long getHeldDuration() {
      if (mode != 0) { // Check if a mode is active
        return millis() - heldTime; // Return how long the button has been held
      }
      return 0;
    }

    void reset() {
      mode = 0; // Reset mode
      heldTime = 0; // Reset held time
    }
};

void button_command(u_int64 command){
    switch (command) {
    case IR_REMOTE_1_1:
        std::cout << "Command 1 was pressed \n";
        break;

    case IR_REMOTE_2_1:
        std::cout << "Command 2 was pressed \n";
        break;

    default:
        break;
    }
}

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
    const DWORD bufSize = 256;
    char tmpChar;
    std::string receivedData;
    u_int64 command;

    while (1) {
        if (ReadFile(hSerial, &tmpChar, 1, &bytesRead, NULL) && bytesRead > 0) {
            // Append the read character to the string
            receivedData += tmpChar;

            // Check for the delimiter
            if (tmpChar == '\n') { // Modify if your delimiter is different
                // Process the data
                std::cout << "Data read from serial port: " << receivedData;

                try {
                    command = stoi(receivedData, 0, 16); 
                    button_command(command);
                }
                catch(const std::exception& e) {
                    std::cerr << e.what() << '\n';
                }
                
                
                // Clear the received data for the next message
                receivedData.clear();
            }
        } else {
            // Handle error or no data
            std::cerr << "Error reading from serial port or no data available" << std::endl;
        }
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
