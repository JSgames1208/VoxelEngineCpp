//
// Created by Jasper Soete on 12-4-2026.
//

#pragma once

#include <iostream>
#include <chrono>
#include <vector>

#ifndef PROFILER_ENABLE
#define PROFILER_ENABLE 1
#endif


namespace profiler
{
    struct ProfileResult
    {
        const char* name;
        double time_ms;
    };

    class Profiler {
    public:
        static Profiler &get() {
            static Profiler instance;
            return instance;
        }

        void beginFrame() {
            m_results.clear();
        }

        void add(const char *name, double time) {
            m_results.push_back({name, time});
        }

        void endFrame()
        {
            printFrame();
        }

        void printFrame()
        {
            static int frame = 0;

            std::cout << "===== FRAME " << frame++ << " =====\n";
            for (auto result : m_results)
            {
                std::cout << result.name << ": " << result.time_ms << " ms\n";
            }

            std::cout << "======================\n";
        }
    private:
        std::vector<ProfileResult> m_results;
    };


    //TIMER

    class Timer
    {
    public:
        using clock = std::chrono::steady_clock;

        void start()
        {
            m_start = clock::now();
        }
        double stop()
        {
            clock::time_point end = clock::now();
            return std::chrono::duration<double, std::milli>(end - m_start).count();
        }
    private:
        clock::time_point m_start;
    };

    //SCOPED TIMER (returns result when timer gets destructed)

    class ScopedTimer
    {
    public:
        ScopedTimer(const char* name)
            : m_name(name)
        {
            m_timer.start();
        }
        ~ScopedTimer()
        {
            double elapsed = m_timer.stop();
            Profiler::get().add(m_name, elapsed);
        }
    private:
        Timer m_timer;
        const char* m_name;
    };
}

#if PROFILER_ENABLE
#define PROFILE_SCOPE(name) profiler::ScopedTimer timer_##__LINE__(name)
#else
#define PROFILE_SCOPE(name)
#endif
