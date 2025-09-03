
#include <iostream>
#include <iomanip>
#include "profiler.h"

static void bitonicMergeSortWithCUDA(unsigned int* array, unsigned int length)
{

}

static void naiveSort(unsigned int* array, unsigned int length)
{
	while (true)
	{
		bool isSorted = true;
		for (unsigned int i = 0; i < length - 1; i++)
		{
			unsigned int& curr = array[i];
			unsigned int& next = array[i + 1];
			if (curr > next)
			{
				isSorted = false;
				unsigned int temp = curr;
				curr = next;
				next = temp;
			}
		}
		if (isSorted)
			return;
	}
}

int main()
{
	Profiler profiler;
	double time1 = 0.0;
	double time2 = 0.0;

	unsigned int length = 100000;
	unsigned int* array1 = new unsigned int[length];
	unsigned int* array2 = new unsigned int[length];

	for (unsigned int i = 0; i < length; i++)
	{
		unsigned int random_value = rand();
		array1[i] = random_value;
		array2[i] = random_value;
	}

	profiler.start();
	naiveSort(array1, length);
	profiler.stop();
	time1 = profiler.time();

	profiler.start();
	bitonicMergeSortWithCUDA(array1, length);
	profiler.stop();
	time2 = profiler.time();

	std::cout << "Number of Elements: " << length << "\n";
	std::cout << "Naive Sorting Time         (ms): " << time1 << "\n";
	std::cout << "Bitonic Merge Sorting Time (ms): " << time2 << "\n";
	std::cout << "Bitonic is " << (time1 / time2) << " times faster\n";

	delete[] array1;
	delete[] array2;
	return 0;
}

