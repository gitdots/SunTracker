#ifndef MCP3008_HPP
#define MCP3008_hpp

#include <list>

class Mcp3008 {
    
    public:

        Mcp3008(unsigned char mode, unsigned char bits_per_word, unsigned int speed, unsigned char cs_low);
        ~Mcp3008();

        void setChannels(std::list<int> chs);
        std::list<int> getChannels();
        
        int getReadingVoltageOnChannel(int ch);
        std::list<int> getReadingVoltageOnChannels();
        float getReadingValueOnChannel(int ch);
        std::list<float> getReadingValueOnChannels();

        int openSpi();
        int closeSpi();
        int spiWriteAndRead(unsigned char* TxData, unsigned char* RxData);

    private:
        int *spi_cs_fd;
        int spi_cs0_fd;
        int spi_device;
        unsigned char spi_mode;
        unsigned char spi_bits_per_word;
        unsigned int spi_speed;
        unsigned char cs_low_after_transfer;


        std::list<int> channels;
};  


#endif