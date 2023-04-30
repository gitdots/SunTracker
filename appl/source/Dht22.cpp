#include "Dht22.hpp"
#include "Utils.hpp"

#include <iostream>
#include <stdlib.h>
#include <sstream>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


using namespace std;

Dht22::Dht22(int _pin) : pin(_pin) {
    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFd == -1) {
        cout << "Error creating socket fd" << endl;
    }

    struct sockaddr_un addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    // Start script that gets the data and transmits it
    string script = "python3 ";
    script.append(DHT22_PATH);
    const string command = script + " " + to_string(DHT) + " " + SOCKET_NAME + " &";
    int result = system(command.c_str());
    cout << "Script call result: " << result << endl;
    if(!result) {
        cout << "Script could not start" << endl;
    }

    int connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    while(connResult == -1) {
        cout << "Waiting for server to start..." << endl;
        sleep(1);
        connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    }

    cout << "Connected to server!" << endl;
}

Dht22::~Dht22() {
    requestClose();
    close(socketFd);
}

pair<float,float> Dht22::getTempHum() {
    requestData();
    return data;
}

void Dht22::requestData() {
    // send a request
    std::string request = "1";
    if (send(socketFd, request.c_str(), request.size(), 0) == -1) {
        cout << "fail writing to socket" << endl;
    }
    char buff[1024];
    int bytes = recv(socketFd, buff, sizeof(buff), 0);
    if(bytes == -1) {
        cout << "Problem reading " << endl;
    }
    else if(bytes == 0) cout <<"Connection closed" << endl;
    string reqData(buff, bytes);
    
    string token;
    bool first = true;
    stringstream ss(reqData);
    while(getline(ss, token, ';')) {
        if(first) {
            data.first = stof(token);
            first = false;
        }
        else data.second = stof(token);
    }
}

void Dht22::requestClose() {
    // send a request
    std::string request = "2";
    if (send(socketFd, request.c_str(), request.size(), 0) == -1) {
        cout << "fail writing to socket" << endl;
    }
    
    int i = 0;
    struct sockaddr_un addr;
    memset(&addr, 0 , sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    int connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
    while(connResult == -1) {
        cout << "Waiting for server to start..." << endl;
        sleep(1);
        connResult = connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));
        i++;
        if(i==5) {
            cout << "Server shut down." << endl;
            break;
        }
    }
}
