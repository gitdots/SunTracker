#include "interface.hpp"
#include "Utils.hpp"

#include <memory>
#include <unistd.h>

using namespace std;

int main() {

	shared_ptr<SocketCommunicator> comm = make_shared<SocketCommunicator>();
	comm->startPythonBridge();

	Dht22Ptr dht = std::make_shared<Dht22>(comm);
	ServomotorPtr servos = make_shared<Servomotor>(SERVO_0X, SERVO_0Y, comm);
	Mcp3008Ptr mcp = make_shared<Mcp3008>(MCP_SPI_MODE, MCP_SPI_BPW, MCP_SPI_SPEED, MCP_SPI_CSLOW);
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

	while(1) {
		usleep(5000);
	}
	comm->requestClose();
    return 0;	
}