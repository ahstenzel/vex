# CMake config
cmake_minimum_required(VERSION 3.22)
project(vex)
include(CMakeDependentOption)
include(CTest)
cmake_dependent_option(VEX_BUILD_SHARED "Build as a shared library" ON "BUILD_SHARED_LIBS" OFF)
cmake_dependent_option(VEX_BUILD_TESTING "Build test units" ON "BUILD_TESTING" OFF)
option(VEX_BUILD_CPP "Build C++ interface wrapper" OFF)

# Gather sources
if(VEX_BUILD_CPP)
	set(SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/vex_cpp_implementation.cpp")
else()
	set(SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/vex_implementation.c")
endif()

# Build interface
if (VEX_BUILD_SHARED)
	add_library(vex SHARED ${SOURCES})
else()
	add_library(vex STATIC ${SOURCES})
endif()

# Include headers
target_include_directories(vex PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

# Build tests
if (VEX_BUILD_TESTING)
	enable_testing()

	# Check for basic context initialization
	add_executable(test_init "${CMAKE_CURRENT_LIST_DIR}/test/test_init.c")
	target_include_directories(test_init PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
	target_link_libraries(test_init vex)
	add_test(NAME test_init COMMAND test_init)

	# Check for presense of a flag
	add_executable(test_flag_presence "${CMAKE_CURRENT_LIST_DIR}/test/test_flag_presence.c")
	target_include_directories(test_flag_presence PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
	target_link_libraries(test_flag_presence vex)
	add_test(NAME test_flag_presence_success COMMAND test_flag_presence -f)
	add_test(NAME test_flag_presence_failure COMMAND test_flag_presence)
	set_property(TEST test_flag_presence_failure PROPERTY WILL_FAIL TRUE)

	# Check for arguments grouping correctly
	add_executable(test_flag_grouping "${CMAKE_CURRENT_LIST_DIR}/test/test_flag_grouping.c")
	target_include_directories(test_flag_grouping PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
	target_link_libraries(test_flag_grouping vex)
	add_test(NAME test_flag_grouping_success COMMAND test_flag_grouping -i input1.txt input2.txt)
	add_test(NAME test_flag_grouping_too_many_args COMMAND test_flag_grouping -i input1.txt input2.txt input3.txt)
	add_test(NAME test_flag_grouping_too_few_args COMMAND test_flag_grouping -i input1.txt)
	set_property(TEST test_flag_grouping_too_few_args PROPERTY WILL_FAIL TRUE)
endif()