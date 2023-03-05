#pragma once
#include "Philosopher.h"

void Philosopher::waitForStart(std::condition_variable& dinnerFlag)
{
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    while (!Dinner::dinnerStarted)
    {
        dinnerFlag.wait(lock);
    }
}

void Philosopher::grabForks(std::mutex* first, std::mutex* second)
{
    bool gotFirst = first->try_lock();
    while (true)
    {
        if (gotFirst)
        {
            act("grab a fork", true);
            if (second->try_lock())
            {
                act("grab other fork", true);
                return;
            }
            else
            {
                act("failed to grab other fork", true);
                first->unlock();
                std::swap(first, second);
            }
        }
        else
        {
            act("failed to grab a fork", true);
        }
        first->lock();
        gotFirst = true;
    }
}

void Philosopher::freeForks()
{
    m_leftFork->unlock();
    m_rightFork->unlock();
    act("puts down forks", true);
}

std::chrono::milliseconds Philosopher::act(const char* action, bool instantaneous)
{
    if (instantaneous)
    {
        snprintf(messageBuffer, g_buffer_size, "%s %s", m_name.c_str(), action);
        Dinner::print(messageBuffer);
        return std::chrono::milliseconds(0);
    }

    std::chrono::milliseconds duration = std::chrono::milliseconds(Dinner::randomInt(Dinner::min_duration_ms, Dinner::max_duration_ms));

    snprintf(messageBuffer, g_buffer_size, "%s %s for %.3f ms", m_name.c_str(), action, (float)duration.count() / 1000.0f);
    Dinner::print(messageBuffer);

    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    return duration;
}

void Philosopher::run(std::condition_variable& dinnerFlag)
{
    waitForStart(dinnerFlag);
    std::chrono::milliseconds timeLeft_ms = std::chrono::milliseconds(Dinner::eat_time_ms);
    while (timeLeft_ms.count() > 0)
    {
        act("think", false);

        grabForks(m_leftFork, m_rightFork);

        timeLeft_ms -= act("eat", false);

        freeForks();
    }
    act("finished eating", true);
}