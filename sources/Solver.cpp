#include "Solver.h"

Solver::Solver()
{
}

Solver::~Solver()
{
}

void Solver::ResetGrid()
{
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			grid[i][j] = 0;
		}
}

int	Solver::CheckValueInGrid(int i, int j, int value)
{
	int k, m;
	//check in row
	for (k = 0; k<SIZE; k++)
	{
		if (k != j)
        if (grid[i][k] == value)
			return 0;
	}
	// check in column
	for (m = 0; m < SIZE; m++)
	{
		if (m != i)
        if (grid[m][j] == value)
			return 0;
	}
	//check in subgrid
	for (k = (i / (int)sqrt(SIZE)) * (int)sqrt(SIZE); k<(i / (int)sqrt(SIZE)) * (int)sqrt(SIZE) + (int)sqrt(SIZE); k++)
	for (m = (j / (int)sqrt(SIZE)) * (int)sqrt(SIZE); m<(j / (int)sqrt(SIZE)) * (int)sqrt(SIZE) + (int)sqrt(SIZE); m++)
	{
		if ((k != i) || (m != j))
        if (grid[k][m] == value)
			return 0;
	}
	return 1;
}

int	Solver::SearchSolution(int i, int j)
{
	int value;
	if (j == SIZE)
	{
		j = 0;
		i++;
	}
	if (i == SIZE)
	{ //solution
        solutions++;
        return solutions >= MAXSOLUTIONS;
	}

	if (grid[i][j] != 0)
	{
		if (SearchSolution(i, j + 1))
			return 1;
	}
	else
	{
		for (value = 1; value < 10; value++) /* Try possible values. */
		if (CheckValueInGrid(i, j, value))
		{
			grid[i][j] = value; //new test value
			if (SearchSolution(i, j + 1))
				return 1;
		}
		grid[i][j] = 0; //unrecord try
	}
	return 0;
}

int Solver::Solutions()
{
    solutions = 0;
    SearchSolution(0, 0);
    return solutions;
}

int Solver::SetGrid(int grid[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			(*this).grid[i][j] = grid[i][j];
	return 0;
}

bool Solver::ValidState()
{
	bool* state = sudokuEntityState();

	// Rows
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			if (grid[i][j] != 0)
				if (state[grid[i][j] - 1]) return false;
				else state[grid[i][j] - 1] = true;
				state = sudokuEntityState(state);
	}

	// Cols
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 9; i++)
			if (grid[i][j] != 0)
				if (state[grid[i][j] - 1]) return false;
				else state[grid[i][j] - 1] = true;
				state = sudokuEntityState(state);
	}

	// Squares
	for (int sq_i = 0; sq_i < 9; sq_i += 3)
		for (int sq_j = 0; sq_j < 9; sq_j += 3)
		{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (grid[sq_i + i][sq_j + j] != 0)
					if (state[grid[sq_i + i][sq_j + j] - 1]) return false;
					else state[grid[sq_i + i][sq_j + j] - 1] = true;
					state = sudokuEntityState(state);
		}
	delete[] state;
	return true;
}


bool* Solver::sudokuEntityState()
{
	bool* b = new bool[9];
	for (int i = 0; i < 9; i++)
		b[i] = false;
	return b;
}


bool* Solver::sudokuEntityState(bool* b)
{
	for (int i = 0; i < 9; i++)
		b[i] = false;
	return b;
}