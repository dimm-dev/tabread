project(imgfilter-tests VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
find_package(gtest REQUIRED)
else()
include(GoogleTest)
set(GTEST_LIBRARIES gtest)
endif()

add_executable(${PROJECT_NAME}
  src/helper_functions_tests.cpp
  src/app.cpp
)

gtest_add_tests(TARGET ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} imgfilter ${GTEST_LIBRARIES} pthread)
