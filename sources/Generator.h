#ifndef GENERATOR_H
#define GENERATOR_H

#include <time.h>
#include <vector>
#include <iostream>

#include "Constants.h"
#include "Solver.h"

using namespace std;

typedef pair<int, int>	PInt;
typedef vector<int>		VInt;
typedef vector<VInt>	VVInt;
typedef vector<PInt>	VPInt;

class Generator
{
private:
	int fillingField[SIZE][SIZE];
	bool mask[SIZE][SIZE];
	int grid[SIZE][SIZE];
    int valueMatrix[SIZE][SIZE];
    DIFFICULTY difficulty;

    void GenerateMask();
	void GenerateField();
	void ApplyMask();
    void FindSuitableMask();
    bool CheckGridSolvability();
	void FillStringRandom(int stringNumber);
	void UpdateMatrix(int stringNumber);
    VPInt Hungarian(const VVInt &matrix);
	void FillSubRowOfValueMatrix(int i, int j, int value);
    bool isCorrectly();
    void FlushFields();

public:
    Generator();
    Generator(DIFFICULTY);
	const int GetValueInCell(int i, int j);
    const int GetValueInFillingField(int i, int j);
    void Generate();
};

#endif // GENERATOR_H
