# sudoku-c-extension
Small C extension for generating Sudoku puzzles in Python.

Folders:
* binaries. Contains compiled module (VC 2010 for Python 3.4).
* sources. Contains sources of the extension.
* tests. Speed test comparing pure Python and Python+C implementations.

Concluisions:
* C module is 18 times faster then Python solution.
* Using C API implies a lot of boilerplate code: sources/sudokumodule.cpp is literally about calling one function, but we have to parse function arguments, register the module, etc.
* From Python interpreter's point of view, the module is thread-safe and re-enterable since it doesn't alter the GIL. (that means, C code is guaranteed to run from start till end without any interruption unless GIL is explicitly released).
