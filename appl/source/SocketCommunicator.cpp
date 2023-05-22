#include "SocketCommunicator.hpp"
#include "Utils.hpp"


#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <mutex>

using namespace std;

SocketCommunicator::SocketCommunicator() {
    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFd == -1) {
        // cout << "Error creating socket fd" << endl;
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
    // cout << command << endl;
    int result = system(command.c_str());
    // cout << "Script call result: " << result << endl;

    int connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    while(connResult == -1) {
        // cout << "Waiting for socket to become active..." << endl;
        sleep(1);
        connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    }

}

void SocketCommunicator::requestClose() {
    unique_lock<mutex> lock(m);
    
    std::string request = "2";
    if (send(socketFd, request.c_str(), request.size(), 0) == -1) {
        // cout << "Fail writing to socket" << endl;
    }
    // cout << "Send request to close." << endl;

    int i = 0;
    struct sockaddr_un addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    int connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    while(connResult == -1) {
        // cout << "[DHT22] Verifying that server is dead: try " << i << endl;
        sleep(1);
        connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
        i++;
        if(i == 3) { 
            // cout << "[DHT22] Server is shut down." << endl;
            break;
        }
    }
}
    
pair<float, float> SocketCommunicator::requestTempHum() {

    unique_lock<mutex> lock(m);

    string requestCode = "th";
    if (send(socketFd, requestCode.c_str(), requestCode.size(), 0) == -1) {
        // cout << "[DHT22] Faild writing to socket" << endl;
    }
    char buff[1024];
    int bytes = recv(socketFd, buff, sizeof(buff), 0);
    if(bytes == -1) {
        // cout << "[DHT22] Problem reading " << endl;
    }
    lock.unlock();
    
    string reqData(buff, bytes);
    pair<float,float> data;
    
    string token;
    bool first = true;
    stringstream ss(reqData);
    // cout << "[DHT22] Received: " << reqData << endl;
    while(getline(ss, token, ';')) {
        try {
            float value = stof(token);
            if(first) {
                first = false;
                data.first = value;
            }
            else data.second = value;
        }
        catch(const std::invalid_argument& e) {
            // cout << "[DHT22] Invalid argument error: " << e.what() << token << endl;
        }
        catch(const std::out_of_range& e) {
            // cout << "[DHT22] Out of range error: " << e.what() << token << endl;
        }
    }
    return data;

}

void SocketCommunicator::requestServoMove(string requestCode) {
    unique_lock<mutex> lock(m);
    if (send(socketFd, requestCode.c_str(), requestCode.size(), 0) == -1) {
        // cout << "Required to adjust servos FAILED" << endl;
    }
    lock.unlock();
}
