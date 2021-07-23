#include "board.hpp"

Board::Board(int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		std::vector<bool> row;
		row.assign(cols, false);
		this->state.push_back(row);
	}

	this->boardRows = rows;
	this->boardCols = cols;
}

bool Board::isCellOccupied(int row, int col) {
	return this->state.at(row).at(col);
}

void Board::changeCellState(int row, int col) {
	this->state.at(row).at(col) = !isCellOccupied(row, col);
}

int Board::getNeighborsCount(int row, int col) {
	int neighbors = 0;

	for (int i = std::max(0, row - 1); i <= std::min(this->boardRows - 1, row + 1); i++) {
		for (int j = std::max(0, col - 1); j <= std::min(this->boardCols - 1, col + 1); j++) {
			if (i == row && j == col) continue;
			if (this->isCellOccupied(i, j)) neighbors++;
		}
	}

	return neighbors;
}

void Board::iterate() {
	std::vector<std::pair<int, int>> toBeUpdated;

	for (int row = 0; row < this->boardRows; row++) {
		for (int col = 0; col < this->boardCols; col++) {
			int neighbors = this->getNeighborsCount(row, col);

			if (this->isCellOccupied(row, col)) {
				if (neighbors < 2 || neighbors > 3)
					toBeUpdated.push_back(std::make_pair(row, col));
			} else {
				if (neighbors == 3)
					toBeUpdated.push_back(std::make_pair(row, col));
			}
		}
	}

	for (std::pair<int, int> cell : toBeUpdated) {
		this->changeCellState(cell.first, cell.second);
	}
}
