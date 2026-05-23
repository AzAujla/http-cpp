add_library(http
  http.cpp
  response.cpp
  request.cpp
  cookies.cpp
)
target_include_directories(http PUBLIC .)
