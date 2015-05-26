#include <Python.h>
#include "Generator.h"


static PyObject* sudoku_in_valid_state(PyObject* self, PyObject* args);
static PyObject* sudoku_is_solved(PyObject* self, PyObject* args);
static PyObject* sudoku_generate(PyObject* self, PyObject* args);


static void parse_sudoku_from_args(PyObject* args, int destination[9][9])
{
	int i = 0, j = 0;
	PyObject* field;
	if (!PyArg_ParseTuple(args, "O", &field)) throw exception("bad args");
	PyObject *iter = PyObject_GetIter(field), *row;
	while (row = PyIter_Next(iter))
	{
		j = 0;
		PyObject *element_iter = PyObject_GetIter(row), *element;
		while (element = PyIter_Next(element_iter))
		{
			destination[i][j] = PyLong_AsLong(element);
			j++;
			Py_DECREF(element);
		}
		i++;
		Py_DECREF(element_iter);
		Py_DECREF(row);
	}
	Py_DECREF(iter);
}


static PyObject* sudoku_in_valid_state(PyObject* self, PyObject* args)
{
	int sudoku_field[9][9];
	parse_sudoku_from_args(args, sudoku_field);

	Solver sudoku;
	sudoku.SetGrid(sudoku_field);
	return PyBool_FromLong(sudoku.ValidState());
}


static PyObject* sudoku_generate(PyObject* self, PyObject* args)
{
	Generator sudoku;
	sudoku.Generate();

	PyObject* field = PyList_New(SIZE);
	for (int i = 0; i < SIZE; i++)
	{
		PyObject* row = PyList_New(SIZE);
		for (int j = 0; j < SIZE; j++)
			PyList_SetItem(row, j, PyLong_FromLong(sudoku.GetValueInCell(i, j)));
		PyList_SetItem(field, i, row);
	}
	return field;
}


static PyObject* sudoku_is_solved(PyObject* self, PyObject* args)
{
	int sudoku_field[9][9];
	parse_sudoku_from_args(args, sudoku_field);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (sudoku_field[i][j] < 1 || sudoku_field[i][j] > 9)
				return PyBool_FromLong(0);
	Solver sudoku;
	sudoku.SetGrid(sudoku_field);
	return PyBool_FromLong(sudoku.ValidState());
}


static PyMethodDef SudokuMethods[] = {
	{ "state_is_valid",  sudoku_in_valid_state,  METH_VARARGS, "Check if sudoku is in valid state (doesn't break any game rule)." },
	{ "generate",		 sudoku_generate,	     METH_VARARGS, "Create a sudoku game with only one solution." },
	{ "is_solved",		 sudoku_is_solved,		 METH_VARARGS, "Check if sudoku game is solved (everything filled and valid)." },

	{NULL, NULL, 0, NULL}
};


static struct PyModuleDef sudokumodule = {
	PyModuleDef_HEAD_INIT,
	"sudoku",			/* name of module */
	"Sudoku module",	/* module documentation, may be NULL */
	-1,					/* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	SudokuMethods
};


PyMODINIT_FUNC PyInit_sudoku(void)
{
	return PyModule_Create(&sudokumodule);
}