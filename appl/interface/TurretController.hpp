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
#include <utility>

class TurretController {
    
    public:
        TurretController(Mcp3008Ptr _mcp, ServomotorPtr _servos);
        ~TurretController();

        void start();
        std::string uploadData();
        
    private:
        Mcp3008Ptr mcp;
        ServomotorPtr servos;
        std::vector<int> data;

        bool usingPhyisicalOffsets;
        std::pair<int, int> computeNextServoValues();

        bool initProcessThread;
        std::thread dataThread;
        std::thread processThread;
        std::mutex m;
        bool dataReady;
        bool running;
        std::condition_variable cv;

        void setServos();
        void getDataThread();
        void processDataThread();
        void stop();
};

typedef std::shared_ptr<TurretController> TurretControllerPtr;

#endif