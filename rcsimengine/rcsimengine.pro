#
# Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
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


TEMPLATE = lib
TARGET = symmsrpplugin
CONFIG += plugin
CONFIG += hb
QT += core

HEADERS +=     ./inc/*.h
SOURCES +=     ./src/*.cpp
INCLUDEPATH += ../epoc32/include/app
INCLUDEPATH += ../commonimlib/inc

# LIBS += -lxqservice
LIBS += -lInetProtUtil.dll \
        -lbafl.dll \
        -lecom.dll \
        -lesock.dll \
        -lsipclient.dll \
        -lsipcodec.dll \
        -lsipprofile.dll \
        -lsipprofilecli.dll \
        -lmceclient.dll \
        -linsock.dll \
        -lsdpcodec.dll \
        -lcntmodel.dll \
        -lcommonimlib \ 
        -lqtcontacts \
		
TARGET.CAPABILITY += All \
    -Tcb
symbian: { 
    TARGET.UID3 = 0xE41BBB83
    :BLD_INF_RULES.prj_exports += "inc/chatcontactmanager.h APP_LAYER_PLATFORM_EXPORT_PATH(chatcontactmanager.h)"
    :BLD_INF_RULES.prj_exports += "inc/chatcontactmanagerimpl_sym.h APP_LAYER_PLATFORM_EXPORT_PATH(chatcontactmanagerimpl_sym.h)"
    :BLD_INF_RULES.prj_exports += "inc/chatsession.h APP_LAYER_PLATFORM_EXPORT_PATH(chatsession.h)"
}
symbian:include(symmsrpplugin_installs_sym.pri)
