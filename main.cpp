#include <windows.h>
#include <iostream>
#include <string>
#include <stdint.h> //include for uint_x variables not in std.
#include "remote_values.h"

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

void button_command(uint64_t command){
    switch (command) {
    case IR_REMOTE_1_1:
        std::cout << "Command 1_1\n";
        break;

    case IR_REMOTE_1_2:
        std::cout << "Command 1_2\n";
        break;

    case IR_REMOTE_1_3:
        std::cout << "Command 1_3\n";
        break;

    case IR_REMOTE_2_1:
        std::cout << "Command 2_1\n";
        break;

    case IR_REMOTE_2_2:
        std::cout << "Command 2_2\n";
        break;

    case IR_REMOTE_2_3:
        std::cout << "Command 2_3\n";
        break;

    case IR_REMOTE_3_1:
        std::cout << "Command 3_1(minus)\n";
        break;

    case IR_REMOTE_3_2:
        std::cout << "Command 3_2\n";
        break;

    case IR_REMOTE_3_3:
        std::cout << "Command 3_3(plus)\n";
        break;

    case IR_REMOTE_4_1:
        std::cout << "Command 3_1\n";
        break;

    case IR_REMOTE_4_2:
        std::cout << "Command 4_2\n";
        break;

    case IR_REMOTE_4_3:
        std::cout << "Command 4_3\n";
        break;

    case IR_REMOTE_5_1:
        std::cout << "1\n";
        break;

    case IR_REMOTE_5_2:
        std::cout << "2\n";
        break;

    case IR_REMOTE_5_3:
        std::cout << "3\n";
        break;

    case IR_REMOTE_6_1:
        std::cout << "4\n";
        break;

    case IR_REMOTE_6_2:
        std::cout << "5\n";
        break;

    case IR_REMOTE_6_3:
        std::cout << "6\n";
        break;

    case IR_REMOTE_7_1:
        std::cout << "7\n";
        break;

    case IR_REMOTE_7_2:
        std::cout << "8\n";
        break;

    case IR_REMOTE_7_3:
        std::cout << "9\n";
        break;

    default:
        std::cout << "HOLD YOUR HAND STILL YOU F*CKING IDIOT!\n";
        break;
    }
}

uint64_t read_from_serial(HANDLE hSerial){
    std::string data;
    uint64_t dataInt;
    DWORD bytesRead;
    const DWORD bufSize = 256;
    char tmpChar;

    while(1){
        
        if (ReadFile(hSerial, &tmpChar, 1, &bytesRead, NULL) && bytesRead > 0) {
            // Append the read character to the string

            // Check for the delimiter
            if (tmpChar == '\n') { // Modify if your delimiter is different
                // Process the data

                //std::cout << data;

                try {
                    dataInt = stoi(data, 0, 16); 
                    }
                    catch(const std::exception& e) {
                    //std::cerr << e.what() << '\n';
                    }
                Sleep(100);
                return dataInt;
            }
        
            data += tmpChar;

        } else {
            // Handle error or no data
            return 0;
        
        }
    }
}

int main() {
    /*
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
    */

    // Open the serial port

    HANDLE hSerial = CreateFile("COM7",
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

    uint64_t recievedData;

    while (1) {
        recievedData = read_from_serial(hSerial);
        if (!recievedData) {
            std::cout << "Error reading from serial or no data available";
        } else{
            button_command(recievedData);
        }
        
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
