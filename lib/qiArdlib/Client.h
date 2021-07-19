/*
  Client.h - Base class that provides Client
 Arduino STM32F10X 移植库
作者：刘佳奇
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
*/

#ifndef client_h
#define client_h
#include "Print.h"
#include "Stream.h"
#include "IPAddress.h"

class Client : public Stream {

public:
    virtual int connect(IPAddress ip, uint16_t port) = 0;
    virtual int connect(const char *host, uint16_t port) = 0;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buf, size_t size) = 0;
    using Print::write;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int read(uint8_t *buf, size_t size) = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual void stop() = 0;
    virtual uint8_t connected() = 0;
    virtual operator bool() = 0;
protected:
    uint8_t* rawIPAddress(IPAddress& addr) {
        return addr.raw_address();
    };
};

#endif
