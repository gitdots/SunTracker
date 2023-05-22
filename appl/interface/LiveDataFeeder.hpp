#pragma once

#include "Observer.hpp"


#include <memory>
#include <vector>
#include <thread>
#include <map>
#include <string>

class LiveDataFeeder {

    public:
        LiveDataFeeder();
        ~LiveDataFeeder();

        void subscribeStreamer(std::shared_ptr<Observer> newStreamer);
        void start();
        void streamLiveDataThread();

    private:
        int socketFd;
        int flaskClient;
        bool streamingLiveData;
        std::thread streamingThread;
        std::vector<std::shared_ptr<Observer>> streamers;
        void sendLiveData(std::map<std::string, std::string> liveData);
        std::string dataMapToJson(std::map<std::string, std::string> dataMap);

};

typedef std::shared_ptr<LiveDataFeeder> LiveDataFeederPtr;

// #endif