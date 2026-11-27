cmake_minimum_required(VERSION 3.20)
project(Teleprompter VERSION 0.2.0 LANGUAGES CXX)

include(config/build.cmake)
include(config/deps.cmake)
include(config/platform.cmake)

set(HDRS
    include/BionicWidget.hpp
    include/ChronometerWidget.hpp
    include/CreditsDialog.hpp
    include/FileWidget.hpp
    include/FontBarWidget.hpp
    include/MainWindow.hpp
    include/PresentationWindow.hpp
    include/SettingsDialog.hpp
    include/SlideDeck.hpp
    include/SlidePreviewWidget.hpp
    include/SlideStripWidget.hpp
    include/Theme.hpp
    include/VersionWidget.hpp
)

set(SRCS
    src/BionicWidget.cpp
    src/ChronometerWidget.cpp
    src/CreditsDialog.cpp
    src/FileWidget.cpp
    src/FontBarWidget.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/PresentationWindow.cpp
    src/SettingsDialog.cpp
    src/SlideDeck.cpp
    src/SlidePreviewWidget.cpp
    src/SlideStripWidget.cpp
    src/Theme.cpp
    src/VersionWidget.cpp
)

qt_add_executable(teleprompter
    MANUAL_FINALIZATION
    ${HDRS}
    ${SRCS}
    ${APP_ICON}
)

target_include_directories(teleprompter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
)

target_link_libraries(teleprompter PRIVATE Qt6::Widgets Qt6::Pdf Qt6::Svg)

# Los tokens son la fuente única de los colores: se leen de design/ y viajan como recurso.
# Los iconos SVG de la hoja de estilos exigen el plugin de imágenes SVG de Qt (Qt6::Svg).
set(TELEPROMPTER_TOKENS_FILE ${CMAKE_SOURCE_DIR}/design/teleprompter.tokens.json)
set_source_files_properties(${TELEPROMPTER_TOKENS_FILE} PROPERTIES QT_RESOURCE_ALIAS tokens.json)

qt_add_resources(teleprompter "teleprompter_resources"
    PREFIX "/teleprompter"
    BASE resources
    FILES
        resources/app-icons/teleprompter.icns
        resources/app-icons/teleprompter.ico
        resources/app-icons/teleprompter.png
        resources/qss/teleprompter.qss
        resources/ui/dark/check.svg
        resources/ui/light/check.svg
        ${TELEPROMPTER_TOKENS_FILE}
)

# Identificador fijo del paquete: Qt fija uno por defecto, pero en macOS conviene que no cambie.
set(BUNDLE_ID dev.sigifredo.teleprompter)

set_target_properties(teleprompter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_ID}
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

qt_finalize_executable(teleprompter)
