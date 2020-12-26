project(tabread VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(include/version.h.in ${CMAKE_BINARY_DIR}/include/version.h @ONLY)

set(SOURCE_LIST
  include/application_context.h
  include/application_params.h
  include/check_params.h
  include/fill_params.h
  include/input_image_list.h
  include/task_executor.h
  include/version.h
  src/application_context.cpp
  src/application_params.cpp
  src/check_params.cpp
  src/fill_params.cpp
  src/input_image_list.cpp
  src/tabread.cpp
  src/task_executor.cpp
)

if (WIN32)
list(APPEND SOURCE_LIST include/dump_window.h src/dump_window.cpp)
endif()

if (WIN32)
set(LINK_LIBRARIES w32wrap)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

list(APPEND LINK_LIBRARIES imgfilter text)

if (NOT WIN32)
list(APPEND LINK_LIBRARIES tbb)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include PRIVATE ${CMAKE_BINARY_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

if (WIN32)
if ($ENV{TABREAD_EXTERNAL} STREQUAL "")
error("You have to set TABREAD_EXTERNAL environment variable to build package")
else()
set(TABREAD_EXTERNAL "$ENV{TABREAD_EXTERNAL}")
endif()
message("${TABREAD_EXTERNAL}")
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin COMPONENT applications)
install(DIRECTORY "${TABREAD_EXTERNAL}/bin" DESTINATION bin COMPONENT dependencies)
set(CPACK_GENERATOR "NSIS")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
else()
install(TARGETS ${PROJECT_NAME} DESTINATION /opt/tabread/bin COMPONENT bin)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "none")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
execute_process(COMMAND "dpkg" "--print-architecture" OUTPUT_VARIABLE DEB_ARCH)
string(STRIP ${DEB_ARCH} CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()

include(CPack)