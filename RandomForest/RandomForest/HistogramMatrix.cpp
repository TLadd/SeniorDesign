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

void HistogramMatrix::addHistogram(int row, int col, vector<double> hist) {
	matrix.at(row).at(col) = hist;
}

vector<double> HistogramMatrix::getHistogram(int row, int col) {
	return matrix.at(row).at(col);
}

HistogramMatrix::~HistogramMatrix(void) {

}