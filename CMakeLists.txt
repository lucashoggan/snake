cmake_minimum_required(VERSION 4.0.2)
project(snake)

set(SDL3_DIR "D:/coding/cpp-libs/SDL3/SDL3-3.2.14/x86_64-w64-mingw32" CACHE PATH "SDL3 root directory")

find_path(SDL3_INCLUDE_DIR SDL3/SDL.h HINTS "${SDL3_DIR}/include")
find_library(SDL3_LIBRARY SDL3 HINTS "${SDL3_DIR}/lib")

if (NOT SDL3_INCLUDE_DIR OR NOT SDL3_LIBRARY)
    message(FATAL_ERROR "SDL3 not found")
endif()

add_executable(game src/main.cpp)

target_include_directories(game PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(game PRIVATE ${SDL3_LIBRARY} )

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL3_DIR}/bin/SDL3.dll"
        $<TARGET_FILE_DIR:game>
)