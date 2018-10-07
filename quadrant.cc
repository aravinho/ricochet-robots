#include "quadrant.h"

using namespace std;

int QUADRANT_DIMENSION = 8;

Quadrant::Quadrant(int quadrant_num, int quadrant_dimension) {

	for (int row = 0; row < quadrant_dimension; row++) {
		// set edge of board barriers
		vector<int> rb_indices;
		this->row_barriers.push_back(rb_indices);
		insertInOrder(&(this->row_barriers[row]), 0);

		// initialize diag list
		vector<pair<int, DiagBarrier>> db_indices;
		this->diag_barriers.push_back(db_indices);
	}

	for (int col = 0; col < quadrant_dimension; col++) {
		// set edge of board barriers
		vector<int> cb_indices;
		this->col_barriers.push_back(cb_indices);
		insertInOrder(&(this->col_barriers[col]), 0);
	}

	switch (quadrant_num) {
		case 1: createQuadrant1(this); break;
		case 2: createQuadrant2(this); break;
		case 3: createQuadrant3(this); break;
		case 4: createQuadrant4(this); break;
		case 5: createQuadrant5(this); break;
		case 6: createQuadrant6(this); break;
		case 7: createQuadrant7(this); break;
		case 8: createQuadrant8(this); break;
		default: break;
	}
}




void createQuadrant1(Quadrant* quad) {

	quad->setRowBarrier(0, 4);
	quad->setRowBarrier(1, 1);
	quad->setRowBarrier(2, 7);
	quad->setRowBarrier(4, 3);
	quad->setRowBarrier(5, 7);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 6);
	quad->setColBarrier(1, 2);
	quad->setColBarrier(2, 5);
	quad->setColBarrier(6, 2);
	quad->setColBarrier(7, 5);
	quad->setColBarrier(7, 7);

}

void createQuadrant2(Quadrant* quad) {

	quad->setRowBarrier(0, 4);
	quad->setRowBarrier(2, 6);
	quad->setRowBarrier(4, 3);
	quad->setRowBarrier(5, 7);
	quad->setRowBarrier(6, 1);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 5);
	quad->setColBarrier(1, 6);
	quad->setColBarrier(2, 4);
	quad->setColBarrier(5, 3);
	quad->setColBarrier(7, 6);
	quad->setColBarrier(7, 7);

}

void createQuadrant3(Quadrant* quad) {

	quad->setRowBarrier(0, 5);
	quad->setRowBarrier(1, 2);
	quad->setRowBarrier(3, 6);
	quad->setRowBarrier(5, 5);
	quad->setRowBarrier(6, 2);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 5);
	quad->setColBarrier(1, 7);
	quad->setColBarrier(2, 1);
	quad->setColBarrier(4, 5);
	quad->setColBarrier(6, 4);
	quad->setColBarrier(7, 7);

}

void createQuadrant4(Quadrant* quad) {

	quad->setRowBarrier(0, 4);
	quad->setRowBarrier(1, 6);
	quad->setRowBarrier(2, 1);
	quad->setRowBarrier(4, 6);
	quad->setRowBarrier(6, 3);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 4);
	quad->setColBarrier(1, 3);
	quad->setColBarrier(2, 6);
	quad->setColBarrier(5, 2);
	quad->setColBarrier(6, 4);
	quad->setColBarrier(7, 7);

}

void createQuadrant5(Quadrant* quad) {

	quad->setRowBarrier(0, 4);
	quad->setRowBarrier(1, 6);
	quad->setRowBarrier(3, 2);
	quad->setRowBarrier(4, 5);
	quad->setRowBarrier(5, 3);
	quad->setRowBarrier(5, 8);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 7);
	quad->setColBarrier(1, 3);
	quad->setColBarrier(2, 6);
	quad->setColBarrier(5, 4);
	quad->setColBarrier(6, 2);
	quad->setColBarrier(7, 6);
	quad->setColBarrier(7, 7);

}

void createQuadrant6(Quadrant* quad) {

	quad->setRowBarrier(0, 5);
	quad->setRowBarrier(1, 3);
	quad->setRowBarrier(3, 1);
	quad->setRowBarrier(4, 6);
	quad->setRowBarrier(6, 6);
	quad->setRowBarrier(7, 4);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 5);
	quad->setColBarrier(1, 4);
	quad->setColBarrier(2, 2);
	quad->setColBarrier(3, 8);
	quad->setColBarrier(5, 6);
	quad->setColBarrier(6, 4);
	quad->setColBarrier(7, 7);

}


void createQuadrant7(Quadrant* quad) {

	quad->setRowBarrier(0, 5);
	quad->setRowBarrier(1, 7);
	quad->setRowBarrier(2, 1);
	quad->setRowBarrier(5, 7);
	quad->setRowBarrier(6, 3);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 6);
	quad->setColBarrier(1, 2);
	quad->setColBarrier(3, 7);
	quad->setColBarrier(6, 2);
	quad->setColBarrier(6, 5);
	quad->setColBarrier(7, 7);

}

void createQuadrant8(Quadrant* quad) {

	quad->setRowBarrier(0, 2);
	quad->setRowBarrier(1, 4);
	quad->setRowBarrier(2, 2);
	quad->setRowBarrier(3, 7);
	quad->setRowBarrier(6, 3);
	quad->setRowBarrier(7, 7);

	quad->setColBarrier(0, 6);
	quad->setColBarrier(1, 2);
	quad->setColBarrier(3, 7);
	quad->setColBarrier(4, 1);
	quad->setColBarrier(6, 4);
	quad->setColBarrier(7, 7);

}





void createQuadrant9(Quadrant* quad) {

	quad->setRowBarrier(0, 5);

	quad->setRowBarrier(2, 6);
	quad->setRowBarrier(3, 3);

	quad->setRowBarrier(5, 2);

	quad->setRowBarrier(7, 6); quad->setRowBarrier(7, 7);

	

	quad->setColBarrier(0, 7);
	quad->setColBarrier(1, 6);
	quad->setColBarrier(2, 3);
	quad->setColBarrier(3, 4);


	quad->setColBarrier(6, 2);
	quad->setColBarrier(7, 7);

	

	quad->setDiagBarrier(1, 2, "red", false);
	quad->setDiagBarrier(6, 3, "green", false);

}




Quadrant quadrant1(1);
Quadrant quadrant2(2);
Quadrant quadrant3(3);
Quadrant quadrant4(4);

Quadrant quadrant5(5);
Quadrant quadrant6(6);
Quadrant quadrant7(7);
Quadrant quadrant8(8);

Quadrant quadrant9(9);
Quadrant quadrant10(10);
Quadrant quadrant11(11);
Quadrant quadrant12(12);

Quadrant quadrant13(13);
Quadrant quadrant14(14);
Quadrant quadrant15(15);
Quadrant quadrant16(16);
