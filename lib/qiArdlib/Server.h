/*
  Server.h - Base class that provides Server
 Arduino STM32F10X 移植库
作者：刘佳奇
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
*/

#ifndef server_h
#define server_h

#include "Print.h"

class Server : public Print {
public:
    virtual void begin() = 0;
};

#endif
