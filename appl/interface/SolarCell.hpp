#ifndef SOLARCELL_HPP
#define SOLARCELL_HPP

#include "Observer.hpp"

#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <memory>

class SolarCell : public Observer {

    public:
        SolarCell();
        ~SolarCell();

        void start();
        std::vector<float> getData();
        std::map<std::string, std::string> uploadData();

    private:
        int deviceFd;

        float last_shuntVoltage_mV;
        float last_busVoltage_V;
        float last_current_mA;
        float last_power_mW;

        bool running;
        std::mutex m;
        std::thread readDataThread;
        void readData();

        float current_lsb;
        float power_lsb;

};

typedef std::shared_ptr<SolarCell> SolarCellPtr;

#endif