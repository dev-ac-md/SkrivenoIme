//
//	UDP based Communication Core
//	GSC Game World
//	© 2001 Serguei I. Ivantsov aka ManOwaR Linux Lover
//
//	Jun 22, 2001		V0.1
//	Jul 09, 2001		V0.2
//	Aug	06, 2001		V0.4
// 
//
// For correct work under NAT, the latter must meet the recommendations
// Microsoft companies to broadcast UDP packets through the gateway

#define _COOL_
//#define CC_DEBUG

#ifdef CC_DEBUG
#define _log_message(message) DebugMessage(message)
#else
#define _log_message(message) 
#endif //CC_DEBUG

#ifndef _COMM_CORE_H_INCLUDED_
#define _COMM_CORE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <assert.h>

#ifdef CC_DEBUG
#include <stdio.h>
#endif //CC_DEBUG

#pragma warning (disable : 4200)

#pragma pack(1)

// ---------------------------------------------------------------------------------------------

typedef in_addr PEER_ADDR;
typedef u_short	PEER_PORT;
typedef u_short	PEER_ID,*LPPEER_ID;
	
// ---------------------------------------------------------------------------------------------
// Constants
#define PROTO_ID				0x47534370	// Protocol Identifier ('GSCp')
#define DATA_PORT				34000		// Data port (34000)
#define RETRY_COUNT				8			// How many times do you need to ask to send the package?
											// in case of non-confirmation
											// after which the package is removed from the queue
#define RETRY_TIME				2800		// The packet resending interval in milliseconds
#define MAX_QUEUE_LEN			4096		// The maximum length of the packet queue
											// awaiting confirmation
#define MAX_PEERS				7			// The maximum number of hosts
#define CC_PROTO_VERSION		0x03		// Communication protocol version
#define MAX_PACKET_STAMP		0xEE6B2800	// Maximum package number (used for identification)
#define RECV_BUFFER_LENGTH		8192		// The size of the receiving buffer (8 kilobytes)
#define MAX_HOST_NAME			32			// The maximum length of the host name

// ---------------------------------------------------------------------------------------------
// Description of the host
typedef struct PEER_ENTRY{						// The host
	PEER_ADDR	m_ex_Addr;						// External host (or NAT) address
	PEER_PORT	m_ex_Port;						// External port of the host (or NAT)
	PEER_ID		m_Id;							// The host ID in the server list
	BOOL		m_bAlive;						// Is the host currently available?
	BOOL		m_bOverNAT;						// Is the host located behind NAT in relation to the server
	u_short		m_uLatency;						// Host response time
	CHAR		m_szUserName[MAX_HOST_NAME];	// The name of the host / user
	u_short		m_uUserDataSize;				// User data size
	LPBYTE		m_lpbUserData;					// Pointer to user data
	CHAR		m_szCCUID[23];					// Global host identifier
} *LPPEER_ENTRY;

// ---------------------------------------------------------------------------------------------
// Types of packages (indicated in the header)
#define CC_PT_FRAME_CONFIRM		0x01	// The host confirms receipt of the data packet
#define CC_PT_HOST_ALIVE		0x02	// The host confirms that it is alive; Latency measurement
#define CC_PT_SEND_DATA			0x03	// Package of user data without confirmation
#define CC_PT_SEND_DATA_SECURE	0x04	// Package of user data with confirmation
#define CC_PT_CHCK_HOST			0x05	// Host Availability Request Package; Latency measurement
#define CC_PT_TRY_CONNECT		0x06	// Trying to connect to the server
#define CC_PT_CONNECT_OK		0x07	// Permission to connect to the server
#define CC_PT_CONNECT_REJECT	0x08	// Failure to connect to the server
#define CC_PT_SERVER_LIST		0x09	// Sending a list of hosts (from the server)
#define CC_PT_HOST_EXIT			0x0A	// The host exits the game
#define CC_PT_HOST_DROP			0x0B	// The host is kicked out of the game (from the server)
#define CC_PT_CONNECT_DIFF		0x0C	// The packet comes to the host from the second host, if it knows who the server is
#define CC_PT_HOST_DROP_OK		0x0D	// The server confirms the disconnection of the host

#define CC_PT_SEND_USER_NAME	0x0E	// The user sends his name
#define CC_PT_SEND_USER_DATA	0x0F	// The user sends his data

#define CC_PT_SEND_NEW_NAME		0x10	// The server sends data about the changed user name
#define CC_PT_SEND_NEW_DATA		0x11	// The server sends data about changed user data

//
// ---------------------------------------------------------------------------------------------
enum ConnectState{
	csNone,csWait,csRejected,csConnected,csTimedOut,csBadProto,csSynch
};

enum SessionState{
	ssNone,ssOpen,ssClosed
};

// ---------------------------------------------------------------------------------------------
// Errors
#define	BAD_PEER_ID				255		// Invalid host identifier
#define CE_NOT_SERVER			0x01	// Attempting to connect to a non-server
#define CE_NO_ROOM				0x02	// There are no free connection slots
#define CE_NO_ERROR				0x03	// There are no errors
#define CE_BAD_VERSION			0x04	// Invalid protocol version
#define CE_BAD_MSG_SIZE			0x05	// Invalid message length
#define CE_SESSION_CLOSED		0x06	// It is not possible to connect the host, because the session is already closed

// ---------------------------------------------------------------------------------------------
// Packet headers

// General view of the frame for the protocol
typedef struct CC_PK_RAW_FRAME{
	u_long	m_lProto;				// Protocol type
	u_short	m_uType;				// Package type
	u_long	m_lStamp;				// Unique package number; 0 if no confirmation is required
	PEER_ID	m_PeerId;				// Sender ID
// ----------------------------------
	BYTE	m_bData[];				// Package-specific data of variable length
} *LPCC_PK_RAW_FRAME;

// CC_PT_FRAME_CONFIRM
typedef struct CC_PK_FRAME_CONFIRM{
	u_long	m_lConfirmStamp;		// Unique package number confirming receipt
} *LPCC_PK_FRAME_CONFIRM;

// CC_PT_SEND_DATA
typedef struct CC_PK_SEND_DATA{
	u_short	m_uSize;				// The size of the data field
	BYTE	m_bData[];				// Variable length data field
} *LPCC_PK_SEND_DATA;

// CC_PT_TRY_CONNECT
typedef struct CC_PK_TRY_CONNECT{
	CHAR		m_cProtoVersion;	// Version number of the protocol / application
	CHAR		m_szUserName[MAX_HOST_NAME];
	CHAR		m_szCCUID[23];
	u_short		m_uAddrCount;
	DWORD		m_dwAddrList[];
} *LPCC_PK_TRY_CONNECT;

// CC_PT_CONNECT_OK
typedef struct CC_PK_CONNECT_OK{
	PEER_ID		m_Id;							// The host number in the server list
	CHAR		m_szSessionName[MAX_HOST_NAME];	// Session name
	DWORD		m_dwOptions;
} *LPCC_PK_CONNECT_OK;

// CC_PT_CONNECT_REJECT
typedef struct CC_PK_CONNECT_REJECT{
	u_short		m_uReason;			// Reason for refusal
} *LPCC_PK_CONNECT_REJECT;

// CC_PT_SERVER_LIST
typedef struct CC_PK_SERVER_LIST{
	CHAR		m_szSessionName[MAX_HOST_NAME];	// Session name
	u_short		m_uCount;						// Number of hosts
	BYTE		m_PeerList[];					// List of variable hosts + dynamic data :)
} *LPCC_PK_SERVER_LIST;

// CC_PT_CHCK_HOST
typedef struct CC_PK_CHCK_HOST{
	DWORD		m_dwTickCount;		// The current number of ticks
} *LPCC_PK_CHCK_HOST;

// CC_PT_HOST_ALIVE
typedef struct CC_PK_HOST_ALIVE{
	DWORD		m_dwTickCount;		// The number of ticks at the time of sending the package (original)
} *LPCC_PK_HOST_ALIVE;

// CC_PT_HOST_EXIT
typedef struct CC_PK_HOST_EXIT{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_EXIT;

// CC_PT_HOST_DROP
typedef struct CC_PK_HOST_DROP{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_DROP;

// CC_PT_HOST_DROP_OK
typedef struct CC_PK_HOST_DROP_OK{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_DROP_OK;

// CC_PT_SEND_USER_NAME
typedef struct CC_PK_SEND_USER_NAME{
	CHAR		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_USER_NAME;

// CC_PT_SEND_USER_DATA
typedef struct CC_PK_SEND_USER_DATA{
	u_short		m_uUserDataSize;
	BYTE		m_UserData[];
} *LPCC_PK_SEND_USER_DATA;

// CC_PT_SEND_NEW_NAME
typedef struct CC_PK_SEND_NEW_NAME{
	PEER_ID		m_PeerId;
	CHAR		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_NEW_NAME;

// CC_PT_SEND_NEW_DATA
typedef struct CC_PK_SEND_NEW_DATA{
	PEER_ID		m_PeerId;
	u_short		m_uUserDataSize;
	BYTE		m_UserData[];
} *LPCC_PK_SEND_NEW_DATA;

// ---------------------------------------------------------------------------------------------
// Description of the package in the queue of messages awaiting confirmation
typedef struct FRAME_ENTRY{						// Package
	LPCC_PK_RAW_FRAME			m_lpFrame;		// A pointer to a package
	u_short						m_uSize;		// Package size
	PEER_ADDR					m_PeerAddr;		// Host address
	PEER_PORT					m_PeerPort;		// Host port
	DWORD						m_dwSendTime;	// The time of the last sending of the package
												// in milliseconds
	u_short						m_uRetrCount;	// The number of requests to send a package
} *LPFRAME_ENTRY;

// Description of callback functions
typedef BOOL (CALLBACK* LP_CC_IDLE_PROC)();
typedef BOOL (CALLBACK* LP_CC_ENUM_PROC)(const PEER_ID PeerID, LPCSTR lpcszPeerName);

// ---------------------------------------------------------------------------------------------
// Communication core class
class CCommCore  
{
// ---------------------------------------------------------------------------------------------
public:
// ---------------------------------------------------------------------------------------------
	LP_CC_IDLE_PROC	lpIdleProc;				// The callback function is called by the kernel at the time
											// work of blocking functions
	LP_CC_ENUM_PROC	lpEnumProc;				// Callback function for enumeration of hosts
// ---------------------------------------------------------------------------------------------
											// Send a message to the selected host
	BOOL SendToPeer	(PEER_ID piNumber, LPBYTE lpbBuffer, u_short u_Size, BOOL bSecure=FALSE);
											// Send a message to all hosts
	BOOL SendToAll	(LPBYTE lpbBuffer, u_short u_Size, BOOL bSecure=FALSE);
											
	u_short ReceiveData	(LPBYTE lpbBuffer, LPPEER_ID lpPeerId=NULL);	// Retrieves one packet from the queue
											// should be called often to avoid buffer overflow
	BOOL SendDropClient(PEER_ID PeerID);	// The server asks the client to log out
	BOOL InitClient	(LPCSTR lpcszServerIP, LPCSTR lpcszUserName);	// IP addresses in the form of a period with dots aaa.bbb.ccc.ddd
	BOOL DoneClient ();						// The client sends a request to remove the server
	BOOL DoneServer ();						// Shuts down the server by removing all hosts
	BOOL DeletePeer (PEER_ID piNumber);		// Removes a host from the list of hosts
	BOOL InitServer	(LPCSTR lpcszSessionName, LPCSTR lpcszUserName);	// Initializes the server
	BOOL QueueProcess();					// The packet sending queue is processed
											// Must call often

	BOOL SendServerList();					// Sends a list of hosts


	BOOL	IsOverNAT(PEER_ID PeerId);	


	LPCSTR	GetUserName(PEER_ID PeerId);	
	BOOL	SetUserName(LPCSTR lpcszUserName);	
	BOOL	SendUserName();

	BOOL	SetSessionName(LPCSTR lpcszSessionName);

	BOOL	GetUserData(PEER_ID PeerId, LPBYTE lpbUserData, u_short * puUserDataSize);
	BOOL	SetUserData(const LPBYTE lpcbUserData, u_short uUserDataSize);
	BOOL	SendUserData();

    BOOL	SendUdpHolePunch(sockaddr* server, char* content, const int content_len);

	VOID	GetServerAddress(LPSTR lpszServerAddress);

	BOOL	EnumPeers();

	BOOL	InitNetwork();					// Initialization of the network interface
	BOOL	CloseNetwork();					// Closing the network interface

	CCommCore		();
	virtual ~CCommCore();

	PEER_ID	GetPeerID()						{	return m_piNumber;		}

	u_short GetPeersCount()					{	return m_uPeerCount;	}
	LPCSTR	GetSessionName()				{	return m_szSessionName;	}

	u_short	GetMaxPeers()					{	return m_uMaxPeers;		}
	VOID	SetMaxPeers(u_short uMaxPeers)	{	m_uMaxPeers=uMaxPeers;	}

	VOID	CloseSession()					{	m_ssState=ssClosed;		}

	BOOL	IsClient()						{	return !m_bServer;		}
	BOOL	IsServer()						{	return m_bServer;		}

	VOID	SetOptions(DWORD dwOptions)		{	m_dwOptions=dwOptions;	}
	DWORD	GetOptions()					{	return m_dwOptions;		}

	// How many bytes received
	DWORD	GetRxBytes()					{	return m_dwRxBytes;		}
	// How many bytes sent
	DWORD	GetTxBytes()					{	return m_dwTxBytes;		}
	// How many bytes forwarded
	DWORD	GetNxBytes()					{	return m_dwNxBytes;		}
	// How many milliseconds ago was the last packet received
	DWORD	GetRecvTimeOut()				{	return (GetTickCount()-m_dwLastPacketTime);	}

// ---------------------------------------------------------------------------------------------
protected:
// ---------------------------------------------------------------------------------------------
#ifdef CC_DEBUG
	FILE	*	m_DebugStream;	
	VOID		DebugMessage(LPCSTR lpcszMessage);
#endif //CC_DEBUG
	
	CHAR		m_szUserName[MAX_HOST_NAME];		//
//	PEER_ADDR	m_paHostAddr;						//
//	CHAR		m_szDotAddr[18];					//
	PEER_ADDR	m_paServAddr;						//

	DWORD		m_dwAddrList[8];					// List of local machine addresses in network order format
	u_short		m_uAddrCount;						// The number of local machine addresses

	DWORD		m_dwLastPacketTime;

	u_short		m_uMaxPeers;
	BOOL		m_bOverNAT;

	DWORD		m_dwRxBytes;
	DWORD		m_dwTxBytes;
	DWORD		m_dwNxBytes;

	DWORD		m_dwOptions;

	CHAR		m_szSessionName[MAX_HOST_NAME];		//

	CHAR		m_szCCUID[23];

	LPBYTE		m_lpbRecvBuffer;					//

	SessionState	m_ssState;						//
	ConnectState	m_csState;						//
	BOOL			m_bBlockingCall;				// They are only processed in blocking mode
													// confirmation packets

	u_short		m_uRejectReason;					//

	u_short		m_uMaxMsgSize;						//

	SOCKET		m_DataSocket;						// A socket serving user messages

	u_long		m_lStamp;							// Auto-incremental packet counter

	BOOL		m_bServer;							// Is the host a server?
	PEER_ID		m_piNumber;							// The host ID in the server list
													// for the server is always 0x01
	PEER_ID		m_piAutoInc;						// half-incremental counter of hosts

	LPBYTE		m_lpbUserData;						// Dynamic user data
	u_short		m_uUserDataSize;					// Size of dynamic user data

	PEER_ENTRY	m_PeerList[MAX_PEERS];				// List of available hosts
	u_short		m_uPeerCount;						// Number of available hosts

	FRAME_ENTRY m_FrameList[MAX_QUEUE_LEN];			// Queue of messages awaiting confirmation
	u_short		m_uFrameCount;						// The number of messages in the queue

// ---------------------------------------------------------------------------------------------

//	PEER_ID		GetIdBySender();					// Returns the logical number of the host
													// To his address/port
	u_short GetPeerById(PEER_ID PeerId);			//
	u_short GetPeerByCCUID(LPCSTR lpcszCCUID);
	BOOL	DropPeer(u_short uPeer);					//

	BOOL InitSocket();
	BOOL CloseSocket();
	BOOL InitHost();

	BOOL QueueAddPacket(	PEER_ADDR			PeerAddr, 
							PEER_PORT			PeerPort, 
							LPCC_PK_RAW_FRAME	lpRawFrame,
							u_short				uSize);

	BOOL SendRawPacket(		PEER_ADDR			PeerAddr,				// Host address
							PEER_PORT			PeerPort,				// Host port
							u_short				uType,					// Package type
							LPBYTE				lpbBuffer,				// Buffer with data
							u_short				uSize,					// Data size in bytes
							BOOL				bSecureMessage,			// TRUE if pseudo-guaranteed data delivery is required
							BOOL				bWaitForCompletion);	// TRUE if the blocking call
																		// that. wait for confirmation or timeout


	BOOL ProcessServerList(LPCC_PK_SERVER_LIST lpServerList);
	
	BOOL ReSendFrame( u_short uFrameNum);
	VOID Cleanup();

	BOOL QueueDropPacket(int iFrameNum);
	BOOL QueueDropConfirmedPacket(u_long lStamp);
	BOOL QueueClearAll();
	BOOL QueuePacketExists(u_long lStamp);

	BOOL SendConfirmDataPacket(sockaddr_in * lpSender, u_long lStamp);
	BOOL SendConnectReject(sockaddr_in *lpSender, u_short uReason);
	BOOL SendConnectOk(sockaddr_in *lpSender, PEER_ID PeerId);
	BOOL SendDropOk(sockaddr_in *lpSender);

	BOOL SendNewName(PEER_ID PeerId);		// Sends information about the name (server)
	BOOL SendNewData(PEER_ID PeerId);		// Sends date information (server)

	VOID SetCommCoreUID(LPCSTR lpcszCCUID);
	VOID NewCommCoreUID(LPSTR lpszCCUID);
	VOID GetCommCoreUID(LPSTR lpszCCUID);

// ---------------------------------------------------------------------------------------------
};

#pragma warning (default : 4200)

#endif // _COMM_CORE_H_INCLUDED_
