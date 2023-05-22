#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

#include <string>
#include <memory>
#include <map>
#include <mutex>

#include "SocketCommunicator.hpp"
#include "Observer.hpp"

enum ServomotorMode {
    MODE_AUTOMATIC = 0,
    MODE_MANUAL = 1
};

class Servomotor : public Observer {
    
    public:
        Servomotor(int _pinx, int _piny, std::shared_ptr<SocketCommunicator> _comm);
        ~Servomotor();
        
        void setAngle(int pinx, int anglex, int piny, int angley);
        void setMode(ServomotorMode mode);
        ServomotorMode getMode();
        std::map<std::string, std::string> uploadData();

    private:
        std::shared_ptr<SocketCommunicator> comm;
        std::mutex tm;

        int pinx;
        int piny;
        int lastAngley;
        int currAngley;
        int lastAnglex;
        int currAnglex;
        ServomotorMode runningMode;
};

typedef std::shared_ptr<Servomotor> ServomotorPtr;

#endif