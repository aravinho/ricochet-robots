#ifndef BOARD_H
#define BOARD_H

#include "robots.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

extern int BOARD_DIMENSION;


// utils

// inserts the given element into the given vector, in its sorted position
// doesnt insert if already exists
void insertInOrder(vector<int>* vec, int element);

class DiagBarrier {
	string color;
	bool direction; // true if forward, false is back

public:

	DiagBarrier(string color, bool direction) : color(color), direction(direction) {}
	bool isForward() const;
	bool isBackward() const;
	const string& getColor() const;
};


class Board {

	vector<vector<int>> row_barriers; // row_barriers[i] contains the col indices of each row barrier (vertical) in row i
	vector<vector<int>> col_barriers; // col_barriers[j] contains the row indices of each col barrier (horizontal) in row j
	vector<vector<pair<int, DiagBarrier>>> diag_barriers; // diag_barriers[i][j] points to the diag barrier at position (i, j)

public:

	// initializes vectors to the correct size
	Board(int board_dimension=BOARD_DIMENSION);

	// returns whether there is a solution
	// populates the solution vector with the series of moves it will take to move the MOVING_ROBOT to the DEST
	bool solve(const RobotArrangement& robots, const Position& dest, const string& moving_robot, vector<Move>* solution, 
		int max_depth=20);


	bool hasRowBarrier(int row, int col) const;
	bool hasColBarrier(int col, int row) const;
	bool hasDiagBarrier(int row, int col) const;
	DiagBarrier getDiagBarrier(int row, int col) const;

	void setRowBarrier(int row, int col);
	void setColBarrier(int col, int row);
	void setDiagBarrier(int row, int col, const string& color, bool is_forward);


	void display(const RobotArrangement& robots, const Position& dest) const;

	// overwrites robot_positions with the results of making the given move on this board.
	// returns true if the move actually did something
	bool makeMove(Move move, RobotArrangement* robot_positions) const;

	bool eastMove(const string& moving_robot_color, RobotArrangement* robot_positions) const;
	bool westMove(const string& moving_robot_color, RobotArrangement* robot_positions) const;
	bool northMove(const string& moving_robot_color, RobotArrangement* robot_positions) const;
	bool southMove(const string& moving_robot_color, RobotArrangement* robot_positions) const;






};

#endif