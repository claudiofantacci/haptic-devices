# Copyright: (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ugo Pattacini <ugo.pattacini@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

yarp_prepare_plugin(hapticdevicewrapper CATEGORY device
                                        TYPE HapticDeviceWrapper
                                        INCLUDE hapticdeviceWrapper.h 
                                        EXTRA_CONFIG WRAPPER=hapticdevicewrapper)


if(ENABLE_hapticdevicemod_hapticdevicewrapper)
    include_directories(${YARP_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(${PROJECT_SOURCE_DIR}/interface)
    include_directories(${PROJECT_SOURCE_DIR}/common)

    yarp_add_plugin(hapticdevicewrapper hapticdeviceWrapper.h hapticdeviceWrapper.cpp
                    ${PROJECT_SOURCE_DIR}/interface/IHapticDevice.h
                    ${PROJECT_SOURCE_DIR}/common/common.h)
    target_link_libraries(hapticdevicewrapper ${YARP_LIBRARIES})
    yarp_install(TARGETS hapticdevicewrapper
                 COMPONENT Runtime
                 LIBRARY DESTINATION ${HAPTICDEVICE_DYNAMIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES hapticdevicewrapper.ini DESTINATION ${HAPTICDEVICE_PLUGIN_MANIFESTS_INSTALL_DIR})
endif()

