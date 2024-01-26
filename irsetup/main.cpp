#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "SerialPort.h"

using namespace std;

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];

// ændre COM-porten til den rigtige port
const char *port = "\\\\.\\COM3";

int main() {
    SerialPort arduino(port);
    if (arduino.isConnected()) {
        cout << "Connection established" << endl;
    } else {
        cout << "Error in port connection" << endl;
    }

    while (arduino.isConnected()) {
        int read_result = arduino.readSerialPort(output, MAX_DATA_LENGTH);
        cout << output;
        Sleep(10);
    }
}
