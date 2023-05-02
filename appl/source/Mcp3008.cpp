#include "Mcp3008.hpp"

#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <cstring>

using namespace std;

Mcp3008::Mcp3008(unsigned char mode, unsigned char bits_per_word, unsigned int speed, unsigned char cs_low) :
        spi_mode(mode), spi_bits_per_word(bits_per_word), spi_speed(speed), cs_low_after_transfer(cs_low)
{   
    spi_cs_fd = &spi_cs0_fd;
    noChannels = MCP_NO_CH;
    vRef = 3.3;
    openSpi();
}

Mcp3008::~Mcp3008() {
    closeSpi();
}

vector<int> Mcp3008::getReading() {
    getReadingVoltageOnChannels();
    return data;
}

int Mcp3008::getReadingVoltageOnChannel(int ch) { 
    unsigned char rx[3];
    unsigned char tx[3] = {0x01, 0x80, 0x00};
    tx[1] = (0x80 | (ch << 4));
    int err = spiWriteAndRead(tx, rx);
    int voltage = ((rx[1] & 0x03) << 8) | rx[2];
    return voltage;
}

void Mcp3008::getReadingVoltageOnChannels() {
    cout << endl;
    data.clear();
    for(int ch = 0; ch < noChannels; ch++) {
        data.insert(data.begin() + ch, getReadingVoltageOnChannel(ch));
        cout << data[ch] << " ";
    }
    cout << endl;
}

int Mcp3008::openSpi() {
	int status = -1;
    *spi_cs_fd = open("/dev/spidev0.0", O_RDWR);

    if (*spi_cs_fd < 0) {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status < 0) {
        perror("Could not set SPIMode (WR)");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status < 0) {
        perror("Could not set SPIMode (RD)");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word);
    if(status < 0) {
        perror("Could not set SPI bitsPerWord (WR)");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits_per_word);
    if(status < 0) {
        perror("Could not set SPI bitsPerWord(RD)");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status < 0) {
        perror("Could not set SPI speed (WR)");
        exit(1);
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status < 0) {
        perror("Could not set SPI speed (RD)");
        exit(1);
    }

    cout << "Spi port opened" << endl; 
    return(status);
}

int Mcp3008::closeSpi() {
    int status = -1;

    status = close(*spi_cs_fd);
    if(status < 0) {
    	perror("Error - Could not close SPI device");
    	exit(1);
    }

    cout << "Spi port closed" << endl; 
    return(status);
}

int Mcp3008::spiWriteAndRead(unsigned char *TxData, unsigned char *RxData) {

    int val = -1;
    struct spi_ioc_transfer spi;
    memset(&spi, 0, sizeof(spi));
	spi = {
        .tx_buf = (unsigned long)TxData,		//transmit from "data"
        .rx_buf = (unsigned long)RxData,		//receive into "data"
        .len = 3,   //3 bytes
        .speed_hz = 5000,
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = 0 		//0=Set CS high after a transfer, 1=leave CS set low
    };

    val = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(1), &spi);
    if(val < 0) {
        perror("Error - problem transmitting spi data");
        exit(1);
    }

    return val;
}