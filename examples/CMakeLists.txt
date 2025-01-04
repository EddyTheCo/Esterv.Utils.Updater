if(BUILD_EXAMPLES)
	qt_add_executable(CoreApp coreApp.cpp)
	if(USE_QML)
		qt_add_executable(QuickApp quickApp.cpp)
		qt6_add_qml_module(quickApp
			URI  EquickApp
			VERSION 1.0
			QML_FILES
			"qml/quickApp.qml"
			RESOURCE_PREFIX
			"/esterVtech.com/imports"
			IMPORT_PATH ${CMAKE_BINARY_DIR}
			)
		target_link_libraries(QuickApp PRIVATE Qt::Gui Qt::Qml Qt::Quick Updater
			$<$<STREQUAL:$<TARGET_PROPERTY:Updater,TYPE>,STATIC_LIBRARY>:Updaterplugin>)
		set_target_properties(QuickApp PROPERTIES
			WIN32_EXECUTABLE ON
			MACOSX_BUNDLE ON
			)
		install(TARGETS QuickApp
			COMPONENT ${PROJECT_NAME}-examples
			BUNDLE  DESTINATION . COMPONENT ${PROJECT_NAME}-examples
			)
		if(QTDEPLOY)
			qt_generate_deploy_qml_app_script(
				TARGET QuickApp
				OUTPUT_SCRIPT deploy_script1
				)
			install(SCRIPT ${deploy_script1} COMPONENT ${PROJECT_NAME}-examples)
		endif(QTDEPLOY)
	endif(USE_QML)
	target_link_libraries(CoreApp PRIVATE Updater)

	set_target_properties(CoreApp PROPERTIES
		WIN32_EXECUTABLE ON
		MACOSX_BUNDLE ON
		)
		install(TARGETS CoreApp
		COMPONENT ${PROJECT_NAME}-examples
		BUNDLE  DESTINATION . COMPONENT ${PROJECT_NAME}-examples
		)

	qt_generate_deploy_app_script(
		TARGET coreApp
		OUTPUT_SCRIPT deploy_script
		)
	install(SCRIPT ${deploy_script} COMPONENT ${PROJECT_NAME}-examples)
endif(BUILD_EXAMPLES)
