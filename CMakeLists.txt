cmake_minimum_required(VERSION 3.17)
project(async_sockets_cpp)

set(CMAKE_CXX_STANDARD 14)
find_package (Threads REQUIRED)

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
target_include_directories(async_sockets PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/easysocket/include>
        $<INSTALL_INTERFACE:easysocket/include>
        )

target_link_libraries(async_sockets PUBLIC Threads::Threads)

add_executable(tcp-client examples/tcp-client.cpp)
add_executable(tcp-server examples/tcp-server.cpp)
add_executable(udp-client examples/udp-client.cpp)
add_executable(udp-server examples/udp-server.cpp)

target_link_libraries(tcp-client PUBLIC async_sockets)
target_link_libraries(tcp-server PUBLIC async_sockets)
target_link_libraries(udp-client PUBLIC async_sockets)
target_link_libraries(udp-server PUBLIC async_sockets)

set(FILESEASYSOCKET
        easysocket/include/basesocket.h
        easysocket/include/DllHelper.h
        easysocket/include/tcpserver.h
        easysocket/include/tcpsocket.h
        easysocket/include/udpserver.h
        easysocket/include/udpsocket.h)

install(TARGETS async_sockets DESTINATION lib)
install(FILES ${FILESEASYSOCKET} DESTINATION include)
