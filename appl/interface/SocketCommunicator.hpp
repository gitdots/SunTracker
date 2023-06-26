#ifndef SOCKETCOMUMUNICATOR_HPP
#define SOCKETCOMUMUNICATOR_HPP

#include <string>
#include <utility>
#include <mutex>
#include <condition_variable>

class SocketCommunicator {

    public:
        SocketCommunicator();
        ~SocketCommunicator();

        void startPythonBridge();
        std::pair<int, int> requestServoMove(std::string requestCode);
        std::pair<float, float> requestTempHum();
        void requestClose();
    
    private:
        std::mutex m;
        std::condition_variable m_cv;
        int socketFd;

};


#endif