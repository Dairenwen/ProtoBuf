cmake_minimum_required(VERSION 3.18)

project(test 
    LANGUAGES CXX 
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 11)

set(Protobuf_DIR "/usr/lib/x86_64-linux-gnu/cmake/protobuf/")

find_package(Protobuf CONFIG REQUIRED)

add_executable(main 
    main.cc 
    contact.pb.cc
)


#target_include_directories(main PRIVATE ${Protobuf_INCLUDE_DIRS} )


target_link_libraries(main PRIVATE Protobuf::libprotobuf)