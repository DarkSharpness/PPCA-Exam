#pragma once
#include <iostream>
#include <random>
#include <chrono>

static void verify_output(long long score) {
	unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<unsigned long long> distribution;
	auto a = distribution(generator), b = distribution(generator);
	auto c = a ^ b ^ score;
	std::cout << a << ' ' << b << ' ' << c << std::endl;
}
