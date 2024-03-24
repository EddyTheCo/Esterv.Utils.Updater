cmake_minimum_required(VERSION 3.24)

option(USE_QML "Create QML types and modules" ON)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
    ccommon
    GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
    GIT_TAG use_IFW
)
FetchContent_MakeAvailable(ccommon)
version_from_git(
    LOG       OFF
    TIMESTAMP "%Y%m%d%H%M%S"
)

project(QtUpdater VERSION ${VERSION} DESCRIPTION "In-app updater" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

if(USE_QML)
    find_package(Qt6 COMPONENTS Core Xml Gui Quick)
else()
    find_package(Qt6 COMPONENTS Core Xml)
endif()


if (Qt6_FOUND)
    qt_standard_project_setup()
    add_library(qupdater include/qupdater.hpp src/qupdater.cpp)
    add_library(${PROJECT_NAME}::qupdater ALIAS qupdater)
    set_target_properties(qupdater PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR})
    if(USE_QML)
        qt6_add_qml_module(qupdater
            URI  Esterv.Utils.Updater
            VERSION 1.0
            QML_FILES
            "qml/UpdateBox.qml"
            RESOURCE_PREFIX
            "/esterVtech.com/imports"
            OUTPUT_TARGETS out_targets_var
            OUTPUT_DIRECTORY
            ${CMAKE_BINARY_DIR}/Esterv/Utils/Updater
            IMPORT_PATH ${CMAKE_BINARY_DIR}
        )
    add_library(${PROJECT_NAME}::qupdaterplugin ALIAS qupdaterplugin)
    target_link_libraries(qupdater PUBLIC Qt6::Gui Qt6::Quick)
    target_compile_definitions(qupdater PRIVATE USE_QML)

    install(TARGETS ${out_targets_var}
        EXPORT ${PROJECT_NAME}-config
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT updater
    )
install(DIRECTORY ${CMAKE_BINARY_DIR}/Esterv/Utils/Updater
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Utils
    COMPONENT updater
)
install(TARGETS qupdaterplugin
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Utils/Updater
    COMPONENT updater
)
endif(USE_QML)


target_include_directories(qupdater PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")

target_link_libraries(qupdater PUBLIC Qt6::Core Qt6::Xml )
target_compile_definitions(qupdater PRIVATE WINDOWS_QUPDA)


install(TARGETS qupdater
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT updater
)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv
    COMPONENT updater
)

install(EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    COMPONENT ${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/Esterv/${PROJECT_NAME}
    COMPONENT updater
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT updater
)
export(EXPORT ${PROJECT_NAME}-config
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
    set(cpack_file_name "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
    if(NOT BUILD_SHARED_LIBS)
        set(cpack_file_name "${cpack_file_name}-static")
    endif(NOT BUILD_SHARED_LIBS)
    set(CPACK_PACKAGE_FILE_NAME ${cpack_file_name})
    include(CPack)
    include(CPackIFW)
    cpack_add_component(updater)
    cpack_ifw_configure_component(updater LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
    cpack_ifw_add_repository("Repo" URL "${REPO_URL}/packages/${CMAKE_SYSTEM_NAME}/" )
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

endif(Qt6_FOUND)


if(BUILD_DOCS)
    build_docs()
endif(BUILD_DOCS)
