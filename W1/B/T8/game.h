#pragma once

#include "verify.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

constexpr int cntShapes = 7;

struct Shape {
	// <down, right>
	std::pair<int, int> points[5];
};

// XXX
// X
static auto shapeL1 = Shape{{{0, 0}, {0, 1}, {0, 2}, {1, 0}}};
// XXX
//   X
static auto shapeL2 = Shape{{{0, 0}, {0, 1}, {0, 2}, {1, 2}}};
// XXX
//  X
static auto shapeT1 = Shape{{{0, 0}, {0, 1}, {0, 2}, {1, 1}}};
// XX
//  XX
static auto shapeZ1 = Shape{{{0, 0}, {0, 1}, {1, 1}, {1, 2}}};
//  XX
// XX
static auto shapeZ2 = Shape{{{0, 1}, {0, 2}, {1, 0}, {1, 1}}};
// X
// XXX
//   X
static auto shapeN1 = Shape{{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}}};
//   X
// XXX
// X
static auto shapeN2 = Shape{{{0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}}};


constexpr unsigned int boardWidth = 10;
constexpr unsigned int boardHeight = 20;

class Tetris {
private:
	unsigned int score = {};
	int cnt[cntShapes] = {};
	Shape const *shapes[cntShapes] = {&shapeL1, &shapeL2, &shapeT1, &shapeZ1, &shapeZ2, &shapeN1, &shapeN2};
	bool board[boardHeight][boardWidth] = {};

private:
	int get_down_len(int col) {
		for (int i = 0; i < boardHeight; ++i)
			if (board[boardHeight - 1 - i][col])
				return i;
		return boardHeight;
	}
	void update_board() {
		for (int i = 0; i < boardHeight; ++i) {
			bool full = true;
			for (int j = 0; j < boardWidth; ++j)
				if (!board[i][j]) {
					full = false;
					break;
				}
			if (full) {
				for (int j = i; j < boardHeight - 1; ++j)
					for (int k = 0; k < boardWidth; ++k)
						board[j][k] = board[j + 1][k];
				for (int j = 0; j < boardWidth; ++j)
					board[boardHeight - 1][j] = false;
				if (++score >= 40)
					game_over();
			}
		}
	}

public:
	explicit Tetris() {
		for (auto &c: cnt)
			c = 100;
	}
	void step(int type_k, int col, int rotate_deg) {
		if (type_k >= cntShapes) {
			std::cerr << "Invalid shape type." << std::endl;
			return game_over();
		}
		if (--cnt[type_k] < 0) {
			std::cerr << "No more shape of this type." << std::endl;
			return game_over();
		}
		if (rotate_deg != 0 && rotate_deg != 90 && rotate_deg != 180 && rotate_deg != 270) {
			std::cerr << "Invalid rotation degree." << std::endl;
			return game_over();
		}
		Shape sp = *shapes[type_k];
		// 顺时针
		for (int i = 0; i < rotate_deg / 90; ++i)
			for (auto &p: sp.points) {
				std::swap(p.first, p.second);
				p.second = -p.second;
			}
		int v_max = -10000;
		for (auto &p: sp.points) {
			// get actual position
			p.second += col;
			v_max = std::max(v_max, p.first);
		}
		for (auto &p: sp.points)
			p.first -= v_max;
		int max_down_len = 1919810;
		for (auto &p: sp.points)
			if (p.second >= 0 && p.second < boardWidth)
				max_down_len = std::min(max_down_len, get_down_len(p.second) - p.first);
		for (auto &p: sp.points) {
			int x = p.second, y = boardHeight - max_down_len - p.first;
			if (!(0 <= x && x < boardWidth && 0 <= y && y < boardHeight))
				continue;
			board[y][x] = true;
		}
		update_board();
	}
	decltype(board) const &get_board() {
		return board;
	}
	void game_over() {
		std::cout << "Game Over" << std::endl;
		std::cout << score << std::endl;
		verify_output(score);
		exit(0);
	}
};

static Tetris game;
