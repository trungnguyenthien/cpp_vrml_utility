cmake_minimum_required(VERSION 3.0)

project("CPP_VRML_UTILITY" LANGUAGES CXX) # Fix: FindThreads only works if either C or CXX language is enabled

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (APPLE)
    # Fix: Could NOT find MPFR (missing: MPFR_LIBRARIES MPFR_INCLUDE_DIR)
    # Note: find correct path by `brew --prefix mpfr`
    set(MPFR_INCLUDE_DIR "/usr/local/opt/mpfr/include")
    set(MPFR_LIBRARIES "/usr/local/opt/mpfr/lib")
elseif (WIN32)
    add_link_options(-static -static-libgcc -static-libstdc++)
elseif (UNIX)
    # Điều kiện cho các hệ thống Unix như Ubuntu
else ()
    message(FATAL_ERROR "Unsupported operating system")
endif ()

file(GLOB_RECURSE SOURCES "src/*/*.cpp" CONFIGURE_DEPEND)
file(GLOB_RECURSE HEADERS "src/*/*.h" CONFIGURE_DEPEND)

set(executable_sources
    src/rand_cube_in_vrml.cpp
)
# Link to cli11
add_subdirectory(modules/cli11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GMP REQUIRED gmp)
include_directories(${GMP_INCLUDE_DIRS})
link_directories(${GMP_LIBRARY_DIRS})
add_definitions(${GMP_CFLAGS_OTHER})

find_package(CGAL REQUIRED COMPONENTS Core)
include_directories(${CGAL_INCLUDE_DIRS})
link_directories(${CGAL_LIBRARY_DIRS})
add_definitions(${CGAL_CFLAGS_OTHER})
include(${CGAL_USE_FILE})

foreach(source ${executable_sources})
    # Lấy tên của file mã nguồn mà không có phần mở rộng (.cpp)
    get_filename_component(exec_name ${source} NAME_WE)
    project(${exec_name})
    # Tạo target cho mỗi file mã nguồn
    add_executable(${exec_name} ${source} ${SOURCES} ${HEADERS})
    target_include_directories(${exec_name} PRIVATE modules/cli11/include)

    target_link_libraries(${exec_name} ${GMP_LIBRARIES})
    # target_link_libraries(${exec_name} CGAL::Core)
    target_link_libraries(${exec_name}  ${CGAL_LIBRARIES})
endforeach()
# target_include_directories(${APP_NAME} PRIVATE modules/cli11/include)

install(FILES ${HEADERS} DESTINATION include)