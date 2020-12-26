project(w32wrap VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} STATIC
  include/lazy_value.h
  include/win32_utils.h
  include/window_manager.h
  include/window_ptr.h
  include/window_rect.h
  src/win32_utils.cpp
  src/window_manager.cpp
  src/window_ptr.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME})

add_subdirectory(tests/w32test)
