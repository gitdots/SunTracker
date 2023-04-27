#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

#include <string>

class Servomotor {
    
    public:
        Servomotor(int pin);
        ~Servomotor();
        
        void setAngle(int angle);

    private:
        std::string script;

        int pin;
        int lastAngle;
        int currAngle;
};

#endif
