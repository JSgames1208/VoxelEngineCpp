//
// Created by Jasper Soete on 12-4-2026.
//

#include "engine/core/Logger.h"

void Logger::log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(getMutex());
    std::cout << msg << std::endl;
}

std::mutex& Logger::getMutex()
{
    static std::mutex m;
    return m;
}

void Logger::debug(const std::string& msg)
{
}
