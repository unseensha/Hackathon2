cmake_minimum_required(VERSION 3.20)
project(hackathon C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(Hackathon2
    main.cpp
    Traveler.cpp
    Scene.cpp
    Object.cpp
    Button.cpp
    
)
target_link_libraries(Hackathon2 PRIVATE raylib)