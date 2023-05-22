#include "interface.hpp"
#include "Utils.hpp"

#include <memory>
#include <list>
#include <vector>
#include <iostream>

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstring>
#include <utility>
#include <sys/socket.h>
#include <sys/un.h>
#include <vector>
using namespace std;

#define INA219_ADDRESS 0x40

string dataMapToJson(pair<string, int> pr) {
    string json = "{";
    bool first = true;

	if (!first) {
		json += ",";
	}
	first = false;
	json += "\"" + pr.first + "\":" + to_string(pr.second);
    
    json += "}";
    return json;
}

int main() {

	shared_ptr<SocketCommunicator> comm = make_shared<SocketCommunicator>();
	comm->startPythonBridge();

	Mcp3008Ptr mcp = make_shared<Mcp3008>(MCP_SPI_MODE, MCP_SPI_BPW, MCP_SPI_SPEED, MCP_SPI_CSLOW);
	ServomotorPtr servos = make_shared<Servomotor>(SERVO_0X, SERVO_0Y, comm);
	Dht22Ptr dht = std::make_shared<Dht22>(comm);
	SolarCellPtr cell = make_shared<SolarCell>();
	
	TurretControllerPtr turret = make_shared<TurretController>(mcp, servos);

	turret->start();
	cell->start();
	dht->start();

	LiveDataFeederPtr liveFeed = make_shared<LiveDataFeeder>();
	liveFeed->subscribeStreamer(mcp);
	liveFeed->subscribeStreamer(servos);
	liveFeed->subscribeStreamer(dht);
	liveFeed->subscribeStreamer(cell);

	liveFeed->start();

	while(1){usleep(5000);}

	// unsigned int i = 0;
	// int n;
	// char buf[20];
	// vector<string> s = {"12", "23", "34", "45", "56", "67", "78", "89", "12", "23", "34", "45", "56", "67", "78", "89", "12", "23", "34", "45", "56", "67", "78", "89"};
	// while(1){
	// 	send(client, s[i].c_str(), 2, 0);
	// 	cout << "sent: " << s[i++] << endl;
	// 	sleep(1);
	// 	if(i >= s.size()) break;
	// }

    // close(socketFd);

    return 0;	
}