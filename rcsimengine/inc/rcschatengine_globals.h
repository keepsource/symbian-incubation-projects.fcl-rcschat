/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html."
* Initial Contributors:
* Nokia Corporation - initial contribution.
* Contributors:
*
* Description:
* RCS IM Library - Initial version
*
*/


#ifndef RCSCHATENGINE_GLOBAL_H
#define RCSCHATENGINE_GLOBAL_H

#include <Qt/qglobal.h>


#ifdef RCSCHATENGINE_LIB
    #define RCSCHATENGINE_EXPORT Q_DECL_EXPORT
#else
    #ifdef RCSCHATENGINE_TEST
        #define RCSCHATENGINE_EXPORT
    #else
        #define RCSCHATENGINE_EXPORT Q_DECL_IMPORT
    #endif  
#endif


#endif
