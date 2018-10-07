#ifndef QUADRANT_H
#define QUADRANT_H

#include "board.h"
#include "robots.h"

using namespace std;

class Quadrant;

extern int QUADRANT_DIMENSION;

extern Quadrant quadrant1;
extern Quadrant quadrant2;
extern Quadrant quadrant3;
extern Quadrant quadrant4;

extern Quadrant quadrant5;
extern Quadrant quadrant6;
extern Quadrant quadrant7;
extern Quadrant quadrant8;

extern Quadrant quadrant9;
extern Quadrant quadrant10;
extern Quadrant quadrant11;
extern Quadrant quadrant12;

extern Quadrant quadrant13;
extern Quadrant quadrant14;
extern Quadrant quadrant15;
extern Quadrant quadrant16;

class Quadrant : public Board {

	vector<vector<int>> row_barriers; // row_barriers[i] contains the col indices of each row barrier (vertical) in row i
	vector<vector<int>> col_barriers; // col_barriers[j] contains the row indices of each col barrier (horizontal) in row j
	vector<vector<pair<int, DiagBarrier>>> diag_barriers; // diag_barriers[i][j] points to the diag barrier at position (i, j)

public:

	// only sets default barriers on two edges
	Quadrant(int quadrant_num, int quadrant_dimension=QUADRANT_DIMENSION);
};




void createQuadrant1(Quadrant* quad);
void createQuadrant2(Quadrant* quad);
void createQuadrant3(Quadrant* quad);
void createQuadrant4(Quadrant* quad);

void createQuadrant5(Quadrant* quad);
void createQuadrant6(Quadrant* quad);
void createQuadrant7(Quadrant* quad);
void createQuadrant8(Quadrant* quad);

void createQuadrant9(Quadrant* quad);
void createQuadrant10(Quadrant* quad);
void createQuadrant11(Quadrant* quad);
void createQuadrant12(Quadrant* quad);

void createQuadrant13(Quadrant* quad);
void createQuadrant14(Quadrant* quad);
void createQuadrant15(Quadrant* quad);
void createQuadrant16(Quadrant* quad);

#endif
