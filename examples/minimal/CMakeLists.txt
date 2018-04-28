include(${PROJECT_SOURCE_DIR}/pammap/python/PamMapPython.cmake)
pammap_python_add_module(testmodule SOURCES testmodule.cpp HEADERS testmodule.hpp)
