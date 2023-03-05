#pragma once
#include "Table.h"
#include "Philosopher.h"

bool Dinner::setupTable()
{
	std::vector<const char*> names(std::begin(philospher_names), std::end(philospher_names));
	if (num_philosophers <= 0 || num_philosophers > names.size())
	{
		std::cout << "Invalid number of philosophers. Must be between 1 and " << names.size() << '\n';
		return false;
	}
	philosphers.resize(num_philosophers);
	forks = std::vector<std::mutex>(num_philosophers);
	for (size_t i = 0; i < num_philosophers; ++i)
	{
		Philosopher& p = philosphers[i];
		p.m_name = names[i];
		size_t leftForkIndex = i == 0 ? num_philosophers - 1 : i - 1;
		p.m_leftFork = &forks[leftForkIndex];
		p.m_rightFork = &forks[i];
	}
	return true;
}

void Dinner::startThreads(std::condition_variable& dinnerFlag)
{
	threads.reserve(philosphers.size());
	for (size_t i = 0; i < philosphers.size(); ++i)
	{
		threads.emplace_back(
			&Philosopher::run,
			philosphers[i],
			std::ref(dinnerFlag)
		);
	}
}

void Dinner::startDinner(std::condition_variable& dinnerFlag)
{
	for (int i = 3; i > 0; --i)
	{
		std::cout << "Dinner starts in " << i << " seconds" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "Dinner started!" << std::endl;
	dinnerStarted = true;
	dinnerFlag.notify_all();
}

void Dinner::stopThreads()
{
	for (std::thread& t : threads)
	{
		t.join();
	}
}