#include "game.h"

int main() {
	// TODO: your code here
	// you could call game.step() any times as you want
	// DO NOT output anything to stdout(cout)
	// finally, you can call game.game_over() or call game.step with a invalid parameter to end the game

	// example code
	game.step(0, 0, 90);
	game.step(0, 3, 270);
	game.game_over();

	return 0;
}
