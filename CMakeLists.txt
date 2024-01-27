cmake_minimum_required(VERSION 3.0)
# set(APP_NAME "gtool")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (APPLE)
    # Điều kiện cho macOS
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
foreach(source ${executable_sources})
    # Lấy tên của file mã nguồn mà không có phần mở rộng (.cpp)
    get_filename_component(exec_name ${source} NAME_WE)
    project(${exec_name})
    # Tạo target cho mỗi file mã nguồn
    add_executable(${exec_name} ${source} ${SOURCES} ${HEADERS})
    target_include_directories(${exec_name} PRIVATE modules/cli11/include)
endforeach()
# target_include_directories(${APP_NAME} PRIVATE modules/cli11/include)

install(FILES ${HEADERS} DESTINATION include)