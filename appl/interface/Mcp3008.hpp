#ifndef MCP3008_HPP
#define MCP3008_HPP

#include <Utils.hpp>

#include <vector>
#include <memory>
#include <thread>
#include <mutex>

class Mcp3008 {
    
    public:
        Mcp3008(unsigned char mode, unsigned char bits_per_word, unsigned int speed, unsigned char cs_low);
        ~Mcp3008();
        
        std::vector<int> getReading();

    private:
        int *spi_cs_fd;
        int spi_cs0_fd;
        int spi_device;
        unsigned char spi_mode;
        unsigned char spi_bits_per_word;
        unsigned int spi_speed;
        unsigned char cs_low_after_transfer;

        int noChannels;
        float vRef;
        std::vector<int> data;

        int getReadingVoltageOnChannel(int ch);
        void getReadingVoltageOnChannels();

        int spiWriteAndRead(unsigned char* TxData, unsigned char* RxData);
        int openSpi();
        int closeSpi();
        
};

typedef std::shared_ptr<Mcp3008> Mcp3008Ptr;

#endif