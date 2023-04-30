
#include "interface.hpp"

#include <memory>
#include <list>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utility>

int main(int argc, char* argv[]){

	std::shared_ptr<Mcp3008> mcp = std::make_shared<Mcp3008>(0, 8, 5000, 0);
	mcp->setChannels(std::list<int>{0, 1, 2, 3});

	std::shared_ptr<Servomotor> smX = std::make_shared<Servomotor>(SERVO_X);
	std::shared_ptr<Servomotor> smY = std::make_shared<Servomotor>(SERVO_Y);
	std::shared_ptr<Dht22> dht = std::make_shared<Dht22>(DHT);
	//std::shared_ptr<Database> db = std::make_shared<Database>();

	std::pair<float, float> data;
	data = dht->getTempHum();
	std::cout << "Temp=" << data.first << "\nHum=" << data.second << std::endl;
	
	// now need to make an algorithm to stabilize the photocell in the optimall position
	return 0;
}