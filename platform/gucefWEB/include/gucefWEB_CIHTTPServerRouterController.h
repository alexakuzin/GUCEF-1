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

#ifndef GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H
#define GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORY_H
#include "CTFactory.h"
#define GUCEF_CORE_CTFACTORY_H
#endif /* GUCEF_CORE_CTFACTORY_H ? */

#ifndef GUCEF_WEB_CIHTTPSERVERROUTER_H
#include "gucefWEB_CIHTTPServerRouter.h"
#define GUCEF_WEB_CIHTTPSERVERROUTER_H
#endif /* GUCEF_WEB_CIHTTPSERVERROUTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CIHTTPServerRouterController
{   
    public:
    
    CIHTTPServerRouterController( void );
    
    CIHTTPServerRouterController( const CIHTTPServerRouterController& src );
    
    virtual ~CIHTTPServerRouterController();
    
    CIHTTPServerRouterController& operator=( const CIHTTPServerRouterController& src );
    
    virtual CIHTTPServerRouter* GetHandler( const CString& uri ) = 0;
    
    virtual CString GetUriAfterTheBaseAddress( CIHTTPServerRouter& router , 
                                               const CString& fullUri     ) = 0;
    
    virtual CString MakeUriAbsolute( CIHTTPServerRouter& router      ,
                                     const CString& uriWithAuthority ,
                                     const CString& uriSuffix        ) = 0;
    
    virtual void AddRouterMapping( CIHTTPServerRouter* router    ,
                                   const CString& uriBase        ,
                                   const CString& serviceRootUri ) = 0;
    
    virtual void RemoveRouterMapping( const CString& uriBase ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
