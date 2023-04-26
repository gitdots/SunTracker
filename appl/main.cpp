
#include "Mcp3008.hpp"

#include <memory>
#include <list>
#include <iostream>

#include <unistd.h>

int main() {

    std::shared_ptr<Mcp3008> mcp = std::make_shared<Mcp3008>(0, 8, 5000, 0);
    std::list<int> channels {0,1,2,3};
    mcp->setChannels(channels);
    mcp->openSpi();
    
    for(int i = 0; i < 5; i++) {
      std::cout << "ch0 = " << mcp->getReadingVoltageOnChannel(0) << std::endl;
      std::cout << "ch1 = " << mcp->getReadingVoltageOnChannel(1) << std::endl;
      std::cout << "ch2 = " << mcp->getReadingVoltageOnChannel(2) << std::endl;
      std::cout << "ch3 = " << mcp->getReadingVoltageOnChannel(3) << std::endl;
      usleep(1000);
    }

    mcp->closeSpi();

    return 0;
}