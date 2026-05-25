add_library(session session.cpp)
target_include_directories(session PUBLIC .)
target_link_libraries(session PRIVATE http)
