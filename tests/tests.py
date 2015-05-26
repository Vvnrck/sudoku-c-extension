import timeit
import platform
import sudoku as c_extension_sudoku
import py_sudoku


_system_information = '''
    System:
        {processor}
        {OS} {rel} {ver}

'''.format(
    processor=platform.processor(), OS=platform.system(),
    rel=platform.release(), ver=platform.version()
)


_report_template = \
'''
Sudoku generation time (x{t}):
    Python module: {py}s.
    C module: {c}s. ({n} times faster)''' \
+ _system_information


def compare_sudoku_generation(times=10**3):
    measure = lambda c, s: round(
        timeit.timeit(c, setup=s, number=times), 4
    )

    c_setup = 'from __main__ import c_extension_sudoku' 
    c_code = 'c_extension_sudoku.generate()' 
    c_time = measure(c_code, c_setup)

    py_setup = 'from __main__ import py_sudoku'
    py_code = 'py_sudoku.construct_sudoku(31)'
    py_time = measure(py_code, py_setup)

    return _report_template.format(
        py=py_time, c=c_time, t=times,
        n=round(py_time/c_time, 2),
    )


if __name__ == '__main__':
    report = compare_sudoku_generation()
    print(report)
    with open('report.txt', 'a') as f:
        f.write(report)
