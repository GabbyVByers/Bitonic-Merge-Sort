
#include "profiler.h"

void Profiler::start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void Profiler::stop()
{
	stopTime = std::chrono::high_resolution_clock::now();
	duration = stopTime - startTime;
}

double Profiler::time() const
{
	return duration.count();
}

