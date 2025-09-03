
#include <iostream>
#include "profiler.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void kernelBitonicMergeSort(unsigned int* device_array, int j, int k, int length)
{
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= (unsigned)length) return;

	unsigned int ixj = idx ^ j;

	if (ixj > idx)
	{
		if ((idx & k) == 0)
		{
			if (device_array[idx] > device_array[ixj])
			{
				int tmp = device_array[idx];
				device_array[idx] = device_array[ixj];
				device_array[ixj] = tmp;
			}
		}
		else
		{
			if (device_array[idx] < device_array[ixj])
			{
				int tmp = device_array[idx];
				device_array[idx] = device_array[ixj];
				device_array[ixj] = tmp;
			}
		}
	}
}

static void bitonicMergeSortWithCUDA(unsigned int* array, unsigned int length)
{
	unsigned int* device_array = nullptr;
	cudaMalloc((void**)&device_array, length * sizeof(unsigned int));
	cudaMemcpy(device_array, array, length * sizeof(unsigned int), cudaMemcpyHostToDevice);

	const int THREADS = 256;
	const int BLOCKS = (length + THREADS - 1) / THREADS;

	for (int k = 2; k <= length; k <<= 1)
	{
		for (int j = k >> 1; j > 0; j >>= 1)
		{
			kernelBitonicMergeSort <<<BLOCKS, THREADS>>> (device_array, j, k, length);
			cudaDeviceSynchronize();
		}
	}

	cudaMemcpy(array, device_array, length * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	cudaFree(device_array);
	return;
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

static bool isSorted(unsigned int* array, unsigned int length)
{
	for (int i = 0; i < length - 1; i++)
	{
		unsigned int& curr = array[i];
		unsigned int& next = array[i + 1];
		if (curr > next)
			return false;
	}
	return true;
}

int main()
{
	Profiler profiler;
	double time1 = 0.0;
	double time2 = 0.0;

	unsigned int length = 131072;
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
	bitonicMergeSortWithCUDA(array2, length);
	profiler.stop();
	time2 = profiler.time();

	std::cout << "Number of Elements: " << length << "\n";
	isSorted(array1, length) ? std::cout << "Array 1 is Sorted\n" : std::cout << "Array 1 is NOT Sorted\n";
	isSorted(array2, length) ? std::cout << "Array 2 is Sorted\n" : std::cout << "Array 2 is NOT Sorted\n";
	std::cout << "Naive Sort   (seconds): " << time1 / 1000.0 << "\n";
	std::cout << "Bitonic Sort (seconds): " << time2 / 1000.0 << "\n";

	delete[] array1;
	delete[] array2;
	return 0;
}

