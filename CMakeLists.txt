cmake_minimum_required(VERSION 3.24)

option(USE_QML "Create QML types and modules" ON)
option(BUILD_EXAMPLES "Build the examples" ON)
option(QTDEPLOY "Deploy Qt dependencies for examples" ON)
option(BUILD_DOCS "Build documentation" ON)

include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
  ccommon
  GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
  GIT_TAG v0.5.2)
FetchContent_MakeAvailable(ccommon)
version_from_git(LOG OFF TIMESTAMP "%Y%m%d%H%M%S")

project(
  EstervUpdater
  VERSION ${VERSION}
  DESCRIPTION "In-app updater"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)

if(USE_QML)
  find_package(Qt6 COMPONENTS Core Xml Gui Quick)
else()
  find_package(Qt6 COMPONENTS Core Xml)
endif(USE_QML)

if(Qt6_FOUND)
  qt_standard_project_setup()
  add_library(Updater include/esterv/utils/updater.hpp src/updater.cpp)
  add_library(Esterv::Updater ALIAS Updater)
  set_target_properties(Updater PROPERTIES VERSION ${VERSION} SOVERSION
                                                              ${VERSION_MAJOR})
  if(USE_QML)
    qt6_add_qml_module(
      Updater
      URI
      Esterv.Utils.Updater
      VERSION
      1.0
      QML_FILES
      "qml/UpdateBox.qml"
      RESOURCE_PREFIX
      "/esterVtech.com/imports"
      OUTPUT_TARGETS
      out_targets_var
      OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/Esterv/Utils/Updater
      IMPORT_PATH
      ${CMAKE_BINARY_DIR})
    add_library(Esterv::Updaterplugin ALIAS Updaterplugin)
    target_link_libraries(Updater PUBLIC Qt6::Gui Qt6::Quick)
    target_compile_definitions(Updater PRIVATE USE_QML)
  
    target_include_directories(
    Updater
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/esterv/utils>)

    install(
      TARGETS ${out_targets_var}
      EXPORT ${PROJECT_NAME}-config
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT ${PROJECT_NAME}-qml
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
              COMPONENT ${PROJECT_NAME}-qml)
    install(
      DIRECTORY ${CMAKE_BINARY_DIR}/Esterv/Utils/Updater
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Utils
      COMPONENT ${PROJECT_NAME}-qml)
    install(
      TARGETS Updaterplugin
      EXPORT ${PROJECT_NAME}-config
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Utils/Updater
      COMPONENT ${PROJECT_NAME}-qml
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
              COMPONENT ${PROJECT_NAME}-qml)
  endif(USE_QML)

  target_include_directories(
    Updater PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

  target_link_libraries(Updater PUBLIC Qt6::Core Qt6::Xml)

  target_compile_definitions(
    Updater
    PUBLIC
      $<IF:$<AND:$<BOOL:WIN32>,$<STREQUAL:$<TARGET_PROPERTY:Updater,TYPE>,SHARED_LIBRARY>>,UPDATER_SHARED,>
  )
  target_compile_definitions(
	  Updater
	  PRIVATE 
	  WINDOWS_EXPORT
  )

  install(
    TARGETS Updater
    EXPORT ${PROJECT_NAME}-config
    COMPONENT ${PROJECT_NAME}
    ARCHIVE COMPONENT ${PROJECT_NAME}-dev
    )
  install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT ${PROJECT_NAME}-dev)

  build_exports()
  if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
    build_cpack()
  endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
endif(Qt6_FOUND)

if(BUILD_DOCS)
  build_docs()
endif(BUILD_DOCS)
