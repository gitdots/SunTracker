#ifndef DHT22_HPP
#define DHT22_HPP

#include <string>
#include <utility>

class Dht22 {

    public:
        Dht22(int _pin);
        ~Dht22();
        std::pair<float, float> getTempHum();

    private:
        const int pin;
        int socketFd;
        std::pair<float, float> data;

        void requestData();
        void requestClose();
};

#endif