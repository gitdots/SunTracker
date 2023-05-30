#include "SocketCommunicator.hpp"
#include "Utils.hpp"

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

SocketCommunicator::SocketCommunicator() {
    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFd == -1) {
        cerr << "[SocketCommunicator] Error creating socket file descriptor." << endl;
    }
}

SocketCommunicator::~SocketCommunicator() {
    requestClose();
    close(socketFd);
    
    string killCommand = "pkill -f ";
    killCommand.append(PY_BRIDGE);
    system(killCommand.c_str());
}

void SocketCommunicator::startPythonBridge() {
    
    struct sockaddr_un addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    
    string script = "python3 ";
    script.append(PY_BRIDGE);
    const string command = script + " " + to_string(DHT_PIN) + " " + to_string(SERVO_0X) + " " + to_string(SERVO_0Y) + " " + SOCKET_NAME + " &";
    int result = system(command.c_str());

    int connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    while(connResult == -1) {
        sleep(1);
        connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    }

}

void SocketCommunicator::requestClose() {
    unique_lock<mutex> lock(m);
    
    std::string request = "2";
    if (send(socketFd, request.c_str(), request.size(), 0) == -1) {
        cerr << "[SocketCommunicator] Failed writing to bridge socket." << endl;
    }
}
    
pair<float, float> SocketCommunicator::requestTempHum() {

    unique_lock<mutex> lock(m);

    string requestCode = "th";
    if (send(socketFd, requestCode.c_str(), requestCode.size(), 0) == -1) {
        cerr << "[SocketCommunicator] Faild socket request of temperature and humidty." << endl;
    }
    char buff[1024];
    int bytes = recv(socketFd, buff, sizeof(buff), 0);
    lock.unlock();
    
    string reqData(buff, bytes);
    pair<float,float> data;
    
    string token;
    bool first = true;
    stringstream ss(reqData);
    while(getline(ss, token, ';')) {
        cout << "Converting: " << token << endl;
        try {
            float value = stof(token);
            if(first) {
                first = false;
                data.first = value;
            }
            else {
                data.second = value;
                break;
            }
        }
        catch(const std::invalid_argument& e) {
            cerr << "[SocketCommunicator] Invalid argument error: " << e.what() << token << endl;
        }
        catch(const std::out_of_range& e) {
            cerr << "[SocketCommunicator] Out of range error: " << e.what() << token << endl;
        }
    }
    cout << "[SocketCommunicator] temp: " << data.first << "; hum: " << data.second << endl;
    return data;
}

void SocketCommunicator::requestServoMove(string requestCode) {
    unique_lock<mutex> lock(m);
    if (send(socketFd, requestCode.c_str(), requestCode.size(), 0) == -1) {
        cerr << "[SocketCommunicator] Failed requesting to adjust servos" << endl;
    }
    lock.unlock();
}
