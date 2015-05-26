#include "generator.h"

Generator::Generator()
{
    this->difficulty = NORMAL;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            valueMatrix[i][j] = 0;
    Generate();
}

Generator::Generator(DIFFICULTY difficulty = NORMAL)
{
    this->difficulty = difficulty;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            valueMatrix[i][j] = 0;
    do
    {
        Generate();
    } while(!isCorrectly());
}

void Generator::GenerateMask()
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            mask[i][j] = 1;
    int i, j;
    srand(time(NULL));
    for(int count = 0; count < (int)difficulty; count++)
    {
        i = rand() % 9;
        j = rand() % 9;
        if(mask[i][j])
            mask[i][j] = 0;
        else count--;
    }
	return;
}

void Generator::GenerateField()
{
	FillStringRandom(0);
	UpdateMatrix(0);
	VVInt requestVector(SIZE, VInt(SIZE));
	VPInt resultVector(SIZE);
	for (int i = 1; i < SIZE; i++)
	{
		for (int x = 0; x < SIZE; x++)
		for (int y = 0; y < SIZE; y++)
			requestVector[x][y] = valueMatrix[x][y];
        resultVector = Hungarian(requestVector);
		for (int j = 0; j < SIZE; j++)
			fillingField[i][j] = resultVector[j].first + 1;
		UpdateMatrix(i);
	}
}

void Generator::ApplyMask()
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			if (mask[i][j] == 1)
				grid[i][j] = fillingField[i][j];
			else
				grid[i][j] = 0;
		}
}

void Generator::FindSuitableMask()
{
    do
    {
        GenerateMask();
		ApplyMask();
    } while (!CheckGridSolvability());
}

bool Generator::CheckGridSolvability()
{
	Solver solver;
	solver.SetGrid(fillingField);
//	int solutionsNumber = solver.GetSolutionNum();
//	if (solutionsNumber == 1) return 1;
    if (solver.Solutions() == 1) return 1;
	else return 0;
}

void Generator::FillStringRandom(int stringNumber)
{
	srand(time(NULL));
	for (int i = 0; i < SIZE; i++)
	{
		fillingField[stringNumber][i] = rand() % SIZE + 1;
		for (int j = 0; j < i; j++)
		if (fillingField[stringNumber][j] == fillingField[stringNumber][i])
		{
			i--;
			break;
		}
	}
	return;
}

void Generator::Generate()
{
    FlushFields();
	GenerateField();
    FindSuitableMask();
}

void Generator::UpdateMatrix(int stringNumber)
{
	for (int i = 0; i < SIZE; i++)
	{
		valueMatrix[fillingField[stringNumber][i] - 1][i] = 1;
		//      ����������� ������
		FillSubRowOfValueMatrix(fillingField[stringNumber][i] - 1, i, 2);
    }
	//������� ������, ���� ������� � ���������� �������� 3�3
	int counter = 0;    //���������� ������ � �������
	for (int i = 0; i < SIZE; i++)
	if (valueMatrix[0][i] == 1) counter++;
	if (counter % (int)sqrt(SIZE) == 0)
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				if (valueMatrix[i][j] == 2) valueMatrix[i][j] = 0;
	}
	return;
}

const int Generator::GetValueInCell(int i, int j)
{
	return grid[i][j];
}

const int Generator::GetValueInFillingField(int i, int j)
{
    return fillingField[i][j];
}

VPInt Generator::Hungarian(const VVInt &matrix)
{
	const int inf = numeric_limits<int>::max();
	// ������� �������
	int height = matrix.size(), width = matrix[0].size();

	// ��������, ���������� �� ����� (u) � �������� (v)
	VInt u(height, 0), v(width, 0);

	// ������ ���������� ������ � ������ �������
	VInt markIndices(width, -1);

	// ����� ��������� ������ ������� ���� �� ������
	for (int i = 0; i < height; i++) {
		VInt links(width, -1);
		VInt mins(width, inf);
		VInt visited(width, 0);

		// ���������� �������� (�������� "������������ �������" �� ������� ���������)
		int markedI = i, markedJ = -1, j;
		while (markedI != -1) {
			// ������� ���������� � ��������� � ���������� ������� ������������ ��������
			// ������ �������� � j ������ ������������� ������� � ����� ��������� �� ���
			j = -1;
			for (int j1 = 0; j1 < width; j1++)
			if (!visited[j1]) {
				if (matrix[markedI][j1] - u[markedI] - v[j1] < mins[j1]) {
					mins[j1] = matrix[markedI][j1] - u[markedI] - v[j1];
					links[j1] = markedJ;
				}
				if (j == -1 || mins[j1] < mins[j])
					j = j1;
			}

			// ������ ��� ���������� ������� � ��������� (markIndices[links[j]], j)
			// ���������� ����������� �� �������� � ��������� ���, ����� �� ���������
			int delta = mins[j];
			for (int j1 = 0; j1 < width; j1++)
			if (visited[j1]) {
				u[markIndices[j1]] += delta;
				v[j1] -= delta;
			}
			else {
				mins[j1] -= delta;
			}
			u[i] += delta;

			// ���� �������� �� ��������� - �������� � ��������� ��������
			visited[j] = 1;
			markedJ = j;
			markedI = markIndices[j];
		}

		// ������� �� ��������� ������������ ������� ������, ������ ������� �
		// ���������� ������ � �������� ������� �� ������������
		for (; links[j] != -1; j = links[j])
			markIndices[j] = markIndices[links[j]];
		markIndices[j] = i;
	}

	// ������ ��������� � ������������ �����
	VPInt result;
	for (int j = 0; j < width; j++)
	if (markIndices[j] != -1)
		result.push_back(PInt(markIndices[j], j));
	return result;
}
void Generator::FillSubRowOfValueMatrix(int i, int j, int value)
{
	int firstColPos = j - j%(int)sqrt(SIZE);
	int lastColPos = firstColPos + (int)sqrt(SIZE);
	for(j = firstColPos; j < lastColPos; j++)
		if (valueMatrix[i][j] != 1)
			valueMatrix[i][j] = 2;
}

bool Generator::isCorrectly()
{
    bool testArray[SIZE];

    //check string
    for(int i = 0; i < SIZE; i++)
    {
        for(int k = 0; k < SIZE; k++)
            testArray[k] = 1;
        for(int j = 0; j < SIZE; j++)
        {
            if(testArray[fillingField[i][j] - 1] == 0)
                return false;
            testArray[fillingField[i][j] - 1] = 0;
        }
    }

    //check columns
    for(int i = 0; i < SIZE; i++)
    {
        for(int k = 0; k < SIZE; k++)
            testArray[k] = 1;
        for(int j = 0; j < SIZE; j++)
        {
            if(testArray[fillingField[j][i] - 1] == 0)
                return false;
            testArray[fillingField[j][i] - 1] = 0;
        }
    }
    return true;
}

void Generator::FlushFields()
{
    for(int i = 0; i < SIZE; i ++)
        for(int j = 0; j < SIZE; j++)
        {
            fillingField[i][j] = 0;
            mask[i][j] = 0;
            grid[i][j] = 0;
            valueMatrix[i][j] = 0;
        }
}
