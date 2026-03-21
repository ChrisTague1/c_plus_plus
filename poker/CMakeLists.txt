cmake_minimum_required(VERSION 3.20)
project(poker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-30-2
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(asio)

# Asio is header-only, just need to point at the include dir
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE asio)

add_executable(client src/client.cpp)
target_link_libraries(client PRIVATE asio)
