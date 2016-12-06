#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <array>

#include <stdio.h>

#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include "device_launch_parameters.h"

__constant__ char* target = "pole";
__constant__ int target_length = 4;

__global__ void part1(char* d_hashes, int* d_values, int* d_limits, char* d_checksums, int size) {
	int id = (blockIdx.x * blockDim.x) + threadIdx.x;
	if (id >= size) return;
	int count[26] = { 0 };
	int order[5] = { 0 };

	//check number letters
	for (int i = d_limits[id]; i < d_limits[id + 1]; ++i)
		if (d_hashes[i] != '-')
			++count[d_hashes[i] - 'a'];

	//check most common letters
	for (int steps = 0; steps < 5; ++steps) {
		for (int i = 25; i >= 0; --i) {
			if (count[order[steps]] <= count[i]) {
				order[steps] = i;
			}
		}
		count[order[steps]] = -1;
	}

	//validate checksum
	for (int i = 0; i < 5; ++i) {
		if (order[i] + 'a' != d_checksums[(id * 5) + i]) {
			d_values[id] = 0;  //set to 0 to neutralize the decoy
			break;
		}
	}
}

__global__ void part2(char* d_hashes, int* d_values, int* d_limits, char* d_checksums, int size) {
	int id = (blockIdx.x * blockDim.x) + threadIdx.x;
	if (id >= size) return;  //thread is out of bound
	if (d_values[id] == 0) return;  //this is a decoy

	//Apply cypher
	for (int i = d_limits[id]; i < d_limits[id + 1]; ++i) {
		d_hashes[i] = (((d_hashes[i] - 'a') + d_values[id]) % 26) + 'a';
	}

	//look for target word
	for (int i = d_limits[id]; i < d_limits[id + 1]; ++i) {
		if (d_limits[id + 1] > i + target_length) {
			int nbr_valid = 0;
			for (int j = 0; j < target_length; ++j) {
				if (d_hashes[i+j] == target[j]) {
					++nbr_valid;
				}
			}
			if (nbr_valid == target_length) return; //legit - keep the value
		}
	}
	d_values[id] = 0; //target isn't found, set value to zero
}

int main() {
	std::string hash_str{};
	std::string check_str{};
	thrust::host_vector<int> values{};
	thrust::host_vector<int> limits{};
	limits.push_back(0);

	{ //File manipulation
		std::ifstream file("input.txt");
		std::string line = "";
		std::regex reg("([a-z,-]+)-([0-9]+).([a-z]+).");
		std::smatch cm;

		while (std::getline(file, line)) {
			std::regex_match(line, cm, reg);
			values.push_back(std::stoi(cm[2]));
			hash_str += cm[1];
			check_str += cm[3];
			limits.push_back(hash_str.size());
		}
		file.close();
	}

	char* d_hashes = 0;
	char* d_checksums = 0;

	cudaMalloc((void**)&d_hashes, sizeof(char) * hash_str.size());
	cudaMemcpy(d_hashes, hash_str.data(), sizeof(char) * hash_str.size(), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&d_checksums, sizeof(char) * check_str.size());
	cudaMemcpy(d_checksums, check_str.data(), sizeof(char) * check_str.size(), cudaMemcpyHostToDevice);

	thrust::device_vector<int> d_values = values;
	thrust::device_vector<int> d_limits = limits;

	//Neutralize decoys
	part1 << < 1 + (values.size() / 32), 32 >> > (d_hashes,
		thrust::raw_pointer_cast(d_values.data()),
		thrust::raw_pointer_cast(d_limits.data()),
		d_checksums,
		values.size());

	cudaDeviceSynchronize();
	int sum = thrust::reduce(d_values.begin(), d_values.end());  //All the decoy's values are set to 0
	std::cout << "The sum of sectors is: " << std::to_string(sum) << std::endl;

	//Neutralize anything that does't contain the target word
	part2 << < values.size(), 1 >> > (d_hashes,
		thrust::raw_pointer_cast(d_values.data()),
		thrust::raw_pointer_cast(d_limits.data()),
		d_checksums,
		values.size());

	cudaDeviceSynchronize();
	sum = thrust::reduce(d_values.begin(), d_values.end()); //Normally, only one value is left
	std::cout << "The north pole's sector ID is: " << std::to_string(sum) << std::endl;  

	std::cin.get();
	return 0;
}