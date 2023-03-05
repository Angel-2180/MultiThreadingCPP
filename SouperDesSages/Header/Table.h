#pragma once
#include "MendatoryInclude.h"
#include "Philosopher.h"
struct Philosopher;
struct Dinner
{
	static constexpr int eat_time_ms = 5000;
	const char* philospher_names[5] = { "Aristotle", "Plato", "Socrates", "Descartes", "Kant" };
	static constexpr int min_duration_ms = 300;
	static constexpr int max_duration_ms = 500;
	inline static   bool dinnerStarted = false;
	int num_philosophers = 5;
	std::vector<Philosopher> philosphers;
	std::vector<std::mutex> forks;
	std::vector<std::thread> threads;

	static const void print(const char* message)
	{
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);
		std::cout << message << '\n';
	}

	static const int randomInt(int min, int max)
	{
		thread_local std::random_device rd;
		thread_local std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);

		return dis(gen);
	}

	bool setupTable();
	void startThreads(std::condition_variable& dinnerFlag);

	void startDinner(std::condition_variable& dinnerFlag);

	void stopThreads();
};
