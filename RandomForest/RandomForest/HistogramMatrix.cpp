#include "HistogramMatrix.h"

/*
 * A class to abstract all of the ugly vector stuff that is happening
 */
HistogramMatrix::HistogramMatrix(int _width, int _height) {
	width = _width;
	height = _height;
	matrix = vector<vector<vector<double>>>(height);
	for(int j = 0; j < height; j++) {
			vector<vector<double>> row = vector<vector<double>>(width);

			matrix.at(j) = row;
	}
}

HistogramMatrix::HistogramMatrix(int _width, int _height, int numClasses) {
	width = _width;
	height = _height;
	matrix = vector<vector<vector<double>>>(height);
	for(int j = 0; j < height; j++) {
			vector<vector<double>> row = vector<vector<double>>(width);

			for(int k=0; k < width; k++) {
				row.at(k) = vector<double>(numClasses, 0);
			}


			matrix.at(j) = row;
	}
}

void HistogramMatrix::addHistogram(int row, int col, vector<double> hist) {
	matrix.at(row).at(col) = hist;
	/*for(int i=0; i < hist.size(); i++) {
		matrix.at(row).at(col).at(i) += hist.at(i);
	}*/
}

vector<double> HistogramMatrix::getHistogram(int row, int col) {
	return matrix.at(row).at(col);
}

HistogramMatrix::~HistogramMatrix(void) {

}