find_package(Boost REQUIRED)

file(GLOB SOURCES *.cpp)

add_executable(scheduler ${SOURCES})
target_link_libraries(scheduler ${Boost_LIBRARIES})
target_compile_options(scheduler PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic )

set(cmake_export_compile_commands 1)
set_target_properties(scheduler PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
