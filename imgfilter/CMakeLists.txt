project(imgfilter VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenCV REQUIRED imgcodecs imgproc)

add_library(${PROJECT_NAME} STATIC
    include/contour/contour_area_filter.h
    include/helpers/process_image.h
    include/image/adaptive_threshold.h
    include/image/clear_highlight.h
    include/image/convert_gray.h
    include/image/crop.h
    include/contour_filter.h
    include/image_filter.h
    include/ocr_filter.h
    src/contour/contour_area_filter.cpp
    src/helpers/process_image.cpp
    src/image/adaptive_threshold.cpp
    src/image/clear_highlight.cpp
    src/image/convert_gray.cpp
    src/image/crop.cpp
    src/ocr_filter.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} actions)

add_subdirectory(tests)
