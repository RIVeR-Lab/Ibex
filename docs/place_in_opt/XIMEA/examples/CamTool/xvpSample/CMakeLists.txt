add_library(xvpSample SHARED
    BayerPseudoColorsChnbl.cpp BayerPseudoColorsChnbl.h
    CameraTriggerForm.cpp CameraTriggerForm.h CameraTriggerForm.ui
    FlipConfigurationWgt.cpp FlipConfigurationWgt.h FlipConfigurationWgt.ui
    FlipImageCnbl.cpp FlipImageCnbl.h
    MeanGrayMeasForm.cpp MeanGrayMeasForm.h MeanGrayMeasForm.ui
    MeanGrayMeasGraphDlg.cpp MeanGrayMeasGraphDlg.h MeanGrayMeasGraphDlg.ui
    NegativeChnbl.cpp NegativeChnbl.h
    sampleplugin.cpp sampleplugin.h
)
target_include_directories(xvpSample PRIVATE
    ../../qcustomplot
    ../../xiCore/inc
    ../../xiCoreGui/inc
)

target_compile_definitions(xvpSample PRIVATE QCUSTOMPLOT_USE_LIBRARY)

target_link_libraries(xvpSample PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    xiCore
    xiCoreGui
)
if(WIN32)
    target_link_libraries(xvpSample PRIVATE qcustomplot2)
else()
    target_link_libraries(xvpSample PRIVATE qcustomplot)
endif()

# Resources:
if(QT_VERSION_MAJOR GREATER 5)
    set(xvpSample_resource_files
        "images/flipHoriz.png"
        "images/flipVert.png"
        "images/start.png"
        "images/stop.png"
    )
    qt_add_resources(xvpSample "xvpSample" PREFIX "/" FILES ${xvpSample_resource_files})
else()
    qt5_add_resources(xvpSample_RESOURCES xvpSample.qrc)
    target_sources(xvpSample PRIVATE ${xvpSample_RESOURCES})
endif()
