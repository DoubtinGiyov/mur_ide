cmake_minimum_required(VERSION 3.10)

project(mur_ide)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	set(CMAKE_MACOSX_BUNDLE ON)
endif ()

set(CMAKE_REQUIRED_FLAGS -std=c++17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(OpenCV REQUIRED)
find_package(Threads)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Widgets
        SerialPort
        WebSockets
        WebEngine
        PrintSupport
        Qml
        Quick
        Gui
        Gamepad
        Concurrent)

if (WIN32)
    include(windeployqt)
endif ()

add_executable(${PROJECT_NAME} WIN32 resources/icon.rc)
#add_executable(${PROJECT_NAME} resources/icon.rc)

target_sources(${PROJECT_NAME}
        PRIVATE sources/main.cpp
        PRIVATE sources/Application.cpp
        PRIVATE sources/Application.hxx
        PRIVATE resources/resources.qrc
        PRIVATE sources/EditorController.cpp
        PRIVATE sources/EditorController.hxx
        PRIVATE sources/EditorErrors.cpp
        PRIVATE sources/EditorErrors.hxx
        PRIVATE sources/EditorHighlighter.cpp
        PRIVATE sources/EditorHighlighter.hxx
        PRIVATE sources/EditorSearch.cpp
        PRIVATE sources/EditorSearch.hxx
        PRIVATE sources/EditorSelection.cpp
        PRIVATE sources/EditorSelection.hxx
        PRIVATE sources/EditorUtils.cpp
        PRIVATE sources/EditorUtils.hxx
        PRIVATE sources/QmlUtils.cpp
        PRIVATE sources/QmlUtils.hxx
        PRIVATE sources/TextIO.cpp
        PRIVATE sources/TextIO.hxx
        PRIVATE sources/ApplicationController.cpp
        PRIVATE sources/ApplicationController.hxx
        PRIVATE sources/ApplicationMenu.cpp
        PRIVATE sources/ApplicationMenu.hxx
        PRIVATE sources/NetworkController.cpp
        PRIVATE sources/NetworkController.hxx
        PRIVATE sources/SimulatorController.cpp
        PRIVATE sources/SimulatorController.hxx
        PRIVATE sources/ApplicationLogger.cpp
        PRIVATE sources/ApplicationLogger.hxx
        PRIVATE sources/LocalScriptsController.hxx
        PRIVATE sources/JsonUtils.cpp
        PRIVATE sources/JsonUtils.hxx
        PRIVATE sources/RemoteController.cpp
        PRIVATE sources/RemoteController.hxx
        PRIVATE sources/QmlImageItem.cpp
        PRIVATE sources/QmlImageItem.hxx
        PRIVATE sources/Gamepad.cpp
        PRIVATE sources/Gamepad.hxx
        PRIVATE sources/EditorIndenter.cpp
        PRIVATE sources/EditorIndenter.hxx
        PRIVATE sources/SettingsController.cpp
        PRIVATE sources/SettingsController.hxx
        PRIVATE sources/UpdateController.cpp
        PRIVATE sources/UpdateController.hxx
        PRIVATE sources/EditorHints.cpp
        PRIVATE sources/EditorHints.hxx
        PRIVATE sources/HintBase.cpp
        PRIVATE sources/HintBase.hxx
        PRIVATE sources/ApiTokenDialog.cpp
        PRIVATE sources/ApiTokenDialog.hxx)

if (WIN32)
    target_sources(${PROJECT_NAME}
        PRIVATE sources/LocalScriptsController_win32.cpp)
else ()
    target_sources(${PROJECT_NAME}
        PRIVATE sources/LocalScriptsController_unix.cpp)
endif()

target_compile_features(${PROJECT_NAME}
        PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::SerialPort
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::WebSockets
        PRIVATE Qt5::PrintSupport
        PRIVATE Qt5::WebEngine
        PRIVATE Qt5::Qml
        PRIVATE Qt5::Quick
        PRIVATE Qt5::Gui
        PRIVATE Qt5::Gamepad
        PRIVATE Qt5::Concurrent
        PRIVATE ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT})

if (WIN32)
    add_custom_command(TARGET mur_ide POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/resources/" "$<TARGET_FILE_DIR:mur_ide>/resources/"
        )

    set(OpenCV_DIR_DLL "${OpenCV_DIR}/bin/$<CONFIGURATION>")

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_custom_command(TARGET mur_ide POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_highgui430d.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_imgproc430d.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_core430d.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_imgcodecs430d.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_videoio430d.dll" $<TARGET_FILE_DIR:mur_ide>
            )
    else()
        add_custom_command(TARGET mur_ide POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_highgui430.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_imgproc430.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_core430.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_imgcodecs430.dll" $<TARGET_FILE_DIR:mur_ide>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/opencv_videoio430.dll" $<TARGET_FILE_DIR:mur_ide>
            )
    endif()

#    add_custom_command(TARGET mur_ide POST_BUILD
#        #COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_DIR_DLL}/zlib1.dll" $<TARGET_FILE_DIR:simulator>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/intl-8.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/ffi-7.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/z-1.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/orc-0.4-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstapp-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstreamer-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstriff-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstpbutils-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstbase-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gsttag-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstaudio-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gstvideo-1.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/glib-2.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gobject-2.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GSTREAMER_DIR_DLL}/gmodule-2.0-0.dll" $<TARGET_FILE_DIR:mur_ide>
#        )

    windeployqt(mur_ide)
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    file(GLOB_RECURSE PROJECT_RESOURCES
        ${PROJECT_SOURCE_DIR}/resources/*
        )

    install(FILES ${PROJECT_RESOURCES} DESTINATION share/mur-ide)
endif ()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
