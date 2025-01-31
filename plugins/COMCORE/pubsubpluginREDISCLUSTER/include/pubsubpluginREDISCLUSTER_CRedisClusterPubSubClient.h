/*
 *  pubsubpluginREDISCLUSTER: Generic GUCEF COMCORE plugin for providing pubsub via Redis
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

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CTHREADPOOL_H
#include "gucefCORE_CThreadPool.h"
#define GUCEF_CORE_CTHREADPOOL_H
#endif /* GUCEF_CORE_CTHREADPOOL_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

#include "hiredis.h"
#include "async.h"
#include "redis++.h"

#ifndef PUBSUBPLUGIN_REDISCLUSTER_REDISNODE_H
#include "pubsubpluginREDISCLUSTER_RedisNode.h"
#define PUBSUBPLUGIN_REDISCLUSTER_REDISNODE_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_REDISNODE_H ? */

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPIC_H
#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClientTopic.h"
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPIC_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPIC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace REDISCLUSTER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class PUBSUBPLUGIN_REDISCLUSTER_PLUGIN_PRIVATE_CPP CRedisClusterPubSubClient : public COMCORE::CPubSubClient
{
    public:

    static const CORE::CString TypeName;
    
    typedef std::map< const COMCORE::CPubSubClientTopicConfig* , CORE::CString::StringSet > TTopicConfigPtrToStringSetMap;

    CRedisClusterPubSubClient( const COMCORE::CPubSubClientConfig& config );

    virtual ~CRedisClusterPubSubClient() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const COMCORE::CPubSubClientTopicConfig* GetTopicConfig( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* GetTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetMultiTopicAccess( const CORE::CString& topicName    ,
                                      PubSubClientTopicSet& topicAccess ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetMultiTopicAccess( const CORE::CString::StringSet& topicNames ,
                                      PubSubClientTopicSet& topicAccess          ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool CreateMultiTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig ,
                                         PubSubClientTopicSet& topicAccess                    ) GUCEF_VIRTUAL_OVERRIDE;

    bool AutoCreateMultiTopicAccess( const COMCORE::CPubSubClientTopicConfig& templateTopicConfig ,
                                     const CORE::CString::StringSet& topicNameList                ,
                                     PubSubClientTopicSet& topicAccess                            );

    bool AutoCreateMultiTopicAccess( const TTopicConfigPtrToStringSetMap& topicsToCreate ,
                                     PubSubClientTopicSet& topicAccess                   );

    void AutoDestroyTopicAccess( const CORE::CString::StringSet& topicNames );
    
    virtual void DestroyTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetAvailableTopicNameList( CORE::CString::StringSet& topicNameList                                            ,
                                            const CORE::CString::StringSet& globPatternFilters = CORE::CString::EmptyStringSet ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual void GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Connect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    COMCORE::CPubSubClientConfig& GetConfig( void );

    CORE::ThreadPoolPtr GetThreadPool( void );
    
    const RedisNodeMap& GetRedisNodeMap( void ) const;

    RedisClusterPtr GetRedisContext( void ) const;

    private:

    bool GetRedisClusterNodeMap( RedisNodeMap& nodeMap );

    bool GetAllGlobPatternTopicNames( CORE::CString::StringSet& allGlobPatternTopicNames );
    
    bool IsStreamIndexingNeeded( void );

    CORE::UInt32 GetRedisClusterErrorRepliesCounter( bool resetCounter );

    const COMCORE::CPubSubClientTopicConfig* FindTemplateConfigForTopicName( const CORE::CString& topicName ) const;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );

    void
    OnStreamIndexingTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );

    void
    OnRedisKeyCacheUpdate( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );

    void RegisterEventHandlers( void );
    
    CRedisClusterPubSubClient( void ); /**< not implemented */

    private:

    typedef CORE::CTEventHandlerFunctor< CRedisClusterPubSubClient > TEventCallback;
    typedef std::map< CORE::CString, CRedisClusterPubSubClientTopic* > TTopicMap;

    COMCORE::CPubSubClientConfig m_config;
    RedisNodeMap m_nodeMap;
    RedisClusterPtr m_redisContext;
    CORE::UInt32 m_redisErrorReplies;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_redisReconnectTimer;
    CORE::CTimer* m_streamIndexingTimer;
    TTopicMap m_topicMap;
    CORE::ThreadPoolPtr m_threadPool;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISCLUSTER */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H ? */
