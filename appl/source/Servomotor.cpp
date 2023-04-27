#include "Servomotor.hpp"
#include "Utils.hpp"

#include <stdlib.h>
#include <iostream>

using namespace std;

Servomotor::Servomotor(int _pin) : pin(_pin) {
    script = "python3 ";
    script.append(SM_PATH);

    lastAngle = 0;
    currAngle = 0;
}

Servomotor::~Servomotor() {

}

void Servomotor::setAngle(int angle) {
    string command = script + " " + to_string(pin) + " " + to_string(angle);
    int result = system(command.c_str());
    cout << result << endl;
    lastAngle = currAngle;
    currAngle = angle;
}