#
#Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
#All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html."
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
#
# Description:
# RCS IM Library - Initial version
#
#

TEMPLATE = lib
TARGET = commonimlib

QT += core 

DEPENDPATH +=   ./inc \
                ./src

DEFINES += COMMONIMLIB_BUILD

INCLUDEPATH += ./inc
INCLUDEPATH += ../epoc32/include/app

HEADERS += inc/chatimplfactory.h \ 
    inc/chatinterfaces.h \
    inc/immanagerfactory.h \
	inc/imlib_common.h

SOURCES += src/chatimplfactory.cpp

symbian: { 
	TARGET.CAPABILITY += All -Tcb
    TARGET.UID3 = 0xE41BBB83
    TARGET.EPOCALLOWDLLDATA=1
    :BLD_INF_RULES.prj_exports += "inc/chatimplfactory.h APP_LAYER_PLATFORM_EXPORT_PATH(chatimplfactory.h)"
    :BLD_INF_RULES.prj_exports += "inc/chatinterfaces.h APP_LAYER_PLATFORM_EXPORT_PATH(chatinterfaces.h)"
    :BLD_INF_RULES.prj_exports += "inc/imlib_common.h APP_LAYER_PLATFORM_EXPORT_PATH(imlib_common.h)"
    :BLD_INF_RULES.prj_exports += "inc/immanagerfactory.h APP_LAYER_PLATFORM_EXPORT_PATH(immanagerfactory.h)"    
}

win32:include(commonimlib_installs_win32.pri)

