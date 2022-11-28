#ifndef ASIO_LOGGING_H
#define ASIO_LOGGING_H

#include <stdio.h>

namespace asio{
}

#define LOG_INFO(format, ...) printf(format, ##__VA_ARGS__), printf("\n");

#endif