#include "ThreadHandler.h"
#include "Game.h"

void ThreadHandler::AddThread(void(thCallback(void)), Game& game)
{
	threads.emplace_back(std::thread(thCallback, game));
}

void ThreadHandler::WaitThreads()
{
	for (size_t i = 0; i < threads.size(); ++i)
	{
		if (threads.at(i).joinable())
			threads.at(i).join();
	}
}

void ThreadHandler::Stop()
{
	stopRequest = true;
}
/*
void threadTemplate(void(thCallback(Game*)), Game* aClass)
{
	while (1)
	{
		thCallback(aClass);
	}
}

/*
void ThreadHandler::AddThread(void(thCallback()))
{
	threads.emplace_back(std::thread(threadTemplate, thCallback));
}

void ThreadHandler::WaitThreads()
{
	for (size_t i = 0; i < threads.size(); ++i)
	{
		if (threads.at(i).joinable())
			threads.at(i).join();
	}
}

void ThreadHandler::Stop()
{
	stopRequest = true;
}

void threadTemplate(void(thCallback()))
{
	while (1)
	{
		thCallback();
	}
}
*/