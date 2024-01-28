cmake_minimum_required(VERSION 3.0)

project("CPP_VRML_UTILITY" LANGUAGES CXX) # Fix: FindThreads only works if either C or CXX language is enabled

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (APPLE)
    # Fix: Could NOT find GMP (missing: GMP_LIBRARIES GMP_INCLUDE_DIR)
    # Note: find correct path by `brew --prefix gmp`
    set(GMP_INCLUDE_DIR "/usr/local/opt/gmp/include")
    set(GMP_LIBRARIES "/usr/local/opt/gmp/lib")
    # Fix: Could NOT find MPFR (missing: MPFR_LIBRARIES MPFR_INCLUDE_DIR)
    # Note: find correct path by `brew --prefix mpfr`
    set(MPFR_INCLUDE_DIR "/usr/local/opt/mpfr/include")
    set(MPFR_LIBRARIES "/usr/local/opt/mpfr/lib")

    # set(CAIRO_INCLUDE_DIRS "/usr/local/Cellar/cairo/1.18.0/include/")
    # set(CAIRO_LIBRARIES "/usr/local/Cellar/cairo/1.18.0/lib")
    # set(Cairo_DIR "/usr/local/Cellar/cairo/1.18.0")
elseif (WIN32)
    add_link_options(-static -static-libgcc -static-libstdc++)
elseif (UNIX)
    # Điều kiện cho các hệ thống Unix như Ubuntu
else ()
    message(FATAL_ERROR "Unsupported operating system")
endif ()

file(GLOB_RECURSE SOURCES "src/*/*.cpp" CONFIGURE_DEPEND)
file(GLOB_RECURSE HEADERS "src/*/*.h" CONFIGURE_DEPEND)

# project(${APP_NAME})
# add_executable(${APP_NAME} src/main.cpp ${SOURCES} ${HEADERS})

set(executable_sources
    # src/g4_replace_structure_value.cpp
    # src/g4_gamoslog_to_cubic.cpp
    src/rand_cube_in_vrml.cpp
)
# Link to cli11
add_subdirectory(modules/cli11)
add_subdirectory(modules/geos)

find_package(PkgConfig REQUIRED)
pkg_search_module(CAIRO REQUIRED cairo)
include_directories(${CAIRO_INCLUDE_DIRS})
# include_directories(${CAIRO_INCLUDE_DIRS})
# link_directories(${CAIRO_LIBRARIES})
# add_definitions(${CAIRO_CFLAGS_OTHER})
# include_directories(${CAIRO_INCLUDE_DIRS})

find_package(CGAL)

foreach(source ${executable_sources})
    # Lấy tên của file mã nguồn mà không có phần mở rộng (.cpp)
    get_filename_component(exec_name ${source} NAME_WE)
    project(${exec_name})
    # Tạo target cho mỗi file mã nguồn
    add_executable(${exec_name} ${source} ${SOURCES} ${HEADERS})
    target_include_directories(${exec_name} PRIVATE modules/cli11/include)
    target_include_directories(${exec_name} PRIVATE modules/geos/include)

    target_link_libraries(${exec_name} CGAL::CGAL)

    # Liên kết thư viện Cairo với target của bạn
    target_link_libraries(${exec_name} ${CAIRO_LIBRARIES})
    target_compile_options(${exec_name} PUBLIC ${CAIRO_CFLAGS_OTHER})
    # target_include_directories(${exec_name} PRIVATE ${CAIRO_INCLUDE_DIRS})
endforeach()
# target_include_directories(${APP_NAME} PRIVATE modules/cli11/include)

install(FILES ${HEADERS} DESTINATION include)