add_library(router router.cpp)
target_include_directories(router PUBLIC .)
target_link_libraries(router PRIVATE http)
