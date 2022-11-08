#pragma once

#include <iostream>
#include <vector>
#include <thread>


class ThreadHandler
{
	std::vector<std::thread> threads;

	size_t numberOfThreads;

	bool stopRequest;
public:
	void AddThread(void(thCallback()), Game& game);

	void WaitThreads();

	void Stop();
};
