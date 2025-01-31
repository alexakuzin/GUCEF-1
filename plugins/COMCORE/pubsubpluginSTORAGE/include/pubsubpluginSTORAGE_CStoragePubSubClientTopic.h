/*
 *  pubsubpluginSTORAGE: Generic GUCEF COMCORE plugin for providing pubsub storage
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

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#include "gucefCORE_CDynamicBufferSwap.h"
#define GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERSWAP_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#include "gucefCOMCORE_CPubSubBookmark.h"
#define GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#endif /* GUCEF_COMCORE_CPUBSUBBOOKMARK_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#include "gucefCOMCORE_CPubSubClientTopic.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginSTORAGE_CStoragePubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTVFSTASK_H
#include "pubsubpluginSTORAGE_CStoragePubSubClientTopicVfsTask.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTVFSTASK_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTVFSTASK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace STORAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CStoragePubSubClient;

class PUBSUBPLUGIN_STORAGE_PLUGIN_PRIVATE_CPP CStoragePubSubClientTopic : public COMCORE::CPubSubClientTopic
{
    public:

    typedef enum CStoragePubSubClientTopicConfig::EChannelMode  TChannelMode;
    typedef std::vector< CORE::UInt32 >                         UInt32Vector;

    CStoragePubSubClientTopic( CStoragePubSubClient* client );

    virtual ~CStoragePubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClient* GetClient( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark );

    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark );

    virtual bool SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubBookmark GetCurrentBookmark( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    const CORE::CString& GetMetricFriendlyTopicName( void ) const;

    virtual bool Publish( TPublishActionIdVector& publishActionIds, const COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgConstRawPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgRawPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgSPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TPubSubMsgsRefVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;
    bool AcknowledgeReceiptImpl( const COMCORE::CPubSubBookmark& bookmark, CORE::UInt64 receiveActionId );

    virtual bool SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const COMCORE::CPubSubClientTopicConfig& config );

    class TopicMetrics
    {
        public:

        TopicMetrics( void );

        CORE::UInt32 queuedReadyToReadBuffers;
        CORE::UInt32 smallestBufferSizeInBytes;
        CORE::UInt32 largestBufferSizeInBytes;
    };

    const TopicMetrics& GetMetrics( void ) const;

    const CStoragePubSubClientTopicConfig& GetTopicConfig( void ) const;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef CORE::CTEventHandlerFunctor< CStoragePubSubClientTopic > TEventCallback;

    // Types to implement/hook-up topic interface
    typedef std::vector< COMCORE::CBasicPubSubMsg > TPubSubMsgsVector;

    private:

    class StorageToPubSubRequest : public CORE::CIConfigurable
    {
        public:

        CORE::CDateTime startDt;
        CORE::CDateTime endDt;
        CORE::CString::StringSet vfsPubSubMsgContainersToPush;

        StorageToPubSubRequest( void );
        StorageToPubSubRequest( const CORE::CDateTime& startDt, const CORE::CDateTime& endDt );
        StorageToPubSubRequest( const StorageToPubSubRequest& src );

        virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
        virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;
        virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;
    };
    typedef std::deque< StorageToPubSubRequest > StorageToPubSubRequestDeque;
    
    struct SStorageBufferMetaData
    {
        COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex msgOffsetIndex;
        TPublishActionIdVector publishActionIds; 
    };
    typedef struct SStorageBufferMetaData TStorageBufferMetaData;
    typedef std::map< CORE::CDynamicBuffer*, TStorageBufferMetaData > TStorageBufferMetaDataMap;

    void RegisterEventHandlers( void );

    bool SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config );

    static CORE::CString GenerateMetricsFriendlyTopicName( const CORE::CString& topicName );
    static CORE::CString GenerateDefaultVfsStorageContainerFileExt( void );

    template < typename T >
    bool PublishViaMsgPtrs( TPublishActionIdVector& publishActionIds, const std::vector< T >& msgs, bool notify );

    bool GetPersistedBookmark( CORE::Int32 channelId              , 
                               const CORE::CString& topicName     , 
                               COMCORE::CPubSubBookmark& bookmark );

    bool GetLastPersistedMsgAttributes( CORE::Int32 channelId          , 
                                        const CORE::CString& topicName , 
                                        CORE::CVariant& msgId          , 
                                        CORE::CDateTime& msgDt         );

    bool GetLastPersistedMsgAttributesWithOffset( CORE::Int32 channelId          , 
                                                  const CORE::CString& topicName , 
                                                  CORE::CVariant& msgId          , 
                                                  CORE::CDateTime& msgDt         ,
                                                  CORE::UInt32 lastFileOffset    ,
                                                  bool& fileExistedButHasIssue   );

    bool
    GetPathsToPubSubStorageFiles( const CORE::CDateTime& startDt  ,
                                  const CORE::CDateTime& endDt    ,
                                  CORE::CString::StringSet& files ) const;

    bool AddStorageToPubSubRequest( const StorageToPubSubRequest& request );

    void OnUnableToFullFillStorageToPubSubRequest( const StorageToPubSubRequest& failedRequest );

    CORE::CString GetPathToLastWrittenPubSubStorageFile( CORE::UInt32 lastOffset ) const;

    bool StoreNextReceivedPubSubBuffer( void );

    bool ProcessNextStorageToPubSubRequest( void );

    bool LoadStorageFile( const CORE::CString& vfsPath       ,
                          CORE::CDynamicBuffer& targetBuffer );

    bool GetStartAndEndFromContainerFilename( const CORE::CString& fullPath ,
                                              CORE::CDateTime& startDt      ,
                                              CORE::CDateTime& endDt        ) const;

    CORE::CDynamicBufferSwap& GetSerializedMsgBuffers( void );
    
    bool TransmitNextPubSubMsgBuffer( void );

    void OnStoredPubSubMsgTransmissionFailure( const CORE::CDateTime& firstMsgDt );

    void
    OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );
    
    void
    OnSyncVfsOpsTimerCycle( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventId  ,
                            CORE::CICloneable* eventData );

    
    friend class CStoragePubSubClientTopicVfsTask;

    void PerformASyncVfsWork( void );
    void OnEndOfASyncVfsWork( void );

    bool BeginVfsOps( void );
    bool StopVfsOps( void );

    void FinalizeWriteBuffer( TStorageBufferMetaData* bufferMetaData, CORE::UInt32 bufferOffset );
    void FinalizeWriteBuffer( void );
    
    void
    OnPulseCycle( CORE::CNotifier* notifier    ,
                  const CORE::CEvent& eventId  ,
                  CORE::CICloneable* eventData );

    private:

    CStoragePubSubClient* m_client;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    CStoragePubSubClientTopicConfig m_config;
    CORE::CTimer* m_syncVfsOpsTimer;    
    CORE::CTimer* m_reconnectTimer;
    MT::CMutex m_lock;
    CORE::UInt64 m_currentPublishActionId;
    CORE::UInt64 m_currentReceiveActionId;
    TPublishActionIdVector m_publishSuccessActionIds;
    TMsgsPublishedEventData m_publishSuccessActionEventData;
    TPublishActionIdVector m_publishFailureActionIds;
    TMsgsPublishFailureEventData m_publishFailureActionEventData;
    TopicMetrics m_metrics;
    CORE::CString m_metricFriendlyTopicName;

    CORE::CDynamicBuffer* m_currentReadBuffer;
    CORE::CDynamicBuffer* m_currentWriteBuffer;
    CORE::CString m_vfsFilePostfix;
    CORE::CVariant m_lastPersistedMsgId;
    CORE::CDateTime m_lastPersistedMsgDt;
    CORE::Float32 m_encodeSizeRatio;
    StorageToPubSubRequestDeque m_storageToPubSubRequests;
   
    CORE::CDynamicBufferSwap m_buffers;
    CORE::CDateTime m_lastWriteBlockCompletion;    
    TStorageBufferMetaDataMap m_storageBufferMetaData;
    CStoragePubSubClientTopicVfsTaskPtr m_vfsOpsThread;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H ? */
