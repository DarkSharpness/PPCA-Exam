#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>

// 简单的哈希函数
unsigned int simpleHash(const std::string &input) {
	unsigned int hash = 0;
	for (char c: input) {
		hash = ((hash * 31) + c) % 100000007;
	}
	return hash;
}

// 生成随机字符串
std::string generateRandomString(size_t length) {
	std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string result;
	for (size_t i = 0; i < length; ++i) {
		result += chars[rand() % chars.size()];
	}
	return result;
}

int main() {
	auto target_hash = 0x46ee11e;
	std::unordered_map<unsigned int, std::string> hashMap;
	srand(time(0));         // 初始化随机种子
	size_t stringLength = 8;// 生成的随机字符串长度

	clock_t start = clock();// 记录开始时间
	
	while (true) {
		std::string randomString = generateRandomString(stringLength);
		unsigned int hashValue = simpleHash(randomString);
		if (hashValue == target_hash) {
			std::cout << "Collision found!" << std::endl;
			std::cout << "String: " << randomString << std::endl;
			std::cout << "Hash Value: " << hashValue << std::endl;
			break;
		}
		// // 检查是否存在相同哈希值的不同字符串
		// if (hashMap.find(hashValue) != hashMap.end()) {
		// 	if (hashMap[hashValue] != randomString) {
		// 		clock_t end = clock();// 记录结束时间

		// 		std::cout << "Collision found!" << std::endl;
		// 		std::cout << "String 1: " << hashMap[hashValue] << std::endl;
		// 		std::cout << "String 2: " << randomString << std::endl;
		// 		std::cout << "Hash Value: " << hashValue << std::endl;
		// 		break;
		// 	}
		// }
		// else {
		// 	hashMap[hashValue] = randomString;
		// }
	}
	clock_t end = clock();// 记录结束时间
	double duration = double(end - start) / CLOCKS_PER_SEC;
	std::cout << "Time taken: " << duration << " seconds" << std::endl;
	return 0;
}
