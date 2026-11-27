# Ajustes por plataforma.

if(APPLE)
    # MACOSX_BUNDLE_ICON_FILE va al Info.plist que genera CMake y lleva solo el nombre del archivo .icns.
    set(MACOSX_BUNDLE_ICON_FILE teleprompter.icns)

    # El archivo en sí se copia a Resources, dentro del paquete.
    set(APP_ICON "${CMAKE_SOURCE_DIR}/resources/app-icons/teleprompter.icns")
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
else()
    set(APP_ICON "")
endif()
