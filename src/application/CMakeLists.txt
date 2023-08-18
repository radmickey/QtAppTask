find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(Parameters Parameters.h Parameters.cpp)

target_link_libraries(Parameters Qt5::Core)
