#include "interface.hpp"

//#include "LightServoBrain.hpp"

#include <memory>
#include <list>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utility>

int main(int argc, char* argv[]){

	LightServoBrainPtr ls = std::make_shared<LightServoBrain>();
	ls->start();

	return 0;
}