cmake_minimum_required(VERSION 3.5)

project(Teleprompter VERSION 0.1.1 LANGUAGES CXX)

include(config/build.cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(include ${PROJECT_BINARY_DIR}/include/)

qt_add_resources(RESOURCES resources/resources.qrc)

set(PROJECT_SOURCES
    include/BionicWidget.hpp
    include/ChronometerWidget.hpp
    include/CreditsDialog.hpp
    include/FileWidget.hpp
    include/FontBarWidget.hpp
    include/MainWindow.hpp
    include/VersionWidget.hpp
    src/BionicWidget.cpp
    src/ChronometerWidget.cpp
    src/CreditsDialog.cpp
    src/FileWidget.cpp
    src/FontBarWidget.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/VersionWidget.cpp
    ${RESOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(teleprompter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(teleprompter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(teleprompter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(teleprompter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.nowhere.teleprompter)
endif()

set_target_properties(teleprompter PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS teleprompter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(teleprompter)
endif()
