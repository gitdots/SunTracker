#pragma once

#include <string>
#include <map>

class Observer {

    public:
        Observer();
        virtual ~Observer();
        virtual std::map<std::string, std::string> uploadData() = 0;
};
