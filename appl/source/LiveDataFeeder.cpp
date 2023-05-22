#include "LiveDataFeeder.hpp"
#include "Utils.hpp"

#include <chrono>
#include <algorithm>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
using namespace std;

LiveDataFeeder::LiveDataFeeder() {
    streamingLiveData = false;

    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket error\n";
    }
	struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, APPL2FLASK_SOCKET_PATH, sizeof(addr.sun_path) - 1);
	unlink(APPL2FLASK_SOCKET_PATH);
	if(bind(socketFd, (sockaddr*)(&addr), sizeof(addr))) {
		std::cerr << "Bind error\n";	
	}
}

LiveDataFeeder::~LiveDataFeeder() { }

void LiveDataFeeder::start() {
    cout << "Unix socket started. Ready to stream data." << endl;
	cout << "Listening for connection." << endl;
    if (listen(socketFd, 1) == -1) {
		std::cerr << "Listen error\n";
	}
	flaskClient = accept(socketFd, NULL, NULL);
	cout << "Flask connected. Start sending live data." << endl;

    streamingLiveData = true;
    streamingThread = thread(&LiveDataFeeder::streamLiveDataThread, this);
}

void LiveDataFeeder::subscribeStreamer(shared_ptr<Observer> newStreamer) {
    auto it = find(streamers.begin(), streamers.end(), newStreamer);
    if(it == streamers.end()) {
        streamers.push_back(newStreamer);
    }
}

void LiveDataFeeder::streamLiveDataThread() {

    map<string, string> liveData;

    while(streamingLiveData) {
        liveData.clear();
        for(auto streamer : streamers) {
            auto streamerLiveData = streamer->uploadData();
            liveData.insert(streamerLiveData.begin(), streamerLiveData.end());
        }
        cout << "Got new data " << endl;
        sendLiveData(liveData);
        this_thread::sleep_for(chrono::seconds(1));
    }
}

string LiveDataFeeder::dataMapToJson(map<string, string> dataMap) {
    string json = "{";
    bool first = true;

    for (auto& pair : dataMap) {
        if (!first) {
            json += ",";
        }
        first = false;
        json += "\"" + pair.first + "\":" + pair.second;
    }
    json += "}";
    return json;
}

void LiveDataFeeder::sendLiveData(map<string, string> liveData) {
    string serializedData = dataMapToJson(liveData);
    cout << "\n#################\n" << serializedData << "\n#################\n";
    int bytesSent = send(flaskClient, serializedData.c_str(), serializedData.size(), 0);
    if(bytesSent == -1) {
        cout << "Failed to send data" << endl;
    }
    
}