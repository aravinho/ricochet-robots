#include "board.h"
#include "solver.h"
#include "quadrant.h"
#include <iostream>

using namespace std;

int BOARD_DIMENSION = 16;


// utils
void insertInOrder(vector<int>* vec, int element) {
	ASSERT(vec != NULL, "cannot insert into a null vec");
	if (find(vec->begin(), vec->end(), element) == vec->end()) {
		vec->push_back(element);
		sort(vec->begin(), vec->end());
	}
}

void insertDiagInOrder(vector<pair<int, DiagBarrier>>* vec, pair<int, DiagBarrier> new_diag) {

	vector<pair<int, DiagBarrier>> updated_diags;
	bool have_inserted = false;
	int new_diag_col = new_diag.first;
	// assumes up till now, is in order, and that there is no diag barrier for the given col
	for (pair<int, DiagBarrier> diag : *vec) {
		int col = diag.first;
		if (col > new_diag_col && !have_inserted) {
			updated_diags.push_back(new_diag);
			have_inserted = true;
		}
		
		updated_diags.push_back(diag);
	}

	if (!have_inserted) {
		updated_diags.push_back(new_diag);
	}

	int new_num_diags = updated_diags.size();
	for (int i = 0; i < new_num_diags; i++) {
		if (i < vec->size()) {
			vec->at(i) = updated_diags[i];
		} else {
			vec->push_back(updated_diags[i]);
		}
	}
}




bool DiagBarrier::isForward() const {
	return this->direction;
}

bool DiagBarrier::isBackward() const {
	return !this->direction;
}

const string& DiagBarrier::getColor() const {
	return this->color;
}




Board::Board(int board_dimension) {

	for (int row = 0; row < board_dimension; row++) {
		// set edge of board barriers
		vector<int> rb_indices;
		this->row_barriers.push_back(rb_indices);
		insertInOrder(&(this->row_barriers[row]), 0);
		insertInOrder(&(this->row_barriers[row]), board_dimension + 1);

		// initialize diag list
		vector<pair<int, DiagBarrier>> db_indices;
		this->diag_barriers.push_back(db_indices);
	}

	for (int col = 0; col < board_dimension; col++) {
		// set edge of board barriers
		vector<int> cb_indices;
		this->col_barriers.push_back(cb_indices);
		insertInOrder(&(this->col_barriers[col]), 0);
		insertInOrder(&(this->col_barriers[col]), board_dimension);
	}

}

bool Board::solve(const RobotArrangement& robots, const Position& dest, const string& moving_robot, vector<Move>* solution, 
		int max_depth) {

	return true;
}

bool Board::hasRowBarrier(int row, int col) const {
	ASSERT(0 <= row && row < BOARD_DIMENSION, "Row " << row << " cannot have a row barrier");
	vector<int> rb_indices = this->row_barriers[row];
	return find(rb_indices.begin(), rb_indices.end(), col) != rb_indices.end();

}

bool Board::hasColBarrier(int col, int row) const {
	vector<int> cb_indices = this->col_barriers[col];
	return find(cb_indices.begin(), cb_indices.end(), row) != cb_indices.end();
}

bool Board::hasDiagBarrier(int row, int col) const {
	vector<pair<int, DiagBarrier>> diag_indices = this->diag_barriers[row];
	for (pair<int, DiagBarrier> diag : this->diag_barriers[row]) {
		if (diag.first == col) {
			return true;
		}
	}
	
	return false;
}

DiagBarrier Board::getDiagBarrier(int row, int col) const {
	ASSERT(this->hasDiagBarrier(row, col), "no diag barrier at " << row << ", " << col);
	for (pair<int, DiagBarrier> diag : this->diag_barriers[row]) {
		if (diag.first == col) {
			return diag.second;
		}
	}
	ASSERT(false, "should not be here");
}


void Board::setRowBarrier(int row, int col) {
	ASSERT(this->row_barriers.size() >= row + 1, "row barriers not long enough to set in row " << row);
	ASSERT(0 <= col and col <= BOARD_DIMENSION, "col " << col << " out of range");
	insertInOrder(&(this->row_barriers[row]), col);
}

void Board::setColBarrier(int col, int row) {
	ASSERT(this->col_barriers.size() >= col + 1, "col barriers not long enough to set in col " << col);
	ASSERT(0 <= row and row <= BOARD_DIMENSION, "row " << row << " out of range");
	insertInOrder(&(this->col_barriers[col]), row);
}

void Board::setDiagBarrier(int row, int col, const string& color, bool is_forward) {
	ASSERT(this->diag_barriers.size() >= row + 1, "row barriers not long enough to set diag in row " << row);
	ASSERT(0 <= col and col < BOARD_DIMENSION, "col " << col << " out of range");
	DiagBarrier db(color, is_forward);
	insertDiagInOrder(&(this->diag_barriers[row]), make_pair(col, db));
}



void Board::display(const RobotArrangement& robots, const Position& dest) const {

	// print top wall
	string top_wall = ".";
	for (int col = 0; col < BOARD_DIMENSION; col++) {
		top_wall += "___.";
	}
	cout << top_wall << endl;

	// prepare to print horizontal barriers (col barriers) in each column
	vector<int> num_col_barriers(BOARD_DIMENSION);
	vector<int> next_col_barrier_index(BOARD_DIMENSION);
	vector<int> next_col_barrier(BOARD_DIMENSION);

	for (int col = 0; col < BOARD_DIMENSION; col++) {
		num_col_barriers[col] = this->col_barriers[col].size();
		next_col_barrier_index[col] = 1;
		next_col_barrier[col] = this->col_barriers[col][1];
	}

	// print each row
	for (int row = 0; row < BOARD_DIMENSION; row++) {

		// print row left wall
		string row_str = "|";

		//prepare to print vertical barriers (row barriers) and diags in this row
		int num_barriers = this->row_barriers[row].size();
		int next_barrier_index = 1;
		int next_barrier_pos = this->row_barriers[row][1]; // has at least 0 and 16

		int num_diags = this->diag_barriers[row].size();
		int next_diag_index = (num_diags > 0) ? 0 : -1;
		int next_diag_pos = (num_diags > 0) ? this->diag_barriers[row][0].first : -1;

		for (int col = 0; col < BOARD_DIMENSION; col++) {

			// determine whether there is a robot
			bool has_robot = false;
			bool robot_above = false;
			bool robot_below = false;
			string robot_str = "";
			map<string, string> robot_colors = {{"yellow", "Y"}, {"red", "R"}, {"green", "G"}, {"blue", "B"}};

			for (auto robot_color : robot_colors) {
				string color = robot_color.first;
				if (robots.getRow(color) == row && robots.getCol(color) == col) {
					has_robot = true;
					robot_above = robots.getAboveDiag(color);
					robot_below = !robot_above;
					robot_str = robot_colors[color];
				}
			}


			// determine whether this spot is the destination
			if (dest.getRow() == row && dest.getCol() == col) {
				has_robot = true;
				robot_above = true;
				robot_below = false;
				robot_str = "*";
			}

			// determine whethr there is a diag at this spot
			bool forward_diag = next_diag_pos == col && this->diag_barriers[row][next_diag_index].second.isForward();
			bool back_diag = next_diag_pos == col && this->diag_barriers[row][next_diag_index].second.isBackward();

			// determine whether there is a col barrier (horizontal) at this spot
			bool col_barrier = (row + 1) == next_col_barrier[col];


			// Print the appropriate robot/diag/col barrier, if necessary
				
			if (forward_diag && col_barrier) {
				if (has_robot && robot_above) {
					row_str += robot_str + "/_";
				} else {
					row_str += "/" + robot_str + "_";
				}
			}

			else if (back_diag && col_barrier) {
				if (has_robot) {
					row_str += robot_above ? "\\" + robot_str + "_" : robot_str + "\\" + "_";
				} else {
					row_str += robot_str + "\\" + "_";
				}
			}
			
			else if (forward_diag) {
				if (has_robot) {
					row_str += robot_above ? robot_str + "/ " : " /" + robot_str;
				} else {
					row_str += " / ";
				}
			}


			else if (back_diag) {
				if (has_robot) {
					row_str += robot_above ? " \\" + robot_str : robot_str + "\\ ";
				} else {
					row_str += " \\ ";
				}
			}

			else if (col_barrier) {
				row_str += has_robot ? "_" + robot_str + "_" : "___";
			}

			else {
				row_str += has_robot ? " " + robot_str + " " : "   ";
			}


			// prepare for the next column barrier
			if (col_barrier) {
				next_col_barrier_index[col] += 1;
				if (next_col_barrier_index[col] < num_col_barriers[col]) {
					next_col_barrier[col] = this->col_barriers[col][next_col_barrier_index[col]];
				} else {
					next_col_barrier[col] = -1;
				}
			}


			// print dot or a vertical barrier
			if ((col + 1) == next_barrier_pos) {
				row_str += "|";
				next_barrier_index += 1;
				if (next_barrier_index < num_barriers) {
					next_barrier_pos = this->row_barriers[row][next_barrier_index];
				} else {
					next_barrier_pos = -1;
				}
			} else {
				row_str += ".";
			}	

		}


		cout << row_str << endl;
	}

	cout << endl;
}



bool Board::makeMove(Move move, RobotArrangement* robot_positions) const {

	string moving_robot_color = getColor(move);
	string direction = getDirection(move);

	if (direction == "east") {
		return this->eastMove(moving_robot_color, robot_positions);
	} else if (direction == "west") {
		return this->westMove(moving_robot_color, robot_positions);
	} else if (direction == "north") {
		return this->northMove(moving_robot_color, robot_positions);
	} else if (direction == "south") {
		return this->southMove(moving_robot_color, robot_positions);
	}

	return true;
}


bool Board::eastMove(const string& moving_robot_color, RobotArrangement* robot_positions) const {

	Position initial_position = robot_positions->getPosition(moving_robot_color);
	int moving_robot_row = robot_positions->getRow(moving_robot_color);
	int moving_robot_col = robot_positions->getCol(moving_robot_color);
	bool moving_robot_above_diag = robot_positions->getAboveDiag(moving_robot_color);
	bool initial_moving_robot_above_diag = moving_robot_above_diag;

	// look for first barrier east, could be row barrier (vertical), a diag barrier or another robot
	int first_barrier_east = BOARD_DIMENSION;
	string first_barrier_type = "barrier";

	// search for that first barrier going east
	for (int col = moving_robot_col; col < BOARD_DIMENSION; col++) {

		// check if there is a robot on the square
		bool is_blocker_robot = robot_positions->isRobotAt(moving_robot_row, col, {moving_robot_color});
		if (is_blocker_robot) {
			first_barrier_east = col;
			first_barrier_type = "robot";
			break;
		}

		// check if there is a diag barrier
		if (this->hasDiagBarrier(moving_robot_row, col)) {
			DiagBarrier diag = this->getDiagBarrier(moving_robot_row, col);

			// if the diag is same color, passes through
			// change above/below diag, in case there is a same color diag followed immediately by a barrier
			if (diag.getColor() == moving_robot_color) {
				moving_robot_above_diag = diag.isForward() ? false : true;
			}

			// if diag is of different color, it will deflect
			else {
				if (diag.isForward()) {
					// if diag is initially on a forward diag square, and is already below diag, nothing
					if (!(col == moving_robot_col && !moving_robot_above_diag)) {
						first_barrier_east = col;
						first_barrier_type = "forward";
						moving_robot_above_diag = true;
						break;
					}
				}

				else if (diag.isBackward()) {

					// if robot is initially on back diag square, and is above that diag, nothing happens
					if (!(col == moving_robot_col && moving_robot_above_diag)) {
						first_barrier_east = col;
						first_barrier_type = "backward";
						moving_robot_above_diag = false;
						break;
					}
				}
			}
		}

		// if neither a robot nor a diag, check if barrier
		if (this->hasRowBarrier(moving_robot_row, col + 1)) {
			first_barrier_east = col + 1;
			first_barrier_type = "barrier";
			break;
		}

	}

	// based on the type of the first barrier decide what to do

	if (first_barrier_type == "barrier" || first_barrier_type == "robot") {
		// if barrier or robot, moving robot stops
		Position new_position = Position(moving_robot_row, first_barrier_east - 1, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;

	} else if (first_barrier_type == "forward") {
		// if forward diag, deflects north
		Position new_position = Position(moving_robot_row, first_barrier_east, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->northMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	
	}

	else if (first_barrier_type == "backward") {
		// if backward diag, deflects south
		Position new_position = Position(moving_robot_row, first_barrier_east, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->southMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	ASSERT(false, "invalid barrier type " << first_barrier_type);


}


bool Board::westMove(const string& moving_robot_color, RobotArrangement* robot_positions) const {
	
	Position initial_position = robot_positions->getPosition(moving_robot_color);
	int moving_robot_row = robot_positions->getRow(moving_robot_color);
	int moving_robot_col = robot_positions->getCol(moving_robot_color);
	bool moving_robot_above_diag = robot_positions->getAboveDiag(moving_robot_color);
	bool initial_moving_robot_above_diag = moving_robot_above_diag;

	// look for first barrier west, could be row barrier (vertical), a diag barrier or another robot
	int first_barrier_west = 0;
	string first_barrier_type = "barrier";

	// search for that first barrier going east
	for (int col = moving_robot_col; col >= 0; col--) {

		// check if there is a robot on the square
		bool is_blocker_robot = robot_positions->isRobotAt(moving_robot_row, col, {moving_robot_color});
		if (is_blocker_robot) {
			first_barrier_west = col;
			first_barrier_type = "robot";
			break;
		}

		// check if there is a diag barrier
		if (this->hasDiagBarrier(moving_robot_row, col)) {
			DiagBarrier diag = this->getDiagBarrier(moving_robot_row, col);

			// if the diag is same color, passes through
			// change above/below diag, in case there is a same color diag followed immediately by a barrier
			if (diag.getColor() == moving_robot_color) {
				moving_robot_above_diag = diag.isForward() ? true : false;
			}

			// if diag is of different color, it will deflect
			else {
				if (diag.isForward()) {
					// if diag is initially on a forward diag square, and is already below diag, nothing
					if (!(col == moving_robot_col && moving_robot_above_diag)) {
						first_barrier_west = col;
						first_barrier_type = "forward";
						moving_robot_above_diag = false;
						break;
					}
				}

				else if (diag.isBackward()) {

					// if robot is initially on back diag square, and is above that diag, nothing happens
					if (!(col == moving_robot_col && !moving_robot_above_diag)) {
						first_barrier_west = col;
						first_barrier_type = "backward";
						moving_robot_above_diag = true;
						break;
					}
				}
			}
		}

		// if neither a robot nor a diag, check if barrier
		if (this->hasRowBarrier(moving_robot_row, col)) {
			first_barrier_west = col;
			first_barrier_type = "barrier";
			break;
		}
	}

	// based on the type of the first barrier decide what to do
	if (first_barrier_type == "barrier") {
		// if barrier, stops
		Position new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "robot") {
		// if barrier or robot, moving robot stops
		Position new_position = Position(moving_robot_row, first_barrier_west + 1, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;

	} else if (first_barrier_type == "forward") {
		// if forward diag, deflects south
		Position new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->southMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "backward") {
		// if backward diag, deflects north
		Position new_position = Position(moving_robot_row, first_barrier_west, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->northMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	ASSERT(false, "invalid barrier type " << first_barrier_type);

}





bool Board::northMove(const string& moving_robot_color, RobotArrangement* robot_positions) const {
	
	Position initial_position = robot_positions->getPosition(moving_robot_color);
	int moving_robot_row = robot_positions->getRow(moving_robot_color);
	int moving_robot_col = robot_positions->getCol(moving_robot_color);
	bool moving_robot_above_diag = robot_positions->getAboveDiag(moving_robot_color);
	bool initial_moving_robot_above_diag = moving_robot_above_diag;

	// look for first barrier south, could be col barrier (horizontal), a diag barrier or another robot
	int first_barrier_north = 0;
	string first_barrier_type = "barrier";

	// search for that first barrier going east
	for (int row = moving_robot_row; row >= 0; row--) {

		// check if there is a robot on the square
		bool is_blocker_robot = robot_positions->isRobotAt(row, moving_robot_col, {moving_robot_color});
		if (is_blocker_robot) {
			first_barrier_north = row;
			first_barrier_type = "robot";
			break;
		}

		// check if there is a diag barrier
		if (this->hasDiagBarrier(row, moving_robot_col)) {
			DiagBarrier diag = this->getDiagBarrier(row, moving_robot_col);

			// if the diag is same color, passes through
			// change above/below diag, in case there is a same color diag followed immediately by a barrier
			if (diag.getColor() == moving_robot_color) {
				moving_robot_above_diag = diag.isForward() ? true : false;
			}

			// if diag is of different color, it will deflect
			else {
				if (diag.isForward()) {
					// if diag is initially on a forward diag square, and is already below diag, nothing
					if (!(row == moving_robot_row && moving_robot_above_diag)) {
						first_barrier_north = row;
						first_barrier_type = "forward";
						moving_robot_above_diag = false;
						break;
					}
				}

				else if (diag.isBackward()) {

					// if robot is initially on back diag square, and is above that diag, nothing happens
					if (!(row == moving_robot_row && moving_robot_above_diag)) {
						first_barrier_north = row;
						first_barrier_type = "backward";
						moving_robot_above_diag = false;
						break;
					}
				}
			}
		}

		// if neither a robot nor a diag, check if barrier
		if (this->hasColBarrier(moving_robot_col, row)) {
			first_barrier_north = row;
			first_barrier_type = "barrier";
			break;
		}

	}

	// based on the type of the first barrier decide what to do

	if (first_barrier_type == "barrier") {
		Position new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "robot") {
		Position new_position = Position(first_barrier_north + 1, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "forward") {
		// if forward diag, deflects east
		Position new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->eastMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "backward") {
		// if backward diag, deflects west
		Position new_position = Position(first_barrier_north, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->westMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	ASSERT(false, "invalid barrier type " << first_barrier_type);


}


bool Board::southMove(const string& moving_robot_color, RobotArrangement* robot_positions) const {
	
	Position initial_position = robot_positions->getPosition(moving_robot_color);
	int moving_robot_row = robot_positions->getRow(moving_robot_color);
	int moving_robot_col = robot_positions->getCol(moving_robot_color);
	bool moving_robot_above_diag = robot_positions->getAboveDiag(moving_robot_color);
	bool initial_moving_robot_above_diag = moving_robot_above_diag;

	// look for first barrier south, could be col barrier (horizontal), a diag barrier or another robot
	int first_barrier_south = BOARD_DIMENSION;
	string first_barrier_type = "barrier";


	// search for that first barrier going east
	for (int row = moving_robot_row; row < BOARD_DIMENSION; row++) {
		// check if there is a robot on the square
		bool is_blocker_robot = robot_positions->isRobotAt(row, moving_robot_col, {moving_robot_color});
		if (is_blocker_robot) {
			first_barrier_south = row;
			first_barrier_type = "robot";
			break;
		}

		// check if there is a diag barrier
		if (this->hasDiagBarrier(row, moving_robot_col)) {
			DiagBarrier diag = this->getDiagBarrier(row, moving_robot_col);

			// if the diag is same color, passes through
			// change above/below diag, in case there is a same color diag followed immediately by a barrier
			if (diag.getColor() == moving_robot_color) {
				moving_robot_above_diag = diag.isForward() ? false : true;
			}

			// if diag is of different color, it will deflect
			else {
				if (diag.isForward()) {
					// if diag is initially on a forward diag square, and is already below diag, nothing
					if (!(row == moving_robot_row && !moving_robot_above_diag)) {
						first_barrier_south = row;
						first_barrier_type = "forward";
						moving_robot_above_diag = true;
						break;
					}
				}

				else if (diag.isBackward()) {
					// if robot is initially on back diag square, and is below that diag, nothing happens
					if (!(row == moving_robot_row && !moving_robot_above_diag)) {
						first_barrier_south = row;
						first_barrier_type = "backward";
						moving_robot_above_diag = true;
						break;
					}
				}
			}
		}

		// if neither a robot nor a diag, check if barrier
		if (this->hasColBarrier(moving_robot_col, row + 1)) {
			first_barrier_south = row + 1;
			first_barrier_type = "barrier";
			break;
		}

	}

	// based on the type of the first barrier decide what to do
	if (first_barrier_type == "barrier" || first_barrier_type == "robot") {
		Position new_position = Position(first_barrier_south - 1, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "forward") {
		// if forward diag, deflects west
		Position new_position = Position(first_barrier_south, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->westMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	else if (first_barrier_type == "backward") {
		// if backward diag, deflects east
		Position new_position = Position(first_barrier_south, moving_robot_col, moving_robot_above_diag);
		robot_positions->setRobot(moving_robot_color, new_position);
		this->eastMove(moving_robot_color, robot_positions);
		new_position = robot_positions->getPosition(moving_robot_color);
		return initial_position != new_position;
	}

	ASSERT(false, "invalid barrier type " << first_barrier_type);

}






void buildBoard(Board* board, const Quadrant& quadrant1, const Quadrant& quadrant2,
	const Quadrant& quadrant3, const Quadrant& quadrant4) {

	// quadrant 1
	for (int row = 0; row <= QUADRANT_DIMENSION; row++) {
		for (int col = 0; col <= QUADRANT_DIMENSION; col++) {
			if (quadrant1.hasRowBarrier(row, col)) {
				board->setRowBarrier(row, col);
			}

			if (quadrant1.hasColBarrier(col, row)) {
				board->setColBarrier(col, row);
			}

			if (quadrant1.hasDiagBarrier(row, col)) {
				DiagBarrier db = quadrant1.getDiagBarrier(row, col);
				board->setDiagBarrier(row, col, db.getColor(), db.isForward());
			}
		}
	}
	


	// quadrant 2
	for (int row = 0; row <= QUADRANT_DIMENSION; row++) {
		for (int col = 0; col <= QUADRANT_DIMENSION; col++) {
			if (quadrant2.hasRowBarrier(row, col)) {
				board->setColBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - row), col);
			}

			if (quadrant2.hasColBarrier(col, row)) {
				board->setRowBarrier(col, QUADRANT_DIMENSION + (QUADRANT_DIMENSION - row));
			}

			if (quadrant2.hasDiagBarrier(row, col)) {
				DiagBarrier db = quadrant1.getDiagBarrier(row, col);
				board->setDiagBarrier(col, (QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - row), db.getColor(), !db.isForward());
			}
		}
	}

	// quadrant 3
	for (int row = 0; row <= QUADRANT_DIMENSION; row++) {
		for (int col = 0; col <= QUADRANT_DIMENSION; col++) {
			if (quadrant3.hasRowBarrier(row, col)) {
				board->setColBarrier(row, QUADRANT_DIMENSION + (QUADRANT_DIMENSION - col));
			}

			if (quadrant3.hasColBarrier(col, row)) {
				board->setRowBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - col), row);
			}

			if (quadrant3.hasDiagBarrier(row, col)) {
				DiagBarrier db = quadrant3.getDiagBarrier(row, col);
				board->setDiagBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - col), row, db.getColor(), !db.isForward());
			}
		}
	}

	// quadrant 4
	for (int row = 0; row <= QUADRANT_DIMENSION; row++) {
		for (int col = 0; col <= QUADRANT_DIMENSION; col++) {
			if (quadrant4.hasRowBarrier(row, col)) {
				board->setRowBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - row), 
					QUADRANT_DIMENSION + (QUADRANT_DIMENSION - col));
			}

			if (quadrant4.hasColBarrier(col, row)) {
				board->setColBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - col), 
					QUADRANT_DIMENSION + (QUADRANT_DIMENSION - row));
			}

			if (quadrant4.hasDiagBarrier(row, col)) {
				DiagBarrier db = quadrant4.getDiagBarrier(row, col);
				board->setDiagBarrier((QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - row),
					(QUADRANT_DIMENSION - 1) + (QUADRANT_DIMENSION - row), db.getColor(), db.isForward());
			}
		}
	}
}







void createBoardTemp(Board* board) {

	board->setRowBarrier(0, 5); board->setRowBarrier(0, 10);
	board->setRowBarrier(1, 13);
	board->setRowBarrier(2, 6); board->setRowBarrier(2, 10);
	board->setRowBarrier(3, 3); board->setRowBarrier(3, 9);

	//
	board->setRowBarrier(5, 2);
	board->setRowBarrier(6, 11);
	board->setRowBarrier(7, 6); board->setRowBarrier(7, 7); board->setRowBarrier(7, 9);

	board->setRowBarrier(8, 7); board->setRowBarrier(8, 9);
	board->setRowBarrier(9, 2); board->setRowBarrier(9, 11);
	board->setRowBarrier(10, 3);
	board->setRowBarrier(11, 7); board->setRowBarrier(11, 13);

	//
	board->setRowBarrier(13, 9);
	board->setRowBarrier(14, 6);
	board->setRowBarrier(15, 7); board->setRowBarrier(15, 11);




	board->setColBarrier(0, 7); board->setColBarrier(0, 13);
	board->setColBarrier(1, 6);
	board->setColBarrier(2, 3); board->setColBarrier(2, 10);
	board->setColBarrier(3, 4);

	//
	board->setColBarrier(5, 8); board->setColBarrier(5, 15);
	board->setColBarrier(6, 2);
	board->setColBarrier(7, 7);	board->setColBarrier(7, 9);	board->setColBarrier(7, 11);

	board->setColBarrier(8, 7); board->setColBarrier(8, 9);
	board->setColBarrier(9, 3); board->setColBarrier(9, 13);
	board->setColBarrier(10, 10);
	board->setColBarrier(11, 7);

	board->setColBarrier(12, 1); board->setColBarrier(12, 11);
	board->setColBarrier(13, 12);
	//
	board->setColBarrier(15, 6); board->setColBarrier(15, 13);


	//
	board->setDiagBarrier(1, 2, "red", false);
	//
	//

	board->setDiagBarrier(4, 14, "green", false);
	board->setDiagBarrier(5, 8, "yellow", true);
	board->setDiagBarrier(6, 3, "green", false);
	//

	board->setDiagBarrier(8, 4, "red", true);
	//
	//
	//

	board->setDiagBarrier(12, 9, "yellow", false);
	board->setDiagBarrier(13, 1, "blue", false);
	board->setDiagBarrier(14, 11, "blue", false);
	//


}



void testEastMove() {
	Board board;
	createBoardTemp(&board);

	Position yellow_robot(9, 3);
	Position red_robot(5, 2);
	Position green_robot(9, 2);
	Position blue_robot(1, 0);

	RobotArrangement robot_positions({{"yellow", yellow_robot}, {"red", red_robot}, {"green", green_robot}, {"blue", blue_robot}});

	Position dest(11, 13);
	string dest_color = "green";


	RobotArrangement new_robot_positions(robot_positions);
	bool yellow_moved = board.eastMove("yellow", &new_robot_positions);
	bool red_moved = board.eastMove("red", &new_robot_positions);
	bool green_moved = board.eastMove("green", &new_robot_positions);
	bool blue_moved = board.eastMove("blue", &new_robot_positions);
	ASSERT(blue_moved && yellow_moved && red_moved && green_moved, "Only blue and yellow and red move east");

	Position new_yellow(9, 10);
	Position new_red(0, 8);
	Position new_green(9, 9);
	Position new_blue(2, 2);
	ASSERT(sameSquare(new_robot_positions.getPosition("yellow"), new_yellow), "yellow wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("red"), new_red), "red wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("green"), new_green), "green wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("blue"), new_blue), "blue wrong");
}

void testWestMove() {
	Board board;
	createBoardTemp(&board);

	Position yellow_robot(6, 10); // hits green
	Position red_robot(8, 6); // goes thru
	Position green_robot(5, 15); // hits forward diag
	Position blue_robot(1, 12); // hits back diag

	RobotArrangement robot_positions({{"yellow", yellow_robot}, {"red", red_robot}, {"green", green_robot}, {"blue", blue_robot}});

	Position dest(11, 13);
	string dest_color = "green";


	RobotArrangement new_robot_positions(robot_positions);
	bool red_moved = board.westMove("red", &new_robot_positions);
	bool green_moved = board.westMove("green", &new_robot_positions);
	bool blue_moved = board.westMove("blue", &new_robot_positions);
	bool yellow_moved = board.westMove("yellow", &new_robot_positions);
	ASSERT(blue_moved && yellow_moved && red_moved && green_moved, "All move west");

	Position new_yellow(6, 9);
	Position new_red(8, 0);
	Position new_green(6, 8);
	Position new_blue(0, 2);
	ASSERT(sameSquare(new_robot_positions.getPosition("yellow"), new_yellow), "yellow wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("red"), new_red), "red wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("green"), new_green), "green wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("blue"), new_blue), "blue wrong");
}



void testNortMove() {
	Board board;
	createBoardTemp(&board);

	Position yellow_robot(6, 8); //goes thru diag
	Position red_robot(15, 14); // hits back diag
	Position green_robot(15, 7); // hits barrier
	Position blue_robot(6, 0); // hits red robot

	RobotArrangement robot_positions({{"yellow", yellow_robot}, {"red", red_robot}, {"green", green_robot}, {"blue", blue_robot}});

	Position dest(11, 13);
	string dest_color = "green";


	RobotArrangement new_robot_positions(robot_positions);
	bool yellow_moved = board.northMove("yellow", &new_robot_positions);
	bool red_moved = board.northMove("red", &new_robot_positions);
	bool green_moved = board.northMove("green", &new_robot_positions);
	bool blue_moved = board.northMove("blue", &new_robot_positions);
	ASSERT(blue_moved && yellow_moved && red_moved && green_moved, "All move west");

	Position new_yellow(0, 8);
	Position new_red(4, 0);
	Position new_green(11, 7);
	Position new_blue(5, 0);
	ASSERT(sameSquare(new_robot_positions.getPosition("yellow"), new_yellow), "yellow wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("red"), new_red), "red wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("green"), new_green), "green wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("blue"), new_blue), "blue wrong");
}


void testSouthMove() {
	Board board;
	createBoardTemp(&board);

	Position yellow_robot(2, 6); //goes thru diag
	Position red_robot(0, 8); // hits back diag
	Position green_robot(7, 11); // hits barrier
	Position blue_robot(8, 6); // hits red robot

	RobotArrangement robot_positions({{"yellow", yellow_robot}, {"red", red_robot}, {"green", green_robot}, {"blue", blue_robot}});

	Position dest(11, 13);
	string dest_color = "green";


	RobotArrangement new_robot_positions(robot_positions);
	bool yellow_moved = board.southMove("yellow", &new_robot_positions);
	bool red_moved = board.southMove("red", &new_robot_positions);
	bool green_moved = board.southMove("green", &new_robot_positions);
	bool blue_moved = board.southMove("blue", &new_robot_positions);
	ASSERT(blue_moved && yellow_moved && red_moved && green_moved, "All move west");

	Position new_yellow(7, 6);
	Position new_red(5, 2);
	Position new_green(14, 15);
	Position new_blue(15, 6);
	ASSERT(sameSquare(new_robot_positions.getPosition("yellow"), new_yellow), "yellow wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("red"), new_red), "red wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("green"), new_green), "green wrong");
	ASSERT(sameSquare(new_robot_positions.getPosition("blue"), new_blue), "blue wrong");
}

int main() {

	Board board;
	buildBoard(&board, quadrant1, quadrant3, quadrant5, quadrant7);

	Position yellow_robot(5, 7);
	Position red_robot(13, 5);
	Position green_robot(2, 6);
	Position blue_robot(10, 9);

	Position dest(14, 9);

	RobotArrangement robot_positions({{"yellow", yellow_robot}, {"red", red_robot}, {"green", green_robot}, {"blue", blue_robot}});

	board.display(robot_positions, dest);
	string dest_color {"yellow"};


	vector<Move> moves;
	int solution_length = solveRicochetBoard(board, robot_positions, dest, dest_color, &moves);
	bool solved = solution_length != -1;
	if (solved) {
		log("Solution takes " + to_string(solution_length) + " moves");
		printMoves(moves);
	} else {
		log("no solution found");
	}
	



	//vector<Move> solution;
	//bool is_solution = solveDepthLimitedDfs(board, robot_positions, dest, dest_color, &solution);

}