cmake_minimum_required(VERSION 3.17)
project(async_sockets_cpp)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG True)
find_package(Threads REQUIRED)

add_library(async_sockets STATIC easysocket/include/basesocket.h
        easysocket/include/DllHelper.h
        easysocket/include/tcpserver.h
        easysocket/include/tcpsocket.h
        easysocket/include/udpserver.h
        easysocket/include/udpsocket.h
        easysocket/src/basesocket.cpp
        easysocket/src/tcpserver.cpp
        easysocket/src/tcpsocket.cpp
        easysocket/src/udpserver.cpp
        easysocket/src/udpsocket.cpp)
target_include_directories(async_sockets PUBLIC easysocket/include)
target_link_libraries(async_sockets PUBLIC Threads::Threads)

add_executable(tcp-client examples/tcp-client.cpp)
add_executable(tcp-server examples/tcp-server.cpp)
add_executable(udp-client examples/udp-client.cpp)
add_executable(udp-server examples/udp-server.cpp)

target_link_libraries(tcp-client async_sockets)
target_link_libraries(tcp-server async_sockets)
target_link_libraries(udp-client async_sockets)
target_link_libraries(udp-server async_sockets)
