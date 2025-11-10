include(CMakeDependentOption)
cmake_dependent_option(VEX_BUILD_SHARED "Build as a shared library" ON "BUILD_SHARED_LIBS" OFF)
option(VEX_BUILD_CPP "Build C++ interface wrapper" OFF)

if(VEX_BUILD_CPP)
	set(SOURCES "src/vex_cpp_implementation.cpp")
else()
	set(SOURCES "src/vex_implementation.c")
endif()

if (VEX_BUILD_SHARED)
	add_library(vex SHARED ${SOURCES})
else()
	add_library(vex STATIC ${SOURCES})
endif()

target_include_directories(vex PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")