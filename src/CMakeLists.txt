find_package(Boost REQUIRED)

add_executable(scheduler main.cpp server.cpp server.h)
target_link_libraries(scheduler ${Boost_LIBRARIES})
target_compile_options(scheduler PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic )

set(cmake_export_compile_commands 1)
set_target_properties(scheduler PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
