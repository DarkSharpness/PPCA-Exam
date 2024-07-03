#include "vm.h"
#include <fstream>

#include <chrono>
#include <random>


int main(int argc, char *argv[]) {
	if (argc < 6) {
		std::cout << "Usage: " << argv[0] << " <_input> <output> <_answer> <score> <message>" << std::endl;
	}
	std::fstream output(argv[2], std::ios::in), score(argv[4], std::ios::out), message(argv[5], std::ios::out);
	unsigned long long cur_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::default_random_engine generator(cur_time);
	std::uniform_int_distribution<unsigned int> d1(0, 1000);
	std::uniform_int_distribution<unsigned long long> d2;
	auto seed = d1(generator);
	auto flag = d2(generator);
	unsigned long long result = 0;
	try {
		VM vm{output, seed, flag};
		std::cout << "Running...";
		result = vm.run();
	} catch (std::runtime_error &e) {
		score << 0;
		message << "Runtime error:" << e.what() << std::endl;
		return 0;
	} catch (...) {
		score << 0;
		message << "Unknown error" << std::endl;
		return 0;
	}
	if (result == flag) {
		score << 1;
		message << "Ok you got the data inner the subsystem.";
	}
	else {
		score << 0;
		message << "Wrong Answer\n" << "Expected: " << flag << "\n" << "Your output: " << result << std::endl;
	}
	return 0;
}
