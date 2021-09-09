cmake_minimum_required(VERSION 3.18)
project(ExampleProgram)

set(CMAKE_CXX_STANDARD 11)

add_executable(tcp-client tcp-client.cpp)
add_executable(tcp-server tcp-server.cpp)
add_executable(udp-client udp-client.cpp)
add_executable(udp-server udp-server.cpp)

# Add -lpthread
target_link_libraries(tcp-client pthread)
target_link_libraries(tcp-server pthread)
target_link_libraries(udp-client pthread)
target_link_libraries(udp-server pthread)

# Add -I../async-sockets
target_include_directories(tcp-client PUBLIC ../async-sockets)
target_include_directories(tcp-server PUBLIC ../async-sockets)
target_include_directories(udp-client PUBLIC ../async-sockets)
target_include_directories(udp-server PUBLIC ../async-sockets)
