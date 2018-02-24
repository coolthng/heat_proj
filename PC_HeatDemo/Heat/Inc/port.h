#ifndef __PORT_H
#define __PORT_H
int16_t PTgetJinKouTemp();
int16_t PTgetChuKouTemp();
int16_t PTgetKeTiTemp();
int16_t PTgetPowerVal();
uint16_t PTgetHsFbVal();
uint16_t PTgetFsFbVal();
uint16_t PTgetYbFbVal();
int PTgetUserSetTemp();
uint16_t PTgetHallFbVal();
uint16_t PTsetPreFengShan(int16_t adjust);
uint16_t PTsetHsVolt(uint16_t voltageValue);
uint16_t PTsetYbHz(uint16_t ybHz);

#endif