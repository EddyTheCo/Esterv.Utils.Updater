if(BUILD_EXAMPLES)
	include(InstallRequiredSystemLibraries)
	qt_add_executable(coreApp coreApp.cpp )
	if(USE_QML)
		qt_add_executable(quickApp quickApp.cpp)
		qt6_add_qml_module(quickApp
			URI  EquickApp
			VERSION 1.0
			QML_FILES
			"qml/quickApp.qml"
			RESOURCE_PREFIX
			"/esterVtech.com/imports"
			IMPORT_PATH ${CMAKE_BINARY_DIR}
			)
		target_link_libraries(quickApp PRIVATE Qt::Gui Qt::Qml Qt::Quick updater
			$<$<STREQUAL:$<TARGET_PROPERTY:updater,TYPE>,STATIC_LIBRARY>:updaterplugin>)
		set_target_properties(quickApp PROPERTIES
			WIN32_EXECUTABLE ON
			MACOSX_BUNDLE ON
			)
		install(TARGETS quickApp
			COMPONENT ${PROJECT_NAME}-examples
			BUNDLE  DESTINATION . COMPONENT ${PROJECT_NAME}-examples
			)
		if(QTDEPLOY)
			qt_generate_deploy_qml_app_script(
				TARGET quickApp
				OUTPUT_SCRIPT deploy_script1
				)
			install(SCRIPT ${deploy_script1} COMPONENT ${PROJECT_NAME}-examples)
		endif(QTDEPLOY)
	endif(USE_QML)
	target_link_libraries(coreApp PRIVATE updater)

	set_target_properties(coreApp PROPERTIES
		WIN32_EXECUTABLE ON
		MACOSX_BUNDLE ON
		)
	install(TARGETS coreApp
		COMPONENT ${PROJECT_NAME}-examples
		BUNDLE  DESTINATION . COMPONENT ${PROJECT_NAME}-examples
		)

	qt_generate_deploy_app_script(
		TARGET coreApp
		OUTPUT_SCRIPT deploy_script
		)
	install(SCRIPT ${deploy_script} COMPONENT ${PROJECT_NAME}-examples)
endif(BUILD_EXAMPLES)
