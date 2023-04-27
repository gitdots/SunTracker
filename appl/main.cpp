
#include "interface.hpp"

#include <memory>
#include <list>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	std::shared_ptr<Mcp3008> mcp = std::make_shared<Mcp3008>(0, 8, 5000, 0);
	mcp->setChannels(std::list<int>{0, 1, 2, 3});

	std::shared_ptr<Servomotor> smX = std::make_shared<Servomotor>(SERVO_X);
	std::shared_ptr<Servomotor> smY = std::make_shared<Servomotor>(SERVO_Y);
	smX->setAngle(45);
	smY->setAngle(90);

	smX->setAngle(0);
	smY->setAngle(20);

	smX->setAngle(35);
	smY->setAngle(70);

	// now need to make an algorithm to stabilize the photocell in the optimall position
	return 0;
}