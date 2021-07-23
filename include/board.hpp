#pragma once
#include <vector>
#include <functional>

class Board {
    public:
        Board(int rows, int cols);
        void changeCellState(int row, int col);
        bool isCellOccupied(int row, int col);
        void iterate();
    
    private:
        std::vector<std::vector<bool>> state;
        int boardRows, boardCols;
        int getNeighborsCount(int row, int col);
};
