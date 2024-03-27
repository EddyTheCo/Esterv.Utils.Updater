if(BUILD_EXAMPLES)

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
    target_link_libraries(quickApp PRIVATE Qt::Gui Qt::Qml Qt::Quick qupdater
        $<$<STREQUAL:$<TARGET_PROPERTY:qupdater,TYPE>,STATIC_LIBRARY>:qupdaterplugin>)
    set_target_properties(quickApp PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
install(TARGETS quickApp 
    COMPONENT examples
    BUNDLE  DESTINATION . COMPONENT examples
)
if(QTDEPLOY)
    qt_generate_deploy_qml_app_script(
        TARGET quickApp
        OUTPUT_SCRIPT deploy_script1
    )
install(SCRIPT ${deploy_script1} COMPONENT examples)
endif(QTDEPLOY)
endif(USE_QML)
target_link_libraries(coreApp PRIVATE qupdater)

set_target_properties(coreApp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
install(TARGETS coreApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT examples
)

qt_generate_deploy_app_script(
    TARGET coreApp
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})


endif(BUILD_EXAMPLES)
