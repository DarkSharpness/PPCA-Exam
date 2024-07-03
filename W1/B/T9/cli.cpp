#include "game.h"// Include the header file you provided
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

void clear_screen() {
	std::cout << "\033[H\033[J";
}

void print_board(decltype(game.get_board()) &board) {
	clear_screen();
	for (int i = boardHeight - 1; i >= 0; --i) {
		for (int j = 0; j < boardWidth; ++j) {
			std::cout << (board[i][j] ? '#' : '.');
		}
		std::cout << '\n';
	}
}

int main() {
	while (true) {
		print_board(game.get_board());

		std::string input;
		std::cout << "Enter shape id, column, and rotation degree (space separated): ";
		std::getline(std::cin, input);
		if (std::cin.eof())
			break;
		std::istringstream iss(input);
		int shape_id, col, rotation_deg;
		if (!(iss >> shape_id >> col >> rotation_deg)) {
			std::cerr << "Invalid input format." << std::endl;
			continue;
		}
		game.step(shape_id, col, rotation_deg);
	}

	return 0;
}
