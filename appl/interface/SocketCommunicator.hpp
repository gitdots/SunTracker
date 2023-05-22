#ifndef SOCKETCOMUMUNICATOR_HPP
#define SOCKETCOMUMUNICATOR_HPP

#include <string>
#include <utility>
#include <mutex>

class SocketCommunicator {

    public:
        SocketCommunicator();
        ~SocketCommunicator();

        void startPythonBridge();
        void requestServoMove(std::string requestCode);
        std::pair<float, float> requestTempHum();
        void requestClose();
    
    private:
        std::mutex m;
        int socketFd;

};


#endif