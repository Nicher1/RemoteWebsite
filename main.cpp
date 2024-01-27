#include <windows.h>
#include <iostream>
#include <string>
#include <cstdint> //include for uint_x variables not in std.

#define IR_REMOTE_1_1 0xFFA25D
#define IR_REMOTE_2_1 0xFF629D

// Define the 'Remote' class.
class Remote {
public:
    // Public member variables:
    uint8_t mode; // Stores the current mode of the remote. It's an 8-bit unsigned integer.
    bool isButtonHeld; // Flag to indicate if a button is currently being held down.

    // Constructor:
    // This is a special function that is called when an object of the class is created.
    // It initializes the member variables to default values.
    Remote() : mode(0), isButtonHeld(false) {}

    // Member function to update the mode of the remote.
    // 'uint8_t newMode' is a parameter that specifies the new mode to set.
    void updateMode(uint8_t newMode) {
        mode = newMode; // Update the mode with the new value.
        isButtonHeld = false; // Reset the button hold state.
    }

    // Member function to set the button hold state.
    // 'bool held' is a parameter that specifies the new state of the button (held down or not).
    void setButtonHeld(bool held) {
        isButtonHeld = held; // Update the button hold state.
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
        
            // Create an instance of the Remote class named 'myRemote'.
            Remote myRemote;

            // Example usage of the class:
            // Call the 'updateMode' function to set the mode of 'myRemote'.
            myRemote.updateMode(1);

            // Call the 'setButtonHeld' function to indicate a button is being held.
            myRemote.setButtonHeld(true);

            // Use the member variables:
            // Check and print the current mode and button hold state of 'myRemote'.
            std::cout << "Current mode: " << static_cast<int>(myRemote.mode) << std::endl;
            std::cout << "Is a button held: " << (myRemote.isButtonHeld ? "Yes" : "No") << std::endl; 
        }
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
