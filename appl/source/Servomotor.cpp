#include "Servomotor.hpp"
#include "Utils.hpp"

#include <stdlib.h>
#include <iostream>

#include <unistd.h>

#include <chrono>

using namespace std;

Servomotor::Servomotor(int _pinx, int _piny, shared_ptr<SocketCommunicator> _comm) : 
                        pinx(_pinx), piny(_piny), comm(_comm) {

    runningMode = MODE_AUTOMATIC;
    lastAnglex = 0;
    lastAngley = 0;
}

Servomotor::~Servomotor() {

}

void Servomotor::setAngle(int pinx, int anglex, int piny, int angley) {
    
    if(anglex != 0 && angley != 0) {
        string reqMessage = to_string(anglex) + ";" + to_string(angley);
        comm->requestServoMove(reqMessage);
        // cout << "Required servo to move: " << reqMessage << endl;
    }
}

void Servomotor::setMode(ServomotorMode mode) {
    runningMode = mode;
}

ServomotorMode Servomotor::getMode() {
    return runningMode;
}
map<string, string> Servomotor::uploadData() {
    map<string, string> cmd;
    unique_lock<mutex> lock_(tm);
    cmd.insert({});
    cmd.insert({});
    return cmd;
}