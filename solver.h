#ifndef SOLVER_H
#define SOLVER_H

#include "robots.h"
#include "board.h"

using namespace std;

extern int DEFAULT_MAX_DEPTH;

// logs message with timestamp
void log(string msg);

// iteratively runs depth limited dfs, incrementing depth each time
int solveRicochetBoard(const Board& board, const RobotArrangement& robot_positions, const Position& dest,
	const string& dest_color, vector<Move>* moves, int max_depth=DEFAULT_MAX_DEPTH);

// returns true if there is a solution of (max_depth - depth) fewer moves,
// starting at robot_positions, and getting the given robot to DEST.
// if true, the vector Moves will contain all the moves, if false, could be anything
int solveDepthLimitedDfs(const Board& board, const RobotArrangement& robot_positions, const Position& dest,
	const string& dest_color, vector<Move>* moves, map<RobotArrangementEncoding, int>* visited_depths,
	int depth=0, int max_depth=DEFAULT_MAX_DEPTH);

// helper function. returns true if this arrangement hasnt been seen
// or, when previously seen, it wasn't expanded to the depth that it will be now
bool worthExpanding(const RobotArrangement& robot_positions, 
	map<RobotArrangementEncoding, int>* visited_depths, int depth, int max_depth);

#endif