add_executable(QtAppTask main.cpp application/Application.cpp)

target_link_libraries(QtAppTask
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

target_link_libraries(QtAppTask Parameters)
