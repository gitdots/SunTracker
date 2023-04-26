#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

class Servomotor {

    private:
        int angle;
    
    public:
        Servomotor();
        ~Servomotor();
        
        void setAngle();
};

#endif