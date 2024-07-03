#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> solver(std::vector<int> problem, long long target_sum);
#include "src.hpp"
// subset sum problem

std::vector<int> problem;
long long target_sum = 0;

void read_problem_from_stdin() {
	// n sum
	// a1 a2 a3 ... an
	int n = 0;
	std::cin >> n >> target_sum;
	problem.resize(n);
	for (int i = 0; i < n; ++i)
		std::cin >> problem[i];
}

bool check_answer(std::vector<int> &answer) {
	// check 1: is sum match
	long long sum = 0;
	for (int i = 0; i < answer.size(); ++i)
		sum += answer[i];
	if (sum != target_sum)
		return false;
	// check 2: is subset
	std::sort(answer.begin(), answer.end());
	std::sort(problem.begin(), problem.end());
	int p = 0;
	for (auto x: answer) {
		while (p < problem.size() && problem[p] < x)
			++p;
		if (p == problem.size() || problem[p] != x)
			return false;
		++p;
	}
	return true;
}

int main() {
	read_problem_from_stdin();
	std::string token;
	std::cin >> token;

	auto cout_bak = std::cout.rdbuf();
	auto res = solver(problem, target_sum);
	std::cout.rdbuf(cout_bak);

	auto correct = check_answer(res);
	if (correct) {
		std::cout << "Correct\n";
	}
	else {
		std::cout << "Incorrect\n";
	}
	std::cout << token << std::endl;
	return 0;
}
