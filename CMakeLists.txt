cmake_minimum_required(VERSION 3.5)

project(ssdb LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools PrintSupport REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    qcustomplot/qcustomplot.cpp
    infoprogram.cpp
    tablewidget.cpp
    dialogopenfile.cpp
    plotwidget.cpp
    dialogsaveplot.cpp
    dialogsavedata.cpp)
set(HEADERS
    mainwindow.h
    qcustomplot/qcustomplot.h
    infoprogram.h
    tablewidget.h
    dialogopenfile.h
    plotwidget.h
    dialogsaveplot.h
    dialogsavedata.h)
set(FORMS
    mainwindow.ui
    infoprogram.ui
    tablewidget.ui
    dialogopenfile.ui
    plotwidget.ui
    dialogsaveplot.ui
    dialogsavedata.ui)
set(RESOURCES
    ssdb.qrc)
set(TRANSLATIONS
    translations/ssdb_Ru.ts)

if(ANDROID)
  add_library(ssdb SHARED
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
    ${TRANSLATIONS}
  )
else()
  add_executable(ssdb
      ${SOURCES}
      ${HEADERS}
      ${FORMS}
      ${RESOURCES}
      ${TRANSLATIONS}
  )
endif()

target_link_libraries(ssdb PRIVATE Qt5::Widgets Qt5::PrintSupport)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TRANSLATIONS})
