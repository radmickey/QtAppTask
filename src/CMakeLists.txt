add_executable(QtAppTask main.cpp)

target_link_libraries(QtAppTask
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

target_link_libraries(Application application/Application.h application/Application.cpp)
target_link_libraries(Parameters application/Parameters.h application/Parameters.cpp)
