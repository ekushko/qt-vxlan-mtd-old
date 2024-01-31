#-------------------------------------------------
#
# Project created by Kushko Evgenii Aleksandrovich
#
#-------------------------------------------------

NAME = VMTD

TEMPLATE  = app
#TEMPLATE  = lib

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
        net \
        net/interface \
        net/node \
        net/nxapi \
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
    VMTD/VMTDController.cpp \
    VMTD/VMTDControllerForm.cpp \
    VMTD/VMTDRepo.cpp \
    VMTD/VMTDSettings.cpp \
    VMTD/VMTDSettingsForm.cpp \
    VMTD/adapter/node/VMTDNodeAdapterForm.cpp \
    VMTD/adapter/node/VMTDNodeClient.cpp \
    VMTD/adapter/node/VMTDNodeClientForm.cpp \
    VMTD/adapter/node/VMTDNodeServer.cpp \
    VMTD/adapter/node/VMTDNodeServerForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.cpp \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServer.cpp \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.cpp \
    VMTD/net/VMTDNet.cpp \
    VMTD/net/VMTDNetForm.cpp \
    VMTD/net/interface/VMTDInterface.cpp \
    VMTD/net/interface/VMTDInterfaceForm.cpp \
    VMTD/net/interface/VMTDInterfaces.cpp \
    VMTD/net/interface/VMTDInterfacesForm.cpp \
    VMTD/net/node/VMTDNodeDevice.cpp \
    VMTD/net/node/VMTDNodeDeviceForm.cpp \
    VMTD/net/nxapi/VMTDNxApiDevice.cpp \
    VMTD/net/nxapi/VMTDNxApiDeviceForm.cpp \
    VMTD/protocol/VMTDProtocol.cpp \
    VMTD/protocol/VMTDProtocolForm.cpp \
    VMTD/protocol/VMTDProtocolHandler.cpp \
    VMTD/protocol/VMTDProtocolHandlerForm.cpp \
    VMTD/protocol/node/VMTDNodeProtocolHandler.cpp \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.cpp

HEADERS += \
    MainWindow.h \
    VMTD/VMTDController.h \
    VMTD/VMTDControllerForm.h \
    VMTD/VMTDDeclareShared.h \
    VMTD/VMTDRepo.h \
    VMTD/VMTDSettings.h \
    VMTD/VMTDSettingsForm.h \
    VMTD/adapter/node/VMTDNodeAdapterForm.h \
    VMTD/adapter/node/VMTDNodeClient.h \
    VMTD/adapter/node/VMTDNodeClientForm.h \
    VMTD/adapter/node/VMTDNodeServer.h \
    VMTD/adapter/node/VMTDNodeServerForm.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapter.h \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.h \
    VMTD/adapter/nxapi/VMTDNxApiServer.h \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.h \
    VMTD/net/VMTDNet.h \
    VMTD/net/VMTDNetForm.h \
    VMTD/net/interface/VMTDInterface.h \
    VMTD/net/interface/VMTDInterfaceForm.h \
    VMTD/net/interface/VMTDInterfaces.h \
    VMTD/net/interface/VMTDInterfacesForm.h \
    VMTD/net/node/VMTDNodeDevice.h \
    VMTD/net/node/VMTDNodeDeviceForm.h \
    VMTD/net/nxapi/VMTDNxApiDevice.h \
    VMTD/net/nxapi/VMTDNxApiDeviceForm.h \
    VMTD/protocol/VMTDProtocol.h \
    VMTD/protocol/VMTDProtocolForm.h \
    VMTD/protocol/VMTDProtocolHandler.h \
    VMTD/protocol/VMTDProtocolHandlerForm.h \
    VMTD/protocol/node/VMTDNodeProtocolHandler.h \
    VMTD/protocol/nxapi/VMTDNxApiProtocolHandler.h

FORMS += \
    MainWindow.ui \
    VMTD/VMTDControllerForm.ui \
    VMTD/VMTDSettingsForm.ui \
    VMTD/adapter/node/VMTDNodeAdapterForm.ui \
    VMTD/adapter/node/VMTDNodeClientForm.ui \
    VMTD/adapter/node/VMTDNodeServerForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiAdapterForm.ui \
    VMTD/adapter/nxapi/VMTDNxApiServerForm.ui \
    VMTD/net/VMTDNetForm.ui \
    VMTD/net/interface/VMTDInterfaceForm.ui \
    VMTD/net/interface/VMTDInterfacesForm.ui \
    VMTD/net/node/VMTDNodeDeviceForm.ui \
    VMTD/net/nxapi/VMTDNxApiDeviceForm.ui \
    VMTD/protocol/VMTDProtocolForm.ui \
    VMTD/protocol/VMTDProtocolHandlerForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
