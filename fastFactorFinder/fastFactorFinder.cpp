#include <boost/multiprecision/integer.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <iostream>
#include <map>
#include <vector>

using namespace boost::multiprecision;
using boost::thread;
using boost::mutex;

void worker();
uint128_t nextNumber();

std::map<uint128_t, uint128_t> factorMap;
uint128_t currentNumber = 0;
mutex factorMapLock, numberLock;

uint128_t maxNumber = 50000000;
int threadCount = std::thread::hardware_concurrency();
bool showOutput = true;

int main(int argc, char** argv)
{
	for (int x = 0; x < argc; x++)
	{
		if (std::strcmp(argv[x], "--help") == 0)
		{
			std::cout << "--threads N - number of threads to use" << std::endl;
			std::cout << "--max N - maximum number to calculate factors" << std::endl;
			std::cout << "--show-output - show the CSV output at the end of calculations" << std::endl;
			return 0;
		}
		else if (std::strcmp(argv[x], "--threads") == 0)
		{
			x++;
			threadCount = atoi(argv[x]);
		}
		else if (std::strcmp(argv[x], "--max") == 0)
		{
			x++;
			maxNumber = uint128_t(argv[x]);
		}
		else if (std::strcmp(argv[x], "--show-output") == 0)
		{
			showOutput = true;
		}
	}

	std::cout << "Factor Counter" << std::endl;
	std::cout << "==============" << std::endl;
	std::cout << std::endl;

	std::cout << "Number of workers: " << threadCount << std::endl;
	std::cout << "Max Search Number: " << maxNumber << std::endl;
	std::cout << std::endl;

	thread** threads = new thread * [threadCount];

	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = new thread(worker);
	}

	for (int i = 0; i < threadCount; i++)
	{
		threads[i]->join();
		delete threads[i];
	}

	if (showOutput)
	{
		std::cout << "Factor Count,Numbers With Factor Count" << std::endl;

		for (std::map<uint128_t, uint128_t>::iterator it = factorMap.begin(); it != factorMap.end(); ++it)
		{
			std::cout << it->first << "," << it->second << std::endl;
		}
	}
}

void worker()
{
	std::map<uint128_t, uint128_t> localFactorMap;

	uint128_t i = -1;
	do 
	{
		i = nextNumber();

		uint128_t factorCount = 1;
		uint128_t max_search = (i/2)+1;

		for (uint128_t y = 1; y <= max_search; y++)
		{
			if (i % y == 0)
			{
				factorCount++;
			}
		}

		if (localFactorMap.count(factorCount) == 0)
		{
			localFactorMap.insert(std::make_pair(uint128_t(factorCount), uint128_t(0)));
		}

		localFactorMap[factorCount]++;
	} while (i != 0);

	factorMapLock.lock();

	for (std::map<uint128_t, uint128_t>::iterator it = localFactorMap.begin(); it != localFactorMap.end(); ++it)
	{
		if (factorMap.count(it->first) == 0)
		{
			factorMap.insert(std::make_pair(uint128_t(it->first), uint128_t(0)));
		}

		factorMap[it->first] += it->second;
	}

	factorMapLock.unlock();
}

uint128_t nextNumber()
{
	uint128_t returnNumber = 0;

	numberLock.lock();
	currentNumber++;
	returnNumber = currentNumber;
	numberLock.unlock();

	if (returnNumber >= maxNumber)
	{
		returnNumber = 0;
	}

	return returnNumber;
}
