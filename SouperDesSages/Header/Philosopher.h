#pragma once

#include "MendatoryInclude.h"
#include "Table.h"

struct Philosopher
{
    std::string m_name;
    std::mutex* m_leftFork;
    std::mutex* m_rightFork;

    char messageBuffer[g_buffer_size];
    void waitForStart(std::condition_variable& dinnerFlag);

    void grabForks(std::mutex* first, std::mutex* second);

    void freeForks();

    std::chrono::milliseconds act(const char* action, bool instantaneous);

    void run(std::condition_variable& dinnerFlag);
};