# MIT License
#
# Copyright (c) 2020 Martin Kloesch
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
Setup script building wrapper to native `foo` library
'''

import setuptools
from Cython.Build import cythonize
try:
    from sphinx.setup_command import BuildDoc
    _cmdclass = {'build_sphinx': BuildDoc}
except ImportError:
    import warnings
    warnings.warn("'sphinx' is required to build the documentation",
                  category=ImportWarning)
    _cmdclass = {}


# Reuse README as long description
with open('README.md', 'r') as f:
    readme = f.read()


# Actual foo wrapper written in Cython
foo_extension = setuptools.Extension(
    'foo.foo_ext',
    language='c++',
    sources=['src/foo/foo_ext.pyx'],
    libraries=['foo']
)


# Python installable package
setuptools.setup(
    name='foo',
    version='0.0.1',
    description='Wrapper to native `foo` library',
    long_description=readme,
    long_description_content_type='text/markdown',
    author='Martin Kloesch',
    author_email='martin.kloesch@gmail.com',
    url='https://github.com/kmhsonnenkind/cmake-cython-example/extras/python-bindings',
    packages=['foo'],
    ext_modules=cythonize(
        foo_extension,
        language_level=3.6
    ),
    package_dir={'': 'src'},
    package_data={
        'foo': ['foo_ext.pxd', 'c_foo.pxd']
    },
    include_package_data=True,
    setup_requires=[
        'cython >= 0.22.1',
        'pytest-runner',
        'sphinx'
    ],
    tests_require=['pytest'],
    zip_safe=False,
    cmdclass=_cmdclass,
    keywords=[
        'foo',
        'cmake',
        'cython'
    ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: Cython',
        'Programming Language :: C',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Software Development :: Libraries',
        'Topic :: Utilities',
        'Typing :: Typed'
    ],
    license='MIT'
)
