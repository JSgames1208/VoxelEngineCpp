//
// Created by Jasper Soete on 12-4-2026.
//

#pragma once
#include <iostream>
#include <mutex>

class Logger
{
public:
    static void log(const std::string& msg);
    static void debug(const std::string& msg);
private:
    static std::mutex& getMutex();
};
