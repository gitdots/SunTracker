#include "SolarCell.hpp"
#include "Utils.hpp"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <chrono>
#include <cstring>

extern "C" {
    #include <linux/i2c-dev.h>
    #include <linux/i2c.h>
    #include <i2c/smbus.h>
}

using namespace std;

SolarCell::SolarCell() {
    deviceFd = open("/dev/i2c-1", O_RDWR);
    if(deviceFd < 0) {
        cerr << "[SolarCell] Failed to open I2C." << endl; 
    } 
    if(ioctl(deviceFd, I2C_SLAVE, INA219_ADDRESS) < 0) {
        cerr << "[SolarCell] Failed to initialized I2C with address provided." << endl;
    }

    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
                      INA219_CONFIG_GAIN_8_320MV |
                      INA219_CONFIG_BADCRES_12BIT |
                      INA219_CONFIG_SADCRES_12BIT_1S_532US |
                      INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    
    int computed_config = 20480;
    if (i2c_smbus_write_word_data(deviceFd, INA219_REG_CONFIG, computed_config) < 0) {
        cerr << "[SolarCell] Failed to write configuration to INA219" << endl;
    }
}

SolarCell::~SolarCell() {
    running = false;
    if(readDataThread.joinable()) {
        readDataThread.join();
    }

    close(deviceFd);
}

map<string, string> SolarCell::uploadData() {
    unique_lock<mutex> lock(m);
    map<string, string> cmd;
    cmd.insert({"shuntVoltage_mV",to_string(last_shuntVoltage_mV)});
    cmd.insert({"busVoltage_V",to_string(last_busVoltage_V)});
    cmd.insert({"current_mA",to_string(last_current_mA)});
    cmd.insert({"power_mW",to_string(last_power_mW)});

    return cmd;
}

vector<float> SolarCell::getData() {
    unique_lock<mutex> lock(m);
    return vector<float> {last_shuntVoltage_mV, last_busVoltage_V, last_current_mA, last_power_mW};
}

void SolarCell::start() {
    running = true;
    readDataThread = thread(&SolarCell::readData, this);
}

void SolarCell::readData() {
    char buff[sizeof(float)];
    while(running) {
        float shuntVoltageSum_mV = 0;
        float busVoltageSum_V = 0;
        float currentSum_mA = 0;
        float powerSum_mW = 0;
        
        for (int i = 0; i < 5; ++i) {
            int16_t raw_shuntVoltage = i2c_smbus_read_word_data(deviceFd, INA219_REG_SHUNTVOLTAGE);
            uint16_t shuntVoltage = __bswap_16(raw_shuntVoltage);
            float shuntVoltage_mV = shuntVoltage * 0.01;

            int16_t raw_busVoltage = i2c_smbus_read_word_data(deviceFd, INA219_REG_BUSVOLTAGE);
            uint16_t busVoltage = __bswap_16(raw_busVoltage) >> 3;
            float busVoltage_V = busVoltage * 0.004;

            float current_mA = shuntVoltage_mV / 0.1;  
            float power_mW = busVoltage_V * current_mA;

            shuntVoltageSum_mV += shuntVoltage_mV;
            busVoltageSum_V += busVoltage_V;
            currentSum_mA += current_mA;
            powerSum_mW += power_mW;
            
            this_thread::sleep_for(chrono::milliseconds(150));
        }
        
        last_shuntVoltage_mV = shuntVoltageSum_mV / 5;
        last_busVoltage_V = busVoltageSum_V / 5;
        last_current_mA = currentSum_mA / 5;
        last_power_mW = powerSum_mW / 5;

        // cout << "Shunt: "<< last_shuntVoltage_mV << " mV\n" << "Voltage: " <<  last_busVoltage_V << " V\n" << "I: " << last_current_mA << " mA\n" << "Power: " << last_power_mW << " mW\n" << endl;

        if(last_busVoltage_V > 5) {        
            last_shuntVoltage_mV = 0;
            last_busVoltage_V = 0;
            last_current_mA = 0;
            last_power_mW = 0;
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}
