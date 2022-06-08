# MIT License
#
# Copyright (c) 2022 Martin Kloesch
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

################################################################################
# Project configuration and settings
################################################################################
cmake_minimum_required(VERSION 3.12)

# Main project configuration
project(foo
  VERSION 0.0.1
  DESCRIPTION "Library for foob'ing the Foo's baz"
  HOMEPAGE_URL "https://github.com/kmhsonnenkind/cmake-cython-example"
  LANGUAGES C CXX)

# Build options
option(BUILD_PYTHON_BINDINGS "Build Python bindings using cython" ON)
option(BUILD_DOCUMENTATION "Build API documentation using doxygen (and optionally sphinx)" OFF)
include(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(CODE_COVERAGE "Measure code coverage information" OFF "BUILD_TESTING" OFF)

# Compiler settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Internal variables
if(BUILD_TESTING AND CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(_CODE_COVERAGE 1)
else()
  set(_CODE_COVERAGE 0)
endif()


################################################################################
# Libraries
################################################################################
# Main C library
add_library(foo include/foo.h src/foo.c)
add_library(kmhsonnenkind::foo ALIAS foo)
target_include_directories(foo
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# Special version of C library for measuring code coverage
if(_CODE_COVERAGE)
  add_library(foo-coverage include/foo.h src/foo.c)
  target_include_directories(foo-coverage
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>)
  target_compile_options(foo-coverage PUBLIC -O0 -g --coverage -fPIC)
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(foo-coverage PUBLIC --coverage)
  else()
    target_link_libraries(foo-coverage PUBLIC --coverage)
  endif()
endif()

# Python bindings
if(BUILD_PYTHON_BINDINGS)
  find_package(PythonInterp 3.6 REQUIRED)
  find_package(PythonLibs 3.6 REQUIRED)

  # Prepare Python's setup.cfg
  file(GENERATE
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/setup.cfg
    CONTENT
"
[build_ext]
library_dirs=$<TARGET_FILE_DIR:foo>
include_dirs=$<JOIN:$<TARGET_PROPERTY:foo,INCLUDE_DIRECTORIES>,:>
define=$<IF:$<BOOL:${CODE_COVERAGE}>,CYTHON_TRACE,>

[build_sphinx]
build_dir=${CMAKE_CURRENT_BINARY_DIR}/docs/python
")

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python.stamp
    COMMAND ${PYTHON_EXECUTABLE} setup.py build_ext
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/python.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/setup.py
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/setup.cfg
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/src/foo/__init__.py
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/src/foo/c_foo.pxd
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/src/foo/foo_ext.pxd
      ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/src/foo/foo_ext.pyx
      foo
    COMMENT "Building Python bindings to 'foo' library"
  )
  add_custom_target(foo-python-bindings ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/python.stamp)
endif()


################################################################################
# Tests
################################################################################
include(CTest)
if(BUILD_TESTING)
  # Catch2 tests for native C library
  add_executable(foo-tests tests/foo-tests-main.cpp tests/foo-tests.cpp)
  target_include_directories(foo-tests PRIVATE tests)
  if (_CODE_COVERAGE)
    target_link_libraries(foo-tests PRIVATE foo-coverage)
  else()
    target_link_libraries(foo-tests PRIVATE foo)
  endif()
  add_test(NAME foo-tests COMMAND foo-tests)

  # Python tests for bindings
  if(BUILD_PYTHON_BINDINGS)
    # Run with coverage module to produce coverage results
    if(CODE_COVERAGE)
      add_test(
        NAME foo-python-tests
        COMMAND ${PYTHON_EXECUTABLE} -m coverage run setup.py pytest
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings)
    # Run in standard mode
    else()
      add_test(
        NAME foo-python-tests
        COMMAND ${PYTHON_EXECUTABLE} setup.py pytest
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings)
    endif()
  endif()
endif()


################################################################################
# Documentation
################################################################################
if(BUILD_DOCUMENTATION)
  # Doxygen documentation of main library
  find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)
  if(DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_HAVE_DOT YES)
  else()
    set(DOXYGEN_HAVE_DOT NO)
  endif()
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/README.md
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      ${CMAKE_CURRENT_SOURCE_DIR}/data/Doxyfile.in
      foo
    COMMENT "Building doxygen documentation for 'foo' library")
  add_custom_target(documentation ALL
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html)

  # Sphinx documentation of Python bindings
  if(BUILD_PYTHON_BINDINGS)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/docs/python/html/index.html
      COMMAND ${PYTHON_EXECUTABLE}
        setup.py
          build_sphinx
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings
      DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/README.md
        ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/docs/conf.py
        ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings/docs/index.rst
        foo-python-bindings
      COMMENT "Building sphinx Python documentation for 'foo' library")
    add_custom_target(foo-python-documentation DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/docs/python/html/index.html)
    add_dependencies(documentation foo-python-documentation)
  endif()

endif()

################################################################################
# Installation
################################################################################
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Config file to be findable by cmake
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/data/foo-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/foo-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/foo)

# Version file for semantic version checks
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/foo-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

# Native foo library
install(
  TARGETS foo
  EXPORT foo-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
  FILES include/foo.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# CMake files for find_package()
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/foo-config-version.cmake
    ${PROJECT_BINARY_DIR}/foo-config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/foo)

# CMake targets for stuff like target_link_libraries()
install(
  EXPORT foo-targets
  FILE foo-targets.cmake
  NAMESPACE kmhsonnenkind::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/foo)
export(
  EXPORT foo-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/foo-targets.cmake
  NAMESPACE kmhsonnenkind::)
export(PACKAGE foo)

# Python bindings
if(BUILD_PYTHON_BINDINGS)
  install(CODE
    "execute_process(
      COMMAND
        ${PYTHON_EXECUTABLE}
          setup.py
            install
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extras/python-bindings)")
endif()
