#include "LightServoBrain.hpp"
#include "Utils.hpp"

#include <memory>
#include <iostream>
#include <list>
#include <vector>


#include <fstream>


using namespace std;

LightServoBrain::LightServoBrain() {
    mcp = make_shared<Mcp3008>(MCP_SPI_MODE, MCP_SPI_BPW, MCP_SPI_SPEED, MCP_SPI_CSLOW);
    servoX = make_shared<Servomotor>(SERVO_0X);
    servoY = make_shared<Servomotor>(SERVO_0Y);
    dataReady = false;
}

LightServoBrain::~LightServoBrain() {
    stop();
}

void LightServoBrain::processData() {
    
    int oydiff0 = data[0] - data[2];
    int oydiff1 = data[1] - data[3];
    int oxdiff0 = data[0] - data[1];
    int oxdiff1 = data[2] - data[3];

    cout << "## ## " << oxdiff0 << "\n## ## " << oxdiff1 <<"\n" << oydiff0 << " " << oydiff1 << endl;

}

void LightServoBrain::start() {
    running = true;
    thread dataThread(&LightServoBrain::getDataThread, this);
    dataThread.detach();
    while(running) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this]{ return dataReady;});
        processData();
        dataReady = false;
    }
}

void LightServoBrain::stop() {
    running = false;
    cv.notify_one();
}

void LightServoBrain::getDataThread() {
    while(running) {
        unique_lock<mutex> lock(m);
        data = mcp->getReading();
        dataReady = true;
        processData();
        cv.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
    }
}