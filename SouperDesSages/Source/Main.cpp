#include "MendatoryInclude.h"
#include "Table.h"

bool startProgram()
{
	Dinner* table = new Dinner;
	std::cout << "How many philosophers are at the table? (1-5): ";
	std::cin >> table->num_philosophers;

	std::condition_variable dinnerFlag;
	if (table->setupTable())
	{
		table->startThreads(dinnerFlag);
		table->startDinner(dinnerFlag);
		table->stopThreads();
		return true;
	}
	return false;
}

int main()
{
	if (startProgram())
	{
		return 0;
	}
	return 1;
}