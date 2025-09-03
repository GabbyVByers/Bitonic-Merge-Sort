#pragma once

#include <chrono>

class Profiler
{
public:

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point stopTime;
	std::chrono::duration<double, std::milli> duration = std::chrono::duration<double, std::milli>(0);

	Profiler() {}
	~Profiler() {}

	void start();
	void stop();
	double time() const;
};

