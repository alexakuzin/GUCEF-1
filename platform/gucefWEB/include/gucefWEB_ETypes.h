/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_WEB_ETYPES_H
#define GUCEF_WEB_ETYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Maximum and minimal values for the simple types which we are about to
 *      define.
 */
#define GUCEFWEB_INT8MAX     GUCEFCORE_INT8MAX
#define GUCEFWEB_INT8MIN     GUCEFCORE_INT8MIN
#define GUCEFWEB_UINT8MAX    GUCEFCORE_UINT8MAX
#define GUCEFWEB_UINT8MIN    GUCEFCORE_UINT8MIN
#define GUCEFWEB_INT16MAX    GUCEFCORE_INT16MAX
#define GUCEFWEB_INT16MIN    GUCEFCORE_INT16MIN
#define GUCEFWEB_UINT16MAX   GUCEFCORE_UINT16MAX
#define GUCEFWEB_UINT16MIN   GUCEFCORE_UINT16MIN
#define GUCEFWEB_INT32MAX    GUCEFCORE_INT32MAX
#define GUCEFWEB_INT32MIN    GUCEFCORE_INT32MIN
#define GUCEFWEB_UINT32MAX   GUCEFCORE_UINT32MAX
#define GUCEFWEB_UINT32MIN   GUCEFCORE_UINT32MIN
#define GUCEFWEB_FLOAT32MAX  GUCEFCORE_FLOAT32MAX
#define GUCEFWEB_FLOAT32MIN  GUCEFCORE_FLOAT32MIN

/*--------------------------------------------------------------------------*/

/*
 *  We only have to define types when using C++ due to namespacing 
 *  For C the gucefMT versions are automaticly used in the global namespace
 */
#ifdef __cplusplus
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CORE::UInt8 UInt8;    /* 1 byte, unsigned */
typedef CORE::Int8 Int8;      /* 1 byte, signed */

typedef CORE::UInt16 UInt16;   /* 2 bytes, unsigned */
typedef CORE::Int16 Int16;     /* 2 bytes, signed */

typedef CORE::UInt32 UInt32;   /* 4 bytes, unsigned */
typedef CORE::Int32  Int32;    /* 4 bytes, signed */

typedef CORE::Int64 Int64;     /* 8 bytes, signed */
typedef CORE::UInt64 UInt64;   /* 8 bytes, unsigned */

typedef CORE::Float32 Float32;  /* 4 bytes, signed, decimal */
typedef CORE::Float64 Float64;  /* 8 bytes, signed, decimal */

typedef CORE::CString CString;  /* import the string class, we consider it a basic type */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

#endif /* GUCEF_WEB_ETYPES_H ? */





