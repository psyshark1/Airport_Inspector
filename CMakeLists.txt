cmake_minimum_required(VERSION 3.5)

project(AirPort_Inspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent PrintSupport Sql)

include_directories(qcustomplot)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h database.cpp
        structs.h
        dialogcalendar.h dialogcalendar.cpp dialogcalendar.ui
        graphic.cpp graphic.h
        graphwindow.cpp graphwindow.h graphwindow.ui
        barchart.h barchart.cpp
        about.h about.cpp about.ui
)

#qt_add_executable(AirPort_Inspector MANUAL_FINALIZATION ${PROJECT_SOURCES})

add_executable(AirPort_Inspector ${PROJECT_SOURCES})

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_subdirectory(qcustomplot)

target_link_libraries(AirPort_Inspector Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::PrintSupport Qt6::Sql qcustomplot)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
#if(${QT_VERSION} VERSION_LESS 6.1.0)
#  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AirPort_Inspector)
#endif()
set_target_properties(AirPort_Inspector PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AirPort_Inspector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AirPort_Inspector)
endif()
