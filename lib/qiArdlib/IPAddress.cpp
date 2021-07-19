/*
  IPAddress.cpp - Base class that provides IPAddress
 Arduino STM32F10X 移植库
作者：刘佳奇
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
*/

#include "qiArdlib.h"
#include <IPAddress.h>
#include <Print.h>

IPAddress::IPAddress()
{
    _address.dword = 0;
}

IPAddress::IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    _address.bytes[0] = first_octet;
    _address.bytes[1] = second_octet;
    _address.bytes[2] = third_octet;
    _address.bytes[3] = fourth_octet;
}

IPAddress::IPAddress(uint32_t address)
{
    _address.dword = address;
}

IPAddress::IPAddress(const uint8_t *address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
}

bool IPAddress::fromString(const char *address)
{
    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (c >= '0' && c <= '9')
        {
            acc = acc * 10 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return false;
            }
        }
        else if (c == '.')
        {
            if (dots == 3) {
                // Too much dots (there must be 3 dots)
                return false;
            }
            _address.bytes[dots++] = acc;
            acc = 0;
        }
        else
        {
            // Invalid char
            return false;
        }
    }

    if (dots != 3) {
        // Too few dots (there must be 3 dots)
        return false;
    }
    _address.bytes[3] = acc;
    return true;
}

IPAddress& IPAddress::operator=(const uint8_t *address)
{
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
}

IPAddress& IPAddress::operator=(uint32_t address)
{
    _address.dword = address;
    return *this;
}

bool IPAddress::operator==(const uint8_t* addr) const
{
    return memcmp(addr, _address.bytes, sizeof(_address.bytes)) == 0;
}

size_t IPAddress::printTo(Print& p) const
{
    size_t n = 0;
    for (int i = 0; i < 3; i++)
    {
        n += p.print(_address.bytes[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address.bytes[3], DEC);
    return n;
}

