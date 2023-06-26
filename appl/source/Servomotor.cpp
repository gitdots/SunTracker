#include "Servomotor.hpp"
#include "Utils.hpp"

#include <utility>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <string>

using namespace std;

Servomotor::Servomotor(int _pinx, int _piny, shared_ptr<SocketCommunicator> _comm) : 
                        pinx(_pinx), piny(_piny), comm(_comm) {

    runningMode = MODE_AUTOMATIC;
    lastAnglex = 90;
    lastAngley = 90;
}

Servomotor::~Servomotor() {

}

void Servomotor::setAngle(int pinx, int anglex, int piny, int angley) {
    unique_lock<mutex> lock_(tm);
    string reqMessage = "{" + to_string(anglex) + ";" + to_string(angley) + "}";
    pair<int, int> angles = comm->requestServoMove(reqMessage);
    lastAnglex = angles.first - 90;
    lastAngley = angles.second - 90;
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
    cmd.insert({"anglex", to_string(lastAnglex)});
    cmd.insert({"angley", to_string(lastAngley)});
    return cmd;
}