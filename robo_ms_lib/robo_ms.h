#ifndef INCLUDED_robo_ms_H
#define INCLUDED_robo_ms_H

#include "mbed.h"

class robo_ms
{
public:
    robo_ms(CAN &can);
    int can_data_send(int moter[4]);
    void can_data_read(CANMessage &msg, short *rotation, short *speed);

private:
    CANMessage _canMessage;
    CAN &_can;
    unsigned short _r;
    int _rotation;
    int _speed;
    int _torque;
    int _temperature;
};

#endif