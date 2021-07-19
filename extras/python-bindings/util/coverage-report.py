# MIT License
#
# Copyright (c) 2021 Martin Kloesch
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

'''
Utility script to generate coverage.py report for :module:`foo`.

coverage.py [5.5] with Cython.Coverage plugin [0.29.23] has issues with cython
files in (sub-) packages so this script will monkey-patch :module:`Cython.Coverage`
to behave correctly.

This script will only generate the report (similar to `coverage report`). You can
use coverage.py as usual to actually generate the report data (`coverage run`).
'''

import os
import typing


def _patch_cython_coverage():
    '''
    Monkey patch `Cython.Coverage` to also work with Cython packages.
    '''
    import Cython.Coverage

    def _get_reverse_hierarchy(path: str) -> typing.Generator[str, None, None]:
        '''
        Generator yielding reverse path hierarchy to simplify path comparison.

        :example:
            ```
            path = "/home/pi/workspace"
            _get_reverse_hierarchy(path)
                -> "workspace"
                -> "pi"
                -> "home"
            ```

        :param path: Path to get (reverse) hierarchy for.
        :return: Path segments in reversed order
        '''
        remaining = path
        while remaining:
            remaining, dir = os.path.split(remaining)
            if not dir:
                break
            yield dir

    # Copy + extension of file from Cython.Coverage.py [0.29.23]
    def _find_dep_file_path(main_file, file_path, relative_path_search=False):
        abs_path = Cython.Coverage.os.path.abspath(file_path)
        if not Cython.Coverage.os.path.exists(abs_path) and (file_path.endswith('.pxi') or
                                             relative_path_search):
            # files are looked up relative to the main source file
            rel_file_path = Cython.Coverage.os.path.join(Cython.Coverage.os.path.dirname(main_file), file_path)
            if Cython.Coverage.os.path.exists(rel_file_path):
                abs_path = Cython.Coverage.os.path.abspath(rel_file_path)

            # begin extension Kloesch:
            else:
                # Check if main_file and file_path share common directory structure
                file_dirs = list(_get_reverse_hierarchy(Cython.Coverage.os.path.dirname(file_path)))
                main_dirs = list(_get_reverse_hierarchy(Cython.Coverage.os.path.dirname(main_file)))
                if main_dirs[:len(file_dirs)] == file_dirs:
                    rel_file_path = Cython.Coverage.os.path.join(Cython.Coverage.os.path.dirname(main_file), Cython.Coverage.os.path.basename(file_path))
                    if Cython.Coverage.os.path.exists(rel_file_path):
                        abs_path = Cython.Coverage.os.path.abspath(rel_file_path)
            # End extension kloesch

        # search sys.path for external locations if a valid file hasn't been found
        if not Cython.Coverage.os.path.exists(abs_path):
            for sys_path in Cython.Coverage.sys.path:
                test_path = Cython.Coverage.os.path.realpath(Cython.Coverage.os.path.join(sys_path, file_path))
                if Cython.Coverage.os.path.exists(test_path):
                    return Cython.Coverage.canonical_filename(test_path)
        return Cython.Coverage.canonical_filename(abs_path)

    # Actually monkey-patch Cython.Coverage
    Cython.Coverage._find_dep_file_path = _find_dep_file_path


if __name__ == '__main__':
    # Monkey-patch cython coverage to also work with cython packages
    _patch_cython_coverage()

    # Actually create coverage report
    import coverage
    _this_dir = os.path.abspath(os.path.dirname(__file__))
    cov = coverage.Coverage(
        data_file = os.path.join(_this_dir, '..', '.coverage'),
        config_file = os.path.join(_this_dir, '..', '.coveragerc')
    )
    cov.load()
    cov.xml_report()
