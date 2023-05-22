#include "TurretController.hpp"
#include "Utils.hpp"

#include <memory>
#include <iostream>
#include <list>
#include <vector>

#include <unistd.h>
using namespace std;

TurretController::TurretController(Mcp3008Ptr _mcp, ServomotorPtr _servos):
    mcp(_mcp), servos(_servos){

    dataReady = false;
    running = true;
    initProcessThread = true;
}

TurretController::~TurretController() {
    stop();
}

void TurretController::start() {
    dataThread = thread(&TurretController::getDataThread, this);
}


void TurretController::stop() {
    running = false;
    if(dataThread.joinable()) dataThread.join();
    if(processThread.joinable()) processThread.join();
}

void TurretController::getDataThread() {

    while(running) {
        if(initProcessThread) {
            processThread = thread(&TurretController::processDataThread, this);
            initProcessThread = false;
        }

        unique_lock<mutex> lock(m);
        data = mcp->getReading();
        // cout << "MCP_READING " << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << endl;
        dataReady = true;
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(5));
        cv.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void TurretController::processDataThread() {
    while(running) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this]{ return dataReady;});
        setServos();
        dataReady = false;
    }
}

void TurretController::setServos() {
    pair<int, int> angles = computeNextServoValues();
    servos->setAngle(SERVO_0X , angles.first, SERVO_0Y, angles.second);
}

pair<int, int> TurretController::computeNextServoValues() {

    pair<int, int> angles = {0, 0}; // first - ox; second - oy

    int avgHorizontalUp = (data[LDR_UL_CH] + data[LDR_UR_CH]) / 2;
    int avgHorizontalDown = (data[LDR_DL_CH] + data[LDR_DR_CH]) / 2;
    int avgVerticalLeft = (data[LDR_UL_CH] + data[LDR_DL_CH]) / 2;
    int avgVerticalRight = (data[LDR_UR_CH] + data[LDR_DR_CH]) / 2;

    // cout << "AVG_UP=" << avgHorizontalUp << "; AVG_DOWN=" << avgHorizontalDown << "; AVG_LEFT=" << avgVerticalLeft << "; AVG_RIGHT=" << avgVerticalRight << endl; 

    if(avgHorizontalUp - avgHorizontalDown > 15)
        angles.second -=10;
    else if ((avgHorizontalDown - avgHorizontalUp > 15))
        angles.second +=10;

    if(avgVerticalLeft - avgVerticalRight > 15)
        angles.first +=10;
    else if ((avgVerticalRight - avgVerticalLeft > 15))
        angles.first -=10;

    return angles;
}
