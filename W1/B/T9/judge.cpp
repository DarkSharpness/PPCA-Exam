#include "game.h"// Include the header file you provided
#include <iostream>
#include <unistd.h>

int main() {
	int n = 0;
	std::cin >> n;
	int shape_id, col, rotation_deg;
	for (int i = 0; i < n; ++i) {
		std::cin >> shape_id >> col >> rotation_deg;
		game.step(shape_id, col, rotation_deg);
	}
	game.game_over();
	return 0;
}
