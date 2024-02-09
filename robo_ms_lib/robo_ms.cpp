#include "robo_ms.h"
#include "mbed.h"
robo_ms::robo_ms(CAN &can)
    : _can(can)
{
    _can.frequency(1000000); // CANのビットレートを指定
    _can.mode(CAN::Normal);  // CANのモードをNormalに設定
}
/*void robo_ms::numeric_conversion(int moter[4], char byte[8]) {
    // int値をchar型に変換
    byte[0] = (char)(moter[0] >> 8); // int値の上位8ビットをcharに変換
    byte[1] = (char)(moter[0] & 0xFF); // int値の下位8ビットをcharに変換

    byte[2] = (char)(moter[1] >> 8); // int値の上位8ビットをcharに変換
    byte[3] = (char)(moter[1] & 0xFF);

    byte[4] = (char)(moter[2] >> 8); // int値の上位8ビットをcharに変換
    byte[5] = (char)(moter[2] & 0xFF);

    byte[6] = (char)(moter[3] >> 8); // int値の上位8ビットをcharに変換
    byte[7] = (char)(moter[3] & 0xFF);

}*/

int robo_ms::can_data_send(int moter[4])
{
    char byte[8];
    byte[0] = (char)(moter[0] >> 8);   // int値の上位8ビットをcharに変換
    byte[1] = (char)(moter[0] & 0xFF); // int値の下位8ビットをcharに変換

    byte[2] = (char)(moter[1] >> 8); // int値の上位8ビットをcharに変換
    byte[3] = (char)(moter[1] & 0xFF);

    byte[4] = (char)(moter[2] >> 8); // int値の上位8ビットをcharに変換
    byte[5] = (char)(moter[2] & 0xFF);

    byte[6] = (char)(moter[3] >> 8); // int値の上位8ビットをcharに変換
    byte[7] = (char)(moter[3] & 0xFF);

    _canMessage.id = 0x200;        // CANメッセージのIDを設定
    _canMessage.len = 8;           // CANメッセージのデータ長を設定
    _canMessage.data[0] = byte[0]; // CANメッセージのデータにbyte1をセット
    _canMessage.data[1] = byte[1]; // CANメッセージのデータにbyte2をセット
    _canMessage.data[2] = byte[2];
    _canMessage.data[3] = byte[3];
    _canMessage.data[4] = byte[4];
    _canMessage.data[5] = byte[5];
    _canMessage.data[6] = byte[6];
    _canMessage.data[7] = byte[7];
    // CANメッセージの送信
    if (_can.write(_canMessage))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
void robo_ms::can_data_read(CANMessage &msg, short *rotation, short *speed)
{
    // printf("id:%x\n",msg.id);
    _r = (msg.data[0] << 8) | (msg.data[1] & 0xff);
    _rotation = (float)_r / 8192 * 360;
    *rotation = _rotation;
    // printf("rotation: %d",_rotation);

    // speed
    _speed = (msg.data[2] << 8) | (msg.data[3] & 0xff);
    if (_speed & 0b1000000000000000)
    {
        _speed--;
        _speed = -~_speed;
    }
    *speed = _speed;
    // printf("speed:%d",_speed);

    // torque
    _torque = (msg.data[4] << 8) | (msg.data[5] & 0xff);
    if (_torque & 0b1000000000000000)
    {
        _torque--;
        _torque = -~_torque;
    }
    // printf("torque:%d",_torque);

    // temperature
    _temperature = msg.data[6];

    // printf("temperature:%d",_temperature);
    // printf(" \n");
}
