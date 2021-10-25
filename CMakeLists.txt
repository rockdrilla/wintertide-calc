cmake_minimum_required( VERSION 3.5 )

project( wintertide-calc VERSION 0.0.1 LANGUAGES CXX )

set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common" )
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common" )

find_package( QT NAMES Qt6 COMPONENTS Core Widgets REQUIRED )
find_package( Qt6 COMPONENTS Core Widgets REQUIRED )

set( PROJECT_SOURCES
        resources.qrc
        appinfo.h
        valclip.h
        poe_data.h
        poe_data.cpp
        poe_math.h
        poe_math.cpp
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        hint_duration.h
        hint_duration.cpp
        hint_duration.ui
        hint_cast.h
        hint_cast.cpp
        hint_cast.ui
        hint_damage.h
        hint_damage.cpp
        hint_damage.ui
        skill_dmg_table.h
        skill_dmg_table.cpp
        skill_dmg_table.ui
)

if ( WIN32 )
    qt_add_executable( ${CMAKE_PROJECT_NAME}
        WIN32
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    qt_add_executable( ${CMAKE_PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif ()

target_link_libraries( ${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

qt_finalize_executable(${CMAKE_PROJECT_NAME})

if ( WIN32 )
    set ( build_with_deploy Release MinSizeRel )
    if ( ${CMAKE_BUILD_TYPE} IN_LIST build_with_deploy )
        get_target_property( _qmake_executable Qt6::qmake IMPORTED_LOCATION )
        get_filename_component( _qt_bin_dir "${_qmake_executable}" DIRECTORY )

        function( windeployqt target )
            add_custom_command( TARGET ${target} POST_BUILD
                COMMAND "${_qt_bin_dir}/windeployqt.exe"
                        --force
                        --verbose 2
                        --release
                        --dir deploy
                        --no-system-d3d-compiler
                        --no-translations
                        --no-virtualkeyboard
                        --no-svg
                        --no-opengl
                        --no-opengl-sw
                        \"$<TARGET_FILE:${target}>\"
                COMMAND ${CMAKE_COMMAND} -E copy
                        \"$<TARGET_FILE:${target}>\"
                        \"${CMAKE_CURRENT_BINARY_DIR}/deploy/$<TARGET_FILE_NAME:${target}>\"
                COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
            )
        endfunction()

        windeployqt( ${CMAKE_PROJECT_NAME} )
    endif ()
endif ()
