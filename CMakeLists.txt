
cmake_minimum_required(VERSION 3.10)
project(Stm32Shell)

file(GLOB_RECURSE SOURCES
        "src/*.*"
        "third_party/microrl-remaster/src/*.*"
)

add_library(Stm32Shell STATIC ${SOURCES})

target_include_directories(Stm32Shell PUBLIC
        src
        third_party/microrl-remaster/src/include/microrl
)




