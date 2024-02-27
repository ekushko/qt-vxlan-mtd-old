#-------------------------------------------------
#
# Project created by Kushko Evgenii Aleksandrovich
#
#-------------------------------------------------

NAME = VMTD

TEMPLATE  = app
TEMPLATE  = lib

CONFIG += debug_and_release
CONFIG += build_all

VERSION  = 0.0.0.1

QMAKE_TARGET_PRODUCT      = Moving Target Defense by VXLAN
QMAKE_TARGET_DESCRIPTION  = Moving Target Defense by VXLAN
QMAKE_TARGET_COMPANY      = Kushko E.A.
QMAKE_TARGET_COPYRIGHT    = (C) Kushko E.A.
#RC_ICONS                = resources/favicon.ico

#
###-----Директивы---------------------------------------------------------------------------
#

DEFINES += VMTD_LIBRARY

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += TARGET=\\\"$$TARGET\\\"

#
###-----Dependecies-------------------------------------------------------------------------
#

QT += core gui
QT += widgets
QT += network
QT += websockets

#
###-----common.pri--------------------------------------------------------------------------
#

include( ../../common.pri )

##
####-----Libraries----------------------------------------------------------------------------
##
#

#openssl.path  = $${BIN_PATH}
#
#contains(QT_ARCH, i386) {
#    win32 {
#        INCLUDEPATH += c:/Qt/Tools/OpenSSL/Win_x86/include
#        LIBS        += -Lc:/Qt/Tools/OpenSSL/Win_x86/bin -llibcrypto-1_1
#
#        openssl.files += c:/Qt/Tools/OpenSSL/Win_x86/bin/libcrypto-1_1.dll
#    }
#} else {
#    win32 {
#        INCLUDEPATH += c:/Qt/Tools/OpenSSL/Win_x64/include
#        LIBS        += -Lc:/Qt/Tools/OpenSSL/Win_x64/bin -llibcrypto-1_1-x64
#
#        openssl.files += c:/Qt/Tools/OpenSSL/Win_x64/bin/libcrypto-1_1-x64.dll
#    }
#}
#
#unix:  LIBS += -lcrypto
#
#win32: LIBS += -lDbgHelp

EXTERNAL_LIBNAMELIST  =

for( LIBNAME, EXTERNAL_LIBNAMELIST ) {

    equals( TEMPLATE, "app" ) {
        extdll.path  = $${BIN_PATH}
        win32: extdll.files += $${EXTERNAL_LIBS_PATH}/$${LIBNAME}/$${LIBNAME}$${LIB_SUFFIX}.dll
        unix:  extdll.files += $${EXTERNAL_LIBS_PATH}/$${LIBNAME}/lib$${LIBNAME}$${LIB_SUFFIX}.so
    }

    INCLUDEPATH += $${EXTERNAL_IMPORT_PATH}/$${LIBNAME}

    win32: LIBS += -L$${EXTERNAL_LIBS_PATH}/$${LIBNAME} -l$${LIBNAME}$${LIB_SUFFIX}
    unix:  LIBS += -L$${EXTERNAL_LIBS_PATH}/$${LIBNAME} -L. -l$${LIBNAME}$${LIB_SUFFIX} -Wl,-rpath,lib -Wl,-rpath,.
}

#
###-----Installs----------------------------------------------------------------------------
#

CONFIG( release, debug|release ) {
#    INSTALLS += openssl
#    INSTALLS += dll
#    INSTALLS += extdll
}

#set.path   = $${BIN_PATH}
#set.files  = resources/settings
#INSTALLS  += set

#equals( TEMPLATE, "app" ) {
#    data.path   = $${BIN_PATH}
#    data.files  = files/*
#    INSTALLS   += data
#}

equals( TEMPLATE, "lib" ) {

    message( "__"$${NAME}.pro":  For LIB copy *.h files to ./import directory" )

    incl.path   = $${IMPORT_PATH}/$${NAME}
    incl.files += $$NAME/*.h
    INSTALLS   += incl

    FOLDERSLIST  = \
        adapter \
        adapter/node \
        adapter/nxapi \
        connection \
        device \
        device/node \
        device/nxapi \
        engine \
        interface \
        protocol \
        protocol/node \
        protocol/nxapi \

    for( FOLDER, FOLDERSLIST ) {
        $${FOLDER}.path   = $${IMPORT_PATH}/$${NAME}/$${FOLDER}
        $${FOLDER}.files  = $$NAME/$${FOLDER}/*.h
        INSTALLS         += $${FOLDER}
    }
}

#
###-----app\lib.pri---------------------------------------------------------------------------
#

equals( TEMPLATE, "app" ) {
    include( ../../app.pri )
} else {
    include( ../../lib.pri )
}

#
###-----Files--------------------------------------------------------------------------------
#

CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    VMTD/VMTDBuildInfo.cpp \
    VMTD/VMTDController.cpp \
    VMTD/VMTDControllerForm.cpp \
    VMTD/VMTDRepo.cpp \
    VMTD/VMTDSettings.cpp \
    VMTD/VMTDSettingsForm.cpp \
    VMTD/adapter/VMTDAdapterForm.cpp \
    VMTD/adapter/node/VMTDNodeAdapterForm.cpp \
    VMTD/adapter/node/VMTDNodeClient.cpp \
    VMTD/adapter/node/VMTDNodeClientForm.cpp \
    VMTD/adapter/node/VMTDNodeServer.cpp \
    VMTD/adapter/node/VMTDNodeServerForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServer.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.cpp \
    VMTD/connection/VMTDConnectionForm.cpp \
    VMTD/connection/VMTDConnectionManager.cpp \
    VMTD/connection/VMTDConnectionManagerForm.cpp \
    VMTD/device/VMTDDevice.cpp \
    VMTD/device/VMTDDeviceManager.cpp \
    VMTD/device/VMTDDeviceManagerForm.cpp \
    VMTD/device/node/VMTDNodeDevice.cpp \
    VMTD/device/node/VMTDNodeDeviceForm.cpp \
    VMTD/device/nxapi/VMTDNxApiDevice.cpp \
    VMTD/device/nxapi/VMTDNxApiDeviceForm.cpp \
    VMTD/engine/VMTDBridge.cpp \
    VMTD/engine/VMTDConfigurator.cpp \
    VMTD/engine/VMTDConfiguratorForm.cpp \
    VMTD/engine/VMTDEngine.cpp \
    VMTD/engine/VMTDEngineForm.cpp \
    VMTD/engine/VMTDGroup.cpp \
    VMTD/engine/VMTDParticipant.cpp \
    VMTD/engine/VMTDParticipantForm.cpp \
    VMTD/interface/VMTDInterface.cpp \
    VMTD/interface/VMTDInterfaceForm.cpp \
    VMTD/interface/VMTDInterfaceManager.cpp \
    VMTD/interface/VMTDInterfaceManagerForm.cpp \
    VMTD/protocol/VMTDProtocol.cpp \
    VMTD/protocol/VMTDProtocolForm.cpp \
    VMTD/protocol/VMTDProtocolHandler.cpp \
    VMTD/protocol/VMTDProtocolHandlerForm.cpp \
    VMTD/protocol/node/VMTDNodeProtocolHandler.cpp \
    VMTD/protocol/node/VMTDNodeProtocolHandlerForm.cpp \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.cpp \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.cpp

HEADERS += \
    MainWindow.h \
    VMTD/VMTDBuildInfo.h \
    VMTD/VMTDController.h \
    VMTD/VMTDControllerForm.h \
    VMTD/VMTDDeclareShared.h \
    VMTD/VMTDRepo.h \
    VMTD/VMTDSettings.h \
    VMTD/VMTDSettingsForm.h \
    VMTD/adapter/VMTDAdapterForm.h \
    VMTD/adapter/node/VMTDNodeAdapterForm.h \
    VMTD/adapter/node/VMTDNodeClient.h \
    VMTD/adapter/node/VMTDNodeClientForm.h \
    VMTD/adapter/node/VMTDNodeServer.h \
    VMTD/adapter/node/VMTDNodeServerForm.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.h \
    VMTD/adapter/nxapi/VMTDNxApiServer.h \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.h \
    VMTD/connection/VMTDConnectionForm.h \
    VMTD/connection/VMTDConnectionManager.h \
    VMTD/connection/VMTDConnectionManagerForm.h \
    VMTD/device/VMTDDevice.h \
    VMTD/device/VMTDDeviceManager.h \
    VMTD/device/VMTDDeviceManagerForm.h \
    VMTD/device/node/VMTDNodeDevice.h \
    VMTD/device/node/VMTDNodeDeviceForm.h \
    VMTD/device/nxapi/VMTDNxApiDevice.h \
    VMTD/device/nxapi/VMTDNxApiDeviceForm.h \
    VMTD/engine/VMTDBridge.h \
    VMTD/engine/VMTDConfigurator.h \
    VMTD/engine/VMTDConfiguratorForm.h \
    VMTD/engine/VMTDEngine.h \
    VMTD/engine/VMTDEngineForm.h \
    VMTD/engine/VMTDGroup.h \
    VMTD/engine/VMTDParticipant.h \
    VMTD/engine/VMTDParticipantForm.h \
    VMTD/interface/VMTDInterface.h \
    VMTD/interface/VMTDInterfaceForm.h \
    VMTD/interface/VMTDInterfaceManager.h \
    VMTD/interface/VMTDInterfaceManagerForm.h \
    VMTD/protocol/VMTDProtocol.h \
    VMTD/protocol/VMTDProtocolForm.h \
    VMTD/protocol/VMTDProtocolHandler.h \
    VMTD/protocol/VMTDProtocolHandlerForm.h \
    VMTD/protocol/node/VMTDNodeProtocolHandler.h \
    VMTD/protocol/node/VMTDNodeProtocolHandlerForm.h \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.h \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.h

FORMS += \
    MainWindow.ui \
    VMTD/VMTDControllerForm.ui \
    VMTD/VMTDSettingsForm.ui \
    VMTD/adapter/VMTDAdapterForm.ui \
    VMTD/adapter/node/VMTDNodeAdapterForm.ui \
    VMTD/adapter/node/VMTDNodeClientForm.ui \
    VMTD/adapter/node/VMTDNodeServerForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.ui \
    VMTD/connection/VMTDConnectionForm.ui \
    VMTD/connection/VMTDConnectionManagerForm.ui \
    VMTD/device/VMTDDeviceManagerForm.ui \
    VMTD/device/node/VMTDNodeDeviceForm.ui \
    VMTD/device/nxapi/VMTDNxApiDeviceForm.ui \
    VMTD/engine/VMTDConfiguratorForm.ui \
    VMTD/engine/VMTDEngineForm.ui \
    VMTD/engine/VMTDParticipantForm.ui \
    VMTD/interface/VMTDInterfaceForm.ui \
    VMTD/interface/VMTDInterfaceManagerForm.ui \
    VMTD/protocol/VMTDProtocolForm.ui \
    VMTD/protocol/VMTDProtocolHandlerForm.ui \
    VMTD/protocol/node/VMTDNodeProtocolHandlerForm.ui \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandlerForm.ui

RESOURCES += \
    resources/VMTDResources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
