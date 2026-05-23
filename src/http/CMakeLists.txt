add_library(http
  http.cpp
  response.cpp
  request.cpp
)
target_include_directories(http PUBLIC .)
