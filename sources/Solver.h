#pragma once
#include <stdio.h>
#include <iostream>
#include "Constants.h"

class Solver
{
private:
    static const int MAXSOLUTIONS = 2;
	int grid[SIZE][SIZE];
	int solutions;

	int	SearchSolution(int i, int j);
    void ResetGrid();
    int	CheckValueInGrid(int i, int j, int value);

	bool* sudokuEntityState();
	bool* sudokuEntityState(bool* state);

public:
	Solver();
	~Solver();

    int	Solutions();
	bool ValidState();
    int SetGrid(int grid[SIZE][SIZE]);
};

