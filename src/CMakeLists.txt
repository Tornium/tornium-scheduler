find_package(Boost REQUIRED)

file(GLOB SOURCES *.cpp)

add_executable(scheduler ${SOURCES})
target_link_libraries(scheduler ${Boost_LIBRARIES})
target_compile_options(scheduler PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic )

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)
target_link_libraries(your_target_name PRIVATE cpr::cpr)

set(cmake_export_compile_commands 1)
set_target_properties(scheduler PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
