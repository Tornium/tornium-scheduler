find_package(Boost REQUIRED)

file(GLOB SOURCES *.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DSOME_DEFINITION)

add_executable(scheduler ${SOURCES})
target_include_directories(scheduler PRIVATE ../include/)

find_package(Boost REQUIRED COMPONENTS url)

target_compile_options(scheduler PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic )

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)

target_link_libraries(scheduler PRIVATE cpr::cpr ${Boost_LIBRARIES})

set_target_properties(scheduler PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
