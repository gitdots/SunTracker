#ifndef DHT22_HPP
#define DHT22_HPP

#include "Observer.hpp"
#include "SocketCommunicator.hpp"

#include <string>
#include <utility>
#include <memory>
#include <mutex>
#include <thread>

class Dht22 : public Observer {

    public:
        Dht22(std::shared_ptr<SocketCommunicator> _comm);
        ~Dht22();
        void start();
        std::pair<float, float> getTempHum();

        std::map<std::string, std::string> uploadData();

    private:
        std::shared_ptr<SocketCommunicator> comm;
        std::pair<float, float> data;

        std::mutex m;
        std::thread t;
        bool isRunning;

        void requestData();
        void requestClose();
};

typedef std::shared_ptr<Dht22> Dht22Ptr;

#endif