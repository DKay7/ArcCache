cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(arc_lib INTERFACE)
target_include_directories(
    arc_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)


project(arc_cache VERSION 2.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG -ggdb3 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wno-narrowing -Wno-old-style-cast -fcheck-new -fstack-protector -fsanitize=address -fsanitize=alignment -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vptr -Og")

add_executable(${PROJECT_NAME} main.cpp)