#include "robots.h"
#include "board.h"

#include <iostream>
#include <cmath>

using namespace std;


int NUM_ROBOTS = 4;


Move YELLOW_NORTH = 0, RED_NORTH = 1, GREEN_NORTH = 2, BLUE_NORTH = 3;
Move YELLOW_SOUTH = 4, RED_SOUTH = 5, GREEN_SOUTH = 6, BLUE_SOUTH = 7;
Move YELLOW_EAST = 8, RED_EAST = 9, GREEN_EAST = 10, BLUE_EAST = 11;
Move YELLOW_WEST =12, RED_WEST = 13, GREEN_WEST = 14, BLUE_WEST = 15;

vector<Move> all_moves = 
	{
		YELLOW_NORTH, YELLOW_SOUTH, YELLOW_EAST, YELLOW_WEST,
		RED_NORTH, RED_SOUTH, RED_EAST, RED_WEST,
		GREEN_NORTH, GREEN_SOUTH, GREEN_EAST, GREEN_WEST,
		BLUE_NORTH, BLUE_SOUTH, BLUE_EAST, BLUE_WEST
	};

string getColor(Move move) {
	ASSERT(0 <= move && move < 16, "Move out of range");
	if (move % NUM_ROBOTS == 0) {
		return "yellow";
	} 
	if (move % NUM_ROBOTS == 1) {
		return "red";
	} 
	if (move % NUM_ROBOTS == 2) {
		return "green";
	} 
	if (move % NUM_ROBOTS == 3) {
		return "blue";
	} 
	ASSERT(false, "error in getColor");
}

string getDirection(Move move) {
	ASSERT(0 <= move && move < 16, "Move out of range");
	if (move / NUM_ROBOTS == 0) {
		return "north";
	} 
	if (move / NUM_ROBOTS == 1) {
		return "south";
	} 
	if (move / NUM_ROBOTS == 2) {
		return "east";
	} 
	if (move / NUM_ROBOTS == 3) {
		return "west";
	} 
	ASSERT(false, "error in getDirection");
}

void printMoves(const vector<Move>& moves) {
	for (Move move : moves) {
		cout << getColor(move) << ", " << getDirection(move) << endl;
	}
}


Position::Position(int row, int col, int above_diag) {
	this->row = row;
	this->col = col;
	this->above_diag = above_diag;
}

int Position::getRow() const {
	return this->row;
}

int Position::getCol() const {
	return this->col;
}

bool Position::getAboveDiag() const {
	return this->above_diag;
}

ostream& operator<<(ostream& os, const Position& pos)  
{  
    os << "(" << pos.row << ',' << pos.col << ',' << pos.above_diag << ")";
    return os;  
}  

bool Position::operator==(const Position& other) {
	return this->getRow() == other.getRow() && this->getCol() == other.getCol() && this->getAboveDiag() == other.getAboveDiag();
}

bool Position::operator!=(const Position& other) {
	return !(*this == other);
}

bool sameSquare(const Position& a, const Position& b) {
	return a.getRow() == b.getRow() && a.getCol() == b.getCol();
}





RobotArrangement::RobotArrangement(const RobotArrangement& orig) {
	this->robot_positions = orig.robot_positions;
}


void RobotArrangement::setRobot(const string& color, const Position& position) {
	if (this->robot_positions.count(color) > 0) {
		this->robot_positions.at(color) = position;
	} else {
		this->robot_positions.insert(make_pair(color, position));
	}
}

int RobotArrangement::getRow(const string& color) const {
	ASSERT(this->robot_positions.count(color) > 0, "Color " << color << " not present");
	return this->robot_positions.at(color).getRow();
}

int RobotArrangement::getCol(const string& color) const {
	ASSERT(this->robot_positions.count(color) > 0, "Color " << color << " not present");
	return this->robot_positions.at(color).getCol();
}

bool RobotArrangement::getAboveDiag(const string& color) const {
	ASSERT(this->robot_positions.count(color) > 0, "Color " << color << " not present");
	return this->robot_positions.at(color).getAboveDiag();
}

Position RobotArrangement::getPosition(const string& color) const {
	ASSERT(this->robot_positions.count(color) > 0, "Color " << color << " not present");
	return this->robot_positions.at(color);
}


bool RobotArrangement::isRobotAt(int row, int col, set<string> exclude) const {
	for (auto robot : this->robot_positions) {
		string color = robot.first;
		Position position = robot.second;
		if (exclude.count(color) == 0 && position.getRow() == row && position.getCol() == col) {
			return true;
		}
	}

	return false;
}

bool RobotArrangement::isSolution(const Position& dest, const string& dest_color) const {
	ASSERT(this->robot_positions.count(dest_color) > 0, "Color " << dest_color << " not found");
	Position dest_robot = this->robot_positions.at(dest_color);
	return dest_robot.getRow() == dest.getRow() && dest_robot.getCol() == dest.getCol();
}



RobotArrangementEncoding encode(const RobotArrangement& robot_positions) {

	long yellow_pos = robot_positions.getRow("yellow") * BOARD_DIMENSION + robot_positions.getCol("yellow");
	long red_pos = robot_positions.getRow("red") * BOARD_DIMENSION + robot_positions.getCol("red");
	long green_pos = robot_positions.getRow("green") * BOARD_DIMENSION + robot_positions.getCol("green");
	long blue_pos = robot_positions.getRow("blue") * BOARD_DIMENSION + robot_positions.getCol("blue");

	vector<long> position_encodings = {yellow_pos, red_pos, green_pos, blue_pos};
	long position_encoding = 0;

	for (int i = 0; i < NUM_ROBOTS; i++) {
		position_encoding += pow(BOARD_DIMENSION, 2*i) * position_encodings[i];
	}
	
	long yellow_above_diag = robot_positions.getAboveDiag("yellow") ? 1 : 0;
	long red_above_diag = robot_positions.getAboveDiag("red") ? 1 : 0;
	long green_above_diag = robot_positions.getAboveDiag("green") ? 1 : 0;
	long blue_above_diag = robot_positions.getAboveDiag("blue") ? 1 : 0;

	vector<long> above_diag_encodings = {yellow_above_diag, red_above_diag, green_above_diag, blue_above_diag};
	long final_encoding = pow(2, NUM_ROBOTS) * position_encoding;

	for (int i = 0; i < NUM_ROBOTS; i++) {
		final_encoding += pow(2, i) * above_diag_encodings[i];
	}

	return final_encoding;

}



