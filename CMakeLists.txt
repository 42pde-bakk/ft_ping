cmake_minimum_required(VERSION 3.7)
project(ft_ping C)
set(CMAKE_CXX_STANDARD 99)

# Add c files of project root directory as source file
file(GLOB SOURCE_FILES "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")


include_directories(include)
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(ft_ping ${SOURCE_FILES} ${HEADER_FILES})
