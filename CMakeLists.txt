cmake_minimum_required(VERSION 3.10)

#
# CMake 3.12 - CMAKE_PROJECT_VERSION 
# Set of variables provides the version details
#
set(CMAKE_PROJECT_VERSION_MAJOR    1)
set(CMAKE_PROJECT_VERSION_MINOR    0)
set(CMAKE_PROJECT_VERSION_PATCH    2)
set(CMAKE_PROJECT_VERSION_TWEAK    0)
set(PROJECT_VERSION_DATE           "2026-07-10") 
set(PROJECT_VERSION_YEAR           "2026")

#
# Projectname
#
set(CHESSZ chessz)

add_definitions(-DCHESSZ_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_definitions(-DCHESSZ_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_definitions(-DCHESSZ_VERSION_REVISION=${CMAKE_PROJECT_VERSION_PATCH})
add_definitions(-DCHESSZ_VERSION_BUILD=${CMAKE_PROJECT_VERSION_TWEAK})
add_definitions(-DCHESSZ_VERSION_DATE=${PROJECT_VERSION_DATE})
add_definitions(-DCHESSZ_VERSION_YEAR=${PROJECT_VERSION_YEAR})

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

project(${CHESSZ} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SOUNDS "Enable sounds (requires Qt5::Multimedia)" OFF)
option(ENABLE_TTS "Enable text-to-speech (requires Qt5::TextToSpeech)" OFF)
option(ENABLE_SCID_SUPPORT "Enable support for Scid database format (*.si4)" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)

add_subdirectory(dep)

# common definitions to use with Qt
add_library(qt_config INTERFACE)
target_compile_definitions(qt_config INTERFACE
  CHESSX_MOD
  QT_DEPRECATED_WARNINGS
  QT_NO_CAST_TO_ASCII
##  QT_USE_QSTRINGBUILDER
)


##   Core Xml DBus Network OpenGL Svg PrintSupport Gui Widgets
set(REQUIRED_QT_COMPONENTS
  Core Xml Network OpenGL Svg PrintSupport Gui Widgets
)

if (ENABLE_SOUNDS)
  list(APPEND REQUIRED_QT_COMPONENTS Multimedia MultimediaWidgets)
endif()

if (ENABLE_TTS)
message(STATUS "QT component TextToSpeech")
  list(APPEND REQUIRED_QT_COMPONENTS TextToSpeech)
endif()

message(STATUS "REQUIRED_QT_COMPONENTS " ${REQUIRED_QT_COMPONENTS})
find_package(Qt5 REQUIRED
  LinguistTools
  "${REQUIRED_QT_COMPONENTS}"
  Test
) 

find_package(Qt5 COMPONENTS 
  Core 
  Xml 
  Network 
  OpenGL 
  Svg 
  PrintSupport 
  Gui 
  Widgets
  LinguistTools
  Test
REQUIRED)

if (ENABLE_SOUNDS)
  find_package(Qt5 COMPONENTS Multimedia MultimediaWidgets REQUIRED)
endif()

if (ENABLE_TTS)
  find_package(Qt5 COMPONENTS TextToSpeech REQUIRED)
endif()


# TODO: enable warnings
# CONFIG += warn_on

# put translations.rc alongside generated .qm files
configure_file(translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# list of .ts files to compile into .qm
set(TRANSLATIONS_SRC_FILES
  # i18n/chessx_da.ts
  # i18n/chessx_de.ts
  # i18n/chessx_fr.ts
  # i18n/chessx_it.ts
  # i18n/chessx_cz.ts
  # i18n/chessx_ru.ts
)

set_source_files_properties(${TRANSLATIONS_SRC_FILES}
  PROPERTIES
  OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/i18n"
)

qt5_add_translation(TRANSLATIONS_BIN_FILES ${TRANSLATIONS_SRC_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(src)

set(OTHER_FILES
  #[[
  data/templates/pgn-default.template
  data/templates/notation-default.template
  data/templates/latex-default.template
  data/templates/html-default.template
  ChangeLog
  COPYING
  ChangeLog.txt
  data/help/about.css
  data/help/about-dark.css
  data/help/about0.html
  data/help/about1.html
  data/help/about1a.html
  data/help/about2.html
  data/help/about3.html
  data/help/about4.html
  data/help/about5.html
  data/help/about6.html
  setup7-64.iss
  setup7-32.iss
  setupXP.iss
  data/styles/orange.css
  unix/chessx.desktop
  ]]
)

add_executable(${CHESSZ} WIN32
  src/gui/main.cpp
  src/common/chessx_mod.hpp
  src/common/chessx_mod.cpp
  src/common/build_inc.h
  src/common/logstream.h
  src/common/logstream.cpp
  resources.qrc
  src/chessx.rc
  ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
  ${OTHER_FILES}
)

target_include_directories(${CHESSZ}
PRIVATE
  src/common
  Qt5Multimedia_INCLUDE_DIRS
  $<$<AND:$<CONFIG:Debug>,$<PLATFORM_ID:Windows>>:${CMAKE_SOURCE_DIR}/src/vld/src>
)

## if(MSVC)
##  add_dependencies(chessx vld)
## endif()

target_link_libraries(${CHESSZ} PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<PLATFORM_ID:Windows>>:vld>
  qt_config
  quazip
  bitboard
  board
  eco
  gui

)

## if (ENABLE_TTS)
## message(STATUS "Enable Speech in chessx")
##   target_compile_definitions(chessx
##     PRIVATE
##       USE_SPEECH
##   )
##   target_link_libraries(chessx PRIVATE
##     Qt5::TextToSpeech
##   )
## else()
## message(STATUS "Disable Speech in chessx")
##   unset(USE_SPEECH CACHE)
## endif()

if(ENABLE_SOUND)
  target_compile_definitions(${CHESSZ}
    PRIVATE
      USE_SOUND
  )
else()
message(STATUS "Disable Sound in chessx")
  unset(USE_SOUND CACHE)
endif()

##
## cpu, os, compiler, runtime - section
##

##
## compiler 
##
if(CMAKE_GENERATOR MATCHES "^Visual Studio")
    add_compile_options(/MP)
endif()

##
## OS
##
if (CMAKE_HOST_APPLE)
  # Make macOS bundle instead of bare executable
  set_target_properties(chessx PROPERTIES
    MACOSX_BUNDLE TRUE
    # TODO: use variables in Info.plist template
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/mac_osx/Info.plist"
)

  # embed required resources
  target_sources(${CHESSZ} PRIVATE
    mac_osx/Info.plist
    mac_osx/qt_menu.nib
    data/images/chessx.icns
  )

  set_property(
    TARGET ${CHESSZ}
    APPEND PROPERTY
    RESOURCE
      mac_osx/qt_menu.nib
      data/images/chessx.icns
  )

  # TODO: embed engines & timeseal

endif()

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

option(ENABLE_TESTING "Enable testing" OFF)
if (ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests/unittests)
endif()

