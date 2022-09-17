cmake_minimum_required(VERSION 3.23)
project(cliontest1)

set(CMAKE_CXX_STANDARD 11)

include_directories(C:\\Program\ Files\\MySQL\\MySQL\ Server\ 8.0\\include)
link_directories(C:\\Program\ Files\\MySQL\\MySQL\ Server\ 8.0\\lib)
link_libraries(libmysql)


add_executable(cliontest1 main.cpp)

target_link_libraries(cliontest1 libmysql)