#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

#include <string>
#include <memory>

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

typedef std::shared_ptr<Servomotor> ServomotorPtr;

#endif
