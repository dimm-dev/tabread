project(text VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    find_package(Tesseract REQUIRED)
    find_package(Leptonica REQUIRED)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(TESSERACT REQUIRED tesseract)
    pkg_search_module(LEPTONICA REQUIRED lept)
    set(Tesseract_LIBRARIES ${TESSERACT_LIBRARIES})
    set(Tesseract_INCLUDE_DIRS ${TESSERACT_INCLUDE_DIRS})
endif()

find_package(OpenCV REQUIRED imgproc)

add_library(${PROJECT_NAME} STATIC
    include/format.h
    include/recognize.h
    include/text_filter.h
    src/format.cpp
    src/recognize.cpp
    src/text_filter.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${Tesseract_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC actions ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES} ${OpenCV_LIBS})
