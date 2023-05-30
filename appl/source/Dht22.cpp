#include "Dht22.hpp"
#include "Utils.hpp"

#include <iostream>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <chrono>

using namespace std;

Dht22::Dht22(shared_ptr<SocketCommunicator> _comm) : comm(_comm) {
    isRunning = false;
}

Dht22::~Dht22() {
}

void Dht22::start() {

    isRunning = true;
    t = thread(&Dht22::requestData, this);
}

pair<float,float> Dht22::getTempHum() {
    unique_lock<mutex> lock(m);
    std::pair<float, float> d = data;
    return d;
}

void Dht22::requestData() {

    while(isRunning) {
        pair<float, float> newData = comm->requestTempHum();
        if (newData.first > 100.0 and newData.second > 100.0) {
            cout << "__________Failed sample of new data__________" << endl;
            this_thread::sleep_for(std::chrono::seconds(2));
        }
        else {
            data = newData;
            cout << "_____DHT NEW DATA __" << data.first << " " << data.second << endl;
            this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}

map<string, string> Dht22::uploadData() {
    map<string, string> cmd;
    unique_lock<mutex> lock(m);
    // cout << "[Dht22] Upload data: " << data.first << ", " << data.second << endl;
    cmd.insert(pair<string, string>("temp", to_string(data.first))); 
    cmd.insert(pair<string, string>("hum", to_string(data.second))); 
    return cmd;
}
