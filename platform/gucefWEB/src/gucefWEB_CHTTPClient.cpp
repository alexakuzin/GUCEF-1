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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef GUCEF_COMCORE_CCOM_H
#include "CCom.h"
#define GUCEF_COMCORE_CCOM_H
#endif /* GUCEF_COMCORE_CCOM_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"      /* gucefCORE registry for URL handlers */
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#include "gucefWEB_CHTTPClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CHTTPClient::ConnectingEvent = "GUCEF::COM::CHTTPClient::ConnectingEvent";
const CORE::CEvent CHTTPClient::ConnectedEvent = "GUCEF::COM::CHTTPClient::ConnectedEvent";
const CORE::CEvent CHTTPClient::DisconnectedEvent = "GUCEF::COM::CHTTPClient::DisconnectedEvent";
const CORE::CEvent CHTTPClient::ConnectionErrorEvent = "GUCEF::COM::CHTTPClient::ConnectionErrorEvent";
const CORE::CEvent CHTTPClient::HTTPErrorEvent = "GUCEF::COM::CHTTPClient::HTTPErrorEvent";
const CORE::CEvent CHTTPClient::HTTPRedirectEvent = "GUCEF::COM::CHTTPClient::HTTPRedirectEvent";
const CORE::CEvent CHTTPClient::HTTPContentEvent = "GUCEF::COM::CHTTPClient::HTTPContentEvent";
const CORE::CEvent CHTTPClient::HTTPDataRecievedEvent = "GUCEF::COM::CHTTPClient::HTTPDataRecievedEvent";
const CORE::CEvent CHTTPClient::HTTPDataSentEvent = "GUCEF::COM::CHTTPClient::HTTPDataSentEvent";
const CORE::CEvent CHTTPClient::HTTPTransferFinishedEvent = "GUCEF::COM::CHTTPClient::HTTPTransferFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPClient::CHTTPClient( void )
    : CObservingNotifier()   
    , m_socket( false )      
    , m_downloading( false ) 
    , m_recieved( 0 )        
    , m_filesize( 0 )        
    , m_proxyHost()          
    , m_proxyPort( 80 )      
    , m_sendBuffer( true )
    , m_bytesSent( 0 )
    , m_bytesInHeaders( 0 )
    , m_bytesInBody( 0 )
    , m_currentOp( HTTP_VERB_UNKNOWN )
{GUCEF_TRACE;

    RegisterEvents();
    SubscribeTo( &m_socket );
}

/*-------------------------------------------------------------------------*/

CHTTPClient::CHTTPClient( CORE::CPulseGenerator& pulseGenerator )
    : CObservingNotifier()       
    , m_socket( pulseGenerator ,
                false          ) 
    , m_downloading( false )     
    , m_recieved( 0 )            
    , m_filesize( 0 )            
    , m_proxyHost()              
    , m_proxyPort( 80 )          
    , m_sendBuffer( true )
    , m_bytesSent( 0 )
    , m_bytesInHeaders( 0 )
    , m_bytesInBody( 0 )
    , m_currentOp( HTTP_VERB_UNKNOWN )
{GUCEF_TRACE;

    RegisterEvents();
    SubscribeTo( &m_socket );
}

/*-------------------------------------------------------------------------*/

CHTTPClient::~CHTTPClient()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::Close( void )
{GUCEF_TRACE;

    m_socket.Close();
    m_downloading = false;
    m_filesize = 0;
    m_recieved = 0;
    m_sendBuffer.Clear();
    m_bytesSent = 0;
    m_bytesInHeaders = 0;
    m_bytesInBody = 0;
    m_currentOp = HTTP_VERB_UNKNOWN;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::Post( const CORE::CString& host                                     ,
                   UInt16 port                                                   ,
                   const CORE::CString& path                                     ,
                   const CORE::CValueList* valuelistAsContent /* = GUCEF_NULL */ ,
                   const CORE::CString& contentType                              )
{GUCEF_TRACE;

    UInt32 contentsize( 0 );
    CORE::CDynamicBuffer payload;
    if ( GUCEF_NULL != valuelistAsContent )
    {
        CORE::CString kvContent = valuelistAsContent->GetAllPairs( "&" );
        payload.LinkTo( kvContent.C_String(), kvContent.Length() );
    }
    return Post( host, port, path, contentType, payload );
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPClient::Post( const CORE::CString& urlstring      ,
                   const CORE::CString& contentType    ,
                   const CORE::CDynamicBuffer& payload )
{GUCEF_TRACE;

    CORE::CString host;
    UInt16 port = 0;
    CORE::CString path;

    if ( ParseURL( urlstring ,
                   host      ,
                   port      ,
                   path      ) )
    {
        return Post( host        ,
                     port        ,
                     path        ,
                     contentType ,
                     payload     );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CHTTPClient::Post( const CORE::CString& host           ,
                   UInt16 port                         ,
                   const CORE::CString& path           ,
                   const CORE::CString& contentType    ,
                   const CORE::CDynamicBuffer& payload )
{GUCEF_TRACE;

    m_socket.Close();

    // reset our counters because we are beginning a new transfer
    m_recieved = 0;
    m_filesize = 0;
    m_bytesSent = 0;
    m_bytesInHeaders = 0;
    m_bytesInBody = 0;
    m_currentOp = HTTP_VERB_POST;
        
    // Write the HTTP headers
    CORE::CDynamicBuffer httpHeaderBuffer( 99 + host.Length() + path.Length() + contentType.Length(), true );
    sprintf( httpHeaderBuffer.AsTypePtr< char >(), "POST %s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: gucefCOM-HTTP/1.0\r\nHost: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", path.ReplaceChar( '\\', '/' ).C_String(), host.C_String(), contentType.C_String(), payload.GetDataSize() );

    if ( m_socket.ConnectTo( host ,
                             port ) )
    {
        m_bytesInHeaders = httpHeaderBuffer.GetDataSize();
        m_bytesInBody = payload.GetDataSize();

        m_sendBuffer.Append( httpHeaderBuffer );
        m_sendBuffer.Append( payload );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::Post( const CORE::CString& urlstring                                ,
                   const CORE::CValueList* valuelistAsContent /* = GUCEF_NULL */ ,
                   const CORE::CString& contentType                              )
{GUCEF_TRACE;

    CORE::CString host;
    UInt16 port = 0;
    CORE::CString path;

    if ( ParseURL( urlstring ,
                   host      ,
                   port      ,
                   path      ) )
    {
        return Post( host               ,
                     port               ,
                     path               ,
                     valuelistAsContent ,
                     contentType        );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::Get( const CORE::CString& host                      ,
                  UInt16 port                                    ,
                  const CORE::CString& path                      ,
                  const UInt32 byteoffset /* = 0 */              ,
                  const CORE::CValueList* valuelist /* = NULL */ )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Performing GET on http://" + host + ":" + CORE::Int32ToString( port ) + "/" + path );

    m_socket.Close();

    // reset our counters because we are beginning a new transfer
    m_recieved = 0;
    m_filesize = 0;
    m_bytesSent = 0;
    m_bytesInHeaders = 0;
    m_bytesInBody = 0;
    m_currentOp = HTTP_VERB_GET;

    UInt32 contentsize( 0 );
    CORE::CString valuepath( path );

    if ( NULL != valuelist )
    {
        for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
        {
            contentsize += valuelist->GetPair( i ).Length()+1;
        }

        if ( valuelist->GetCount() )
        {
            valuepath += '?';
            for ( UInt32 i=0; i<valuelist->GetCount(); ++i )
            {
                valuepath += valuelist->GetPair( i );
                if ( i+1 < valuelist->GetCount() )
                {
                        valuepath += '&';
                }
            }
        }
    }

    valuepath = valuepath.ReplaceChar( '\\', '/' );

    if ( valuepath.Length() )
    {
        if ( valuepath[ 0 ] != '/' )
        {
            CORE::CString tmp( "/" );
            valuepath = tmp + valuepath;
        }
    }

    // Check if a system wide proxy server has been enabled
    CORE::CString proxyHost;
    UInt16 proxyPort = 80;
    bool systemWideProxyActive = false;
    COMCORE::CComCoreGlobal::Instance()->GetCom().GetSystemWideProxyServer( "HTTP"                ,
                                                                            proxyHost             ,
                                                                            proxyPort             ,
                                                                            systemWideProxyActive );
    if ( !systemWideProxyActive )
    {
        // no system wide proxy has been set,.. use the local settings
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient: No system wide HTTP proxy has been set to active" );
        proxyHost = m_proxyHost;
        proxyPort = m_proxyPort;
    }
    else
    {
        // Allow a local proxy setting to override
        if ( m_proxyHost.Length() > 0 )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): A system wide HTTP proxy has been set AND a client HTTP proxy, overriding system wide proxy with the local proxy settings" );
            proxyHost = m_proxyHost;
            proxyPort = m_proxyPort;
        }
        else
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): A system wide HTTP proxy has been set and will be used by the client" );
        }
    }

    char* buffer( NULL );
    if ( proxyHost.Length() > 0 )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Using proxy server: " + proxyHost + CORE::UInt16ToString( proxyPort ) );

        // Forward our request to the proxy
        CORE::CString remoteHost( host + ":" + CORE::Int32ToString( port ) + "/" + valuepath );
        buffer = new char[ 32 + valuepath.Length() + remoteHost.Length() ];
        sprintf( buffer, "GET %s HTTP/1.1\r\n\r\n", remoteHost.C_String() );

        if ( m_socket.ConnectTo( proxyHost ,
                                 proxyPort ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Perpared HTTP header for transmission: " + CORE::CString( buffer ) );
            m_sendBuffer.Append( buffer, (UInt32)strlen( buffer ) );
            delete []buffer;
            return true;
        }
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Not using proxy server, connecting to: " + host + ":" + CORE::Int32ToString( port ) );

        if ( byteoffset == 0 )
        {
            buffer = new char[ 100 + valuepath.Length() + host.Length() ];
            sprintf( buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nAccept: */*\r\nUser-Agent: gucefCOM-HTTP/1.0 (Linux;)\r\n\r\n", valuepath.C_String(), host.C_String() );
        }
        else
        {
            CORE::CString intstr = CORE::ToString( byteoffset );

            buffer = new char[ 100 + valuepath.Length() + host.Length() + intstr.Length() ];
            sprintf( buffer, "GET %s HTTP/1.1\r\nHost: %s\r\nRange: bytes=%d-\r\nUser-Agent: gucefCOM-HTTP/1.0 (Linux;)\r\n\r\n", valuepath.C_String(), host.C_String(), byteoffset );
        }

        if ( m_socket.ConnectTo( host ,
                                 port ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Perpared HTTP header for transmission: " + CORE::CString( buffer ) );
            m_sendBuffer.Append( buffer, (UInt32)strlen( buffer ) );
            delete []buffer;
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::Get( const CORE::CString& urlstring                 ,
                  const UInt32 byteoffset /* = 0 */              ,
                  const CORE::CValueList* valuelist /* = NULL */ )
{GUCEF_TRACE;

    CORE::CString host;
    UInt16 port;
    CORE::CString path;

    if ( ParseURL( urlstring ,
                   host      ,
                   port      ,
                   path      ) )
    {
        return Get( host       ,
                    port       ,
                    path       ,
                    byteoffset ,
                    valuelist  );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::ParseURL( const CORE::CString& urlstring ,
                       CORE::CString& host            ,
                       UInt16& port                   ,
                       CORE::CString& path            )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Parsing URL: " + urlstring );

    if ( urlstring.Length() < 10 ) return false;

    CORE::CString urlStr = urlstring.ReplaceChar( '\\', '/' );
    CORE::CString lowcaseurl( urlStr.Lowercase() );

    if ( 0 != lowcaseurl.HasSubstr( "http://", true ) )
    {
        /*
         *      This client only accepts http URL's,.. D'oh
         */
        return false;
    }

    /*
     *      Parse till the end of the domain segment and get the port
     *      if it is included
     */
    CORE::CString remainder;
    remainder.Set( urlStr.C_String()+7, urlStr.Length()-7 );
    CORE::CString segmenta( remainder.SubstrToChar( '/' ) );
    CORE::CString segmentb( remainder.SubstrToChar( ':' ) );
    port = 80;
    if ( segmenta.Length() < segmentb.Length() )
    {
        host = segmenta;
        path = remainder.C_String() + segmenta.Length()+1;
    }
    else
    {
        host = segmentb;

        remainder = remainder.C_String() + segmentb.Length()+1;
        segmenta = remainder.SubstrToChar( '/' );
        port = CORE::StringToUInt16( segmenta );
        path = remainder.C_String() + segmenta.Length()+1;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

UInt32
CHTTPClient::GetBytesReceived( bool resetCounter )
{GUCEF_TRACE;

    return m_socket.GetBytesReceived( resetCounter );
}

/*-------------------------------------------------------------------------*/

UInt32 
CHTTPClient::GetBytesTransmitted( bool resetCounter )
{GUCEF_TRACE;

    return m_socket.GetBytesTransmitted( resetCounter );
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::OnRead( COMCORE::CTCPClientSocket &socket    ,
                     const char* data                     ,
                     const UInt32 bufferLength            )
{GUCEF_TRACE;

    THTTPCODE http_code = HTTPCODE_DEFAULT;
    UInt32 size( 0 ), length( bufferLength );
    bool resumeable( false );

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Received " + CORE::Int32ToString( length ) + " bytes" );

    if( !m_downloading )
    {
        if ( length < 13 )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Invalid HTTP header received" );
            return;
        }

        UInt32 i, i2, i3;
        char *headers, *tempChar;

        /*
         *	Check if the server is using the HTTP protocol.
         */
        if( ( strncmp( data, "HTTP/1.1", 8 ) != 0 ) &&
            ( strncmp( data, "HTTP/1.0", 8 ) != 0 ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): Doesn't look like HTTP protocol" );
            return;
        }

        /*
         *      Parse the error code from the string.
         */
        char codestr[ 4 ];
        codestr[ 3 ] = '\0';
        strncpy( codestr, data+9, 4 );
        http_code = (THTTPCODE) CORE::Str_To_Int( codestr );

        /*
         *      Check if an error occurred.
         *      If so we will stop processing the data.
         *      There are codes that are not errors so we need to
         *      check for them
         */
        if( http_code >= 400 )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): HTTP Error: " + CORE::Int32ToString( http_code ) );

            THTTPErrorEventData eventData( http_code );
            NotifyObservers( HTTPErrorEvent, &eventData );
            return;
        }

        /*
         *      Move to the next segment end delimiter
         *      This will give use the size of the header segment
         */
        for(i = 0; data[i] != '\r' || data[i+2] != '\r'; i++) {}
        i += 2;

        /*
         *      Create a buffer for the header data and copy
         *      the header info into it.
         */
        headers = new char[i+1];
        strncpy(headers, data, i);
        headers[i] = '\0';

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "header size: " + CORE::Int32ToString( i ) );
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, headers );

        /*
         *      Check whether we are being told to look somewhere else for
         *      the requested resource
         */
        if( http_code == HTTPCODE_302_FOUND )
        {
            m_recieved = size = 0;

            tempChar = strstr(headers, "\nLocation");

            i2 = 9;

            if(tempChar[i2] == ':') i2++;
            if(tempChar[i2] == ' ') i2++;

            for(i3 = i2; tempChar[i3] != '\r' && tempChar[i3] != '\n'; i3++) {}

            tempChar[i3] = '\0';

            socket.Close();

            tempChar = &tempChar[i2];

            THTTPRedirectEventData eventData( tempChar );
            NotifyObservers( HTTPRedirectEvent, &eventData );

            Get( tempChar, 0 );

            delete[] headers;

            return ;
        }

        /*
         *      Parse the content-length integer
         */
        tempChar = strstr(headers, "\nContent-Length");
        if( tempChar != NULL )
        {
            char *tempChar2;
            UInt32 length2;
            i2 = 15;

            if(tempChar[i2] == ':') i2++;
            if(tempChar[i2] == ' ') i2++;

            for(i3 = i2; tempChar[i3] != '\r' && tempChar[i3] != '\n'; i3++) {}

            length2 = i3 - i2;

            tempChar2 = new char[length2 + 1];

            strncpy(tempChar2, &tempChar[i2], length2 + 1);
            tempChar2[length2] = '\0';

            size = CORE::Str_To_Int(tempChar2);

            delete[] tempChar2;
        }
        else
        {
            size = 0;
        }

	    m_recieved = 0;

        /*
         *      Check whether the transfer is resumeable
         */
        tempChar = strstr( headers, "\nAccept-Ranges" );

        if( tempChar != NULL )
        {
            i2 = 14;

            if(tempChar[i2] == ':') i2++;
            if(tempChar[i2] == ' ') i2++;

            if( strncmp( &tempChar[ i2], "bytes", 5 ) == 0 )
            {
                resumeable = true;
            }
            else
            {
                resumeable = false;
            }
        }
        else
        {
            resumeable = false;
        }

        /*
         *      Clean up and reposition the pointer to remove the header
         */
        delete[] headers;
        i += 2;
        length -= i;
        data = &data[i];

        if ( 0 == size )
        {
            /*
             *      Since we did not find a content-length segment we will have to
             *      check the data for content manually and try to determine the
             *      content-length if there is any.
             */
            UInt32 i;
            char *tempChar;

            while( m_filesize < length )
            {
                if( m_filesize == 0 )
                {
                    for( i = 0; (data[i] == '\r' || data[i] == '\n') && i < length; i++ ) {}

                    if( i != 0 )
                    {
                        length -= i;
                        data = &data[i];
                    }

                    if( length == 0 ) return;

                    for( i = 0; data[i] != '\r' && data[i] != '\n' && i < length; i++) {}

                    if( i >= length )
                    {
                        m_recieved = size = 0;
                        socket.Close();

                        // Bad data received
                        NotifyObservers( ConnectionErrorEvent );
                        return;
                    }

                    tempChar = new char[i + 1];
                    strncpy(tempChar, data, i + 1);
                    tempChar[i] = '\0';
                    sscanf(tempChar, "%x", &m_filesize);
                    delete[] tempChar;

                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, CORE::UInt32ToString( m_filesize ) );

                    if( m_filesize == 0 )
                    {
                        m_recieved = size;
                        socket.Close();
                        NotifyObservers( HTTPTransferFinishedEvent );
                        return;
                    }

                    if(data[i] == '\r' && data[i + 1] == '\n') i += 2;
                    else i++;

                    length -= i;
                    data = &data[i];
                }

                if( m_filesize <= length )
                {
                    m_recieved += m_filesize;
                    length -= m_filesize;

                    // Notify observers about the HTTP transfer payload contents we received
                    CORE::CDynamicBuffer linkBuffer;
                    linkBuffer.LinkTo( data, m_filesize );
                    THTTPDataRecievedEventData cBuffer( &linkBuffer );
                    NotifyObservers( HTTPDataRecievedEvent, &cBuffer );

                    data = &data[m_filesize];
                    m_filesize = 0;
                }
            }
        }
        else
        {
            // Set the file size based on the given content size in the header
            m_filesize = size;
        }

        // Set the download flag so that we can process the next data burst we receive
        // as part of the same file and skip the header parsing
        m_downloading = true;

        // Notify observers about the HTTP payload content info we received
        struct SHTTPContentEventData cedStruct;
        cedStruct.contentSize = m_filesize;
        cedStruct.resumeSupported = resumeable;
        cedStruct.HTTPcode = http_code;
        THTTPContentEventData contentEventData( cedStruct );
        NotifyObservers( HTTPContentEvent, &contentEventData );

        // Check if there is more data in the buffer then just the content header
        if ( m_filesize >= length && length != 0 )
        {
            // Notify observers about the HTTP transfer payload contents we received
            CORE::CDynamicBuffer linkBuffer;
            linkBuffer.LinkTo( data, length );
            THTTPDataRecievedEventData cBuffer( &linkBuffer );
            NotifyObservers( HTTPDataRecievedEvent, &cBuffer );

            m_recieved += length;
            m_filesize -= length;

            // Check if we are already finished
            if ( m_filesize == 0 )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient: Received all the desired data thus closing the TCP socket" );
                m_socket.Close();

                NotifyObservers( HTTPTransferFinishedEvent );
            }
        }
    }
    else
    {
        // if we get here then we are busy with an ongoing download and the header was already
        // processed in a previous data burst.
        // Sanity check on data, make sure we are expecting more data to arrive
        if ( m_filesize >= length )
        {
            m_recieved += length;
            m_filesize -= length;

            // Notify observers about the HTTP transfer payload contents we received
            CORE::CDynamicBuffer linkBuffer;
            linkBuffer.LinkTo( data, length );
            THTTPDataRecievedEventData cBuffer( &linkBuffer );
            NotifyObservers( HTTPDataRecievedEvent, &cBuffer );

            // Check if we are finished
            if ( m_filesize == 0 )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient: Received all the desired data thus closing the TCP socket" );
                m_socket.Close();

                NotifyObservers( HTTPTransferFinishedEvent );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::OnDisconnect( COMCORE::CTCPClientSocket &socket )
{GUCEF_TRACE;

        m_downloading = false;
        m_filesize = 0;
        m_recieved = 0;
        m_sendBuffer.Clear();
        NotifyObservers( DisconnectedEvent );
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::OnWrite( COMCORE::CTCPClientSocket& socket                   ,
                      COMCORE::CTCPClientSocket::TDataSentEventData& data )
{GUCEF_TRACE;

    m_bytesSent += data.GetData().GetDataSize();
    
    // Notify observers about the data dispatch
    NotifyObservers( HTTPDataSentEvent, &data );

    if ( HTTP_VERB_POST == m_currentOp ||
         HTTP_VERB_PUT == m_currentOp  ||
         HTTP_VERB_PATCH == m_currentOp )
    {
        Int32 bodyBytesTransferred = (Int32)m_bytesSent - m_bytesInHeaders;
        if ( bodyBytesTransferred > 0 && (UInt32)bodyBytesTransferred >= m_bytesInBody )
        {
            NotifyObservers( HTTPTransferFinishedEvent );   
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::IsConnected( void ) const
{GUCEF_TRACE;

    return m_socket.IsActive();
}

/*-------------------------------------------------------------------------*/

bool
CHTTPClient::SetHTTPProxy( const CORE::CString& proxyHost ,
                           const UInt16 port /* = 80 */   )
{GUCEF_TRACE;
    if ( !IsConnected() )
    {
        m_proxyHost = proxyHost;
        m_proxyPort = port;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::GetHTTPProxy( CORE::CString& proxyHost ,
                           UInt16& port             )
{GUCEF_TRACE;

    proxyHost = m_proxyHost;
    port = m_proxyPort;
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::RegisterEvents( void )
{GUCEF_TRACE;

    ConnectingEvent.Initialize();
    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    ConnectionErrorEvent.Initialize();
    HTTPErrorEvent.Initialize();
    HTTPRedirectEvent.Initialize();
    HTTPContentEvent.Initialize();
    HTTPDataRecievedEvent.Initialize();
    HTTPDataSentEvent.Initialize();
    HTTPTransferFinishedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CHTTPClient::OnNotify( CORE::CNotifier* notifier                 ,
                       const CORE::CEvent& eventid               ,
                       CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( notifier == &m_socket )
    {
        if ( eventid == COMCORE::CTCPClientSocket::ConnectedEvent )
        {
            // Transmit data we have queued for transmission
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket connected to server, sending " + CORE::UInt32ToString( m_sendBuffer.GetDataSize() ) + " bytes to the server" );

            m_socket.Send( m_sendBuffer.GetConstBufferPtr(), m_sendBuffer.GetDataSize() );
            m_sendBuffer.Clear( true );

            NotifyObservers( ConnectedEvent );
        }
        else
        if ( eventid == COMCORE::CTCPClientSocket::ConnectingEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket is connecting to the server" );
            NotifyObservers( ConnectingEvent );
        }
        else
        if ( eventid == COMCORE::CTCPClientSocket::DisconnectedEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket disconnected from the server" );
            OnDisconnect( m_socket );
        }
        else
        if ( eventid == COMCORE::CTCPClientSocket::SocketErrorEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket reported an error" );
            NotifyObservers( ConnectionErrorEvent, eventdata );
        }
        else
        if ( eventid == COMCORE::CTCPClientSocket::DataRecievedEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket received data" );
            COMCORE::CTCPClientSocket::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
            OnRead( m_socket                                                          ,
                    static_cast< const char*>( eData->GetData().GetConstBufferPtr() ) ,
                    eData->GetData().GetDataSize()                                    );
        }
        else
        if ( eventid == COMCORE::CTCPClientSocket::DataSentEvent )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHTTPClient(" + CORE::PointerToString( this ) + "): TCP Socket send data to the server" );
            COMCORE::CTCPClientSocket::TDataSentEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataSentEventData* >( eventdata );
            if ( GUCEF_NULL != eData )
                OnWrite( m_socket, *eData );
        }
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CHTTPClient::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::COM::CHTTPClient";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
