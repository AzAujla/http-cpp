add_library(app app.cpp)
add_subdirectory(router)

target_include_directories(app PUBLIC .)
target_link_libraries(app PRIVATE router)
