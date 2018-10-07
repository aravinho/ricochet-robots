#include "solver.h"
#include <iostream>
#include <time.h>

using namespace std;

int DEFAULT_MAX_DEPTH = 20;

void log(string msg) {
	cout << msg << " -  ";
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d, %X", &tstruct);
    cout << buf << endl;

}

int solveRicochetBoard(const Board& board, const RobotArrangement& robot_positions, const Position& dest,
	const string& dest_color, vector<Move>* moves, int max_depth) {

	map<RobotArrangementEncoding, int> visited_depths;

	for (int depth_limit = 0; depth_limit < max_depth; depth_limit++) {
		log("Looking for solutions at depth " + to_string(depth_limit));

		int solution_length = solveDepthLimitedDfs(board, robot_positions, dest, dest_color, moves, &visited_depths, 0, depth_limit);
		bool solved = solution_length != -1;
		if (solved) {
			return depth_limit;
		}
	}

	return -1;
}


// map should hold how many steps were left when we visited this previously
bool worthExpanding(const RobotArrangement& robot_positions, 
	map<RobotArrangementEncoding, int>* visited_depths, int depth, int max_depth) {

	RobotArrangementEncoding encoding = encode(robot_positions);
	int num_steps_left = max_depth - depth;

	// if never been here, add it to map and return true
	if (visited_depths->count(encoding) == 0) {
		visited_depths->insert(make_pair(encoding, num_steps_left));
		return true;
	} else {
		// if visited already at an earlier depth, no point
		int prev_num_steps_left = visited_depths->at(encoding);
		if (prev_num_steps_left >= num_steps_left) {
			return false;
		} else {
			// if visited before, but at a deeper depth, might still be worth
			visited_depths->at(encoding) = num_steps_left;
			return true;
		}

	}

	return false;
}


int solveDepthLimitedDfs(const Board& board, const RobotArrangement& robot_positions, const Position& dest,
	const string& dest_color, vector<Move>* moves,
	map<RobotArrangementEncoding, int>* visited_depths, int depth, int max_depth) {

	if (depth > max_depth) {
		return -1;
	}

	if (robot_positions.isSolution(dest, dest_color)) {
		return depth; // assume the move to get here was already appended to moves
	}

	// determine if worth expanding based on if we've seen this before
	bool worth_expanding = worthExpanding(robot_positions, visited_depths, depth, max_depth);
	if (!worth_expanding) {
		return -1;
	}


	for (Move move : all_moves) {
		RobotArrangement new_robot_positions(robot_positions);
		bool piece_moves = board.makeMove(move, &new_robot_positions); // updates new_robot_positions according to the new move
		if (piece_moves) {
			moves->push_back(move);
			int solution_length = solveDepthLimitedDfs(board, new_robot_positions, dest, dest_color, moves, visited_depths,
				depth + 1, max_depth);
			bool is_solution = solution_length != -1;
			if (is_solution) {
				return solution_length;
			} else {
				moves->pop_back();
			}
		}
	}

	return -1;
}