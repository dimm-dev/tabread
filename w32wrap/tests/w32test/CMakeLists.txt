project(w32test VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
  src/w32test.cpp
)

target_link_libraries(${PROJECT_NAME} w32wrap)

add_test(Win32WrapperTest ${PROJECT_NAME})
