#ifndef LIGHTSERVOBRAIN_HPP
#define LIGHTSERVOBRAIN_HPP

#include "Servomotor.hpp"
#include "Mcp3008.hpp"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

class LightServoBrain {
    
    public:
        LightServoBrain();
        ~LightServoBrain();

        void start();
        
    private:
        Mcp3008Ptr mcp;
        ServomotorPtr servoX;
        ServomotorPtr servoY;
        std::vector<int> data;

        std::thread th;
        std::mutex m;
        bool dataReady;
        bool running;
        std::condition_variable cv;

        void getDataThread();
        void processData();
        void stop();
};

typedef std::shared_ptr<LightServoBrain> LightServoBrainPtr;

#endif