#ifndef ROBOTS_H
#define ROBOTS_H

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <set>

using namespace std;


/* ASSERT Macro. */

#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

extern int NUM_ROBOTS;


typedef int Move;
extern Move YELLOW_NORTH, RED_NORTH, GREEN_NORTH, BLUE_NORTH;
extern Move YELLOW_SOUTH, RED_SOUTH, GREEN_SOUTH, BLUE_SOUTH;
extern Move YELLOW_EAST, RED_EAST, GREEN_EAST, BLUE_EAST;
extern Move YELLOW_WEST, RED_WEST, GREEN_WEST, BLUE_WEST;

extern vector<Move> all_moves;
string getColor(Move move);
string getDirection(Move move);
void printMoves(const vector<Move>& moves);


class Position {

	int row;
	int col;
	bool above_diag;

public:

	Position(int row, int col, int above_diag=true);

	int getRow() const;
	int getCol() const;
	bool getAboveDiag() const;
	friend ostream& operator<<(ostream& os, const Position& dt);  
	bool operator==(const Position& other);
	bool operator!=(const Position& other);

};

bool sameSquare(const Position& a, const Position& b);
ostream& operator<<(ostream& os, const Position& pos);


class RobotArrangement {

	map<string, Position> robot_positions;

public:

	RobotArrangement(const map<string, Position>& robot_positions) : robot_positions(robot_positions) {}
	RobotArrangement(const RobotArrangement& orig);

	void setRobot(const string& color, const Position& position);

	int getRow(const string& color) const;
	int getCol(const string& color) const;
	bool getAboveDiag(const string& color) const;
	Position getPosition(const string& color) const;

	// returns whether there is a robot (not in exclude) at the given position
	bool isRobotAt(int row, int col, set<string> exclude = set<string>()) const ;

	// returns whether this robot arrangement satisfies the solution
	bool isSolution(const Position& dest, const string& dest_color) const;


};

typedef long RobotArrangementEncoding;
RobotArrangementEncoding encode(const RobotArrangement& robot_positions);


#endif