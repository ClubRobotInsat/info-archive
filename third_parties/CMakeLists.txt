include_directories(asio/asio/include include)

# Transfer property to parent scope
get_property(INCLUDE_DIRECTORIES DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
set_property(DIRECTORY .. PROPERTY INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}")

add_subdirectory(GLFW)
add_subdirectory(catch)