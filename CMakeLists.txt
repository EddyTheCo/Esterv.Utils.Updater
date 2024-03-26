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
		COMPONENT ${PROJECT_NAME}
		)

	build_exports()
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		add_subdirectory(examples)
		build_cpack()
	endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

endif(Qt6_FOUND)


if(BUILD_DOCS)
	build_docs()
endif(BUILD_DOCS)
