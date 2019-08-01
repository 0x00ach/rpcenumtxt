#pragma once
#include <Windows.h>

#define MAX_SIMPLE_DICT_ENTRIES			0x200

#define ULONG_PTR_T ULONG_PTR
#define PTR_T *

#define MAX_RPC_INTERFACE_ANNOTATION			64
#define SIMPLE_DICT_SMALL_ARRAY					4


#define RPC_CORE_DESCRIPTION					"Windows 10 64bits runtime core"
#define RPC_CORE_IS_WOW64						FALSE

#define ULONG_PTR_T ULONG_PTR
#define PTR_T *


#define MAX_RPC_INTERFACE_ANNOTATION			64
#define SIMPLE_DICT_SMALL_ARRAY					4


//==============================================================================
// From Winnt.h
// The following structures are redefined to support Wow64 ptr
//
struct _RTL_CRITICAL_SECTION_T;

typedef struct _LIST_ENTRY_T {
	struct _LIST_ENTRY PTR_T Flink;
	struct _LIST_ENTRY PTR_T Blink;
} LIST_ENTRY_T, * PLIST_ENTRY_T;


typedef struct _RTL_CRITICAL_SECTION_DEBUG_T {
	WORD   									Type;
	WORD   									CreatorBackTraceIndex;
	struct _RTL_CRITICAL_SECTION_T PTR_T	CriticalSection;
	LIST_ENTRY_T							ProcessLocksList;
	DWORD 									EntryCount;
	DWORD 									ContentionCount;
	DWORD 									Flags;
	WORD   									CreatorBackTraceIndexHigh;
	WORD   									SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG_T, PTR_T PRTL_CRITICAL_SECTION_DEBUG_T;

typedef struct _RTL_CRITICAL_SECTION_T {
	PRTL_CRITICAL_SECTION_DEBUG_T	DebugInfo;
	//
	//  The following three fields control entering and exiting the critical
	//  section for the resource
	//
	LONG 						LockCount;
	LONG 						RecursionCount;
	VOID PTR_T					OwningThread;        // from the thread's ClientId->UniqueThread
	VOID PTR_T					LockSemaphore;
	VOID PTR_T					SpinCount;			// force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION_T, PTR_T PRTL_CRITICAL_SECTION_T;

//==============================================================================
// From RpcDceP.h
//
typedef struct _RPC_DISPATCH_TABLE_T {
	UINT							DispatchTableCount;
	RPC_DISPATCH_FUNCTION  PTR_T	DispatchTable;
	ULONG_PTR_T                      Reserved;
} RPC_DISPATCH_TABLE_T, PTR_T PRPC_DISPATCH_TABLE_T;

typedef struct _RPC_PROTSEQ_ENDPOINT_T {
	UCHAR PTR_T RpcProtocolSequence;
	UCHAR PTR_T Endpoint;
} RPC_PROTSEQ_ENDPOINT_T, PTR_T PRPC_PROTSEQ_ENDPOINT_T;

typedef struct _RPC_SERVER_INTERFACE_T {
	UINT					Length;
	RPC_IF_ID				InterfaceId;
	RPC_IF_ID				TransferSyntax;
	PRPC_DISPATCH_TABLE_T	DispatchTable;
	UINT					RpcProtseqEndpointCount;
	PRPC_PROTSEQ_ENDPOINT_T RpcProtseqEndpoint;
	RPC_MGR_EPV PTR_T		DefaultManagerEpv;
	void const PTR_T		InterpreterInfo;
	UINT					Flags;
} RPC_SERVER_INTERFACE_T, PTR_T PRPC_SERVER_INTERFACE_T;


typedef struct _NDR_EXPR_DESC_T
{
	const unsigned short PTR_T	pOffset;
	const unsigned char	PTR_T	pFormatExpr;
} NDR_EXPR_DESC_T;


/*
 * MIDL Stub Descriptor
 */
typedef struct _MIDL_STUB_DESC_T {
	void  PTR_T						RpcInterfaceInformation;
	void  PTR_T						pfnAllocate;
	void  PTR_T						pfnFree;
	void  PTR_T						pAutoHandle;
	const VOID  PTR_T				apfnNdrRundownRoutines;
	const VOID  PTR_T				aGenericBindingRoutinePairs;
	const VOID  PTR_T				apfnExprEval;
	const VOID  PTR_T				aXmitQuintuple;
	const unsigned char  PTR_T		pFormatTypes;
	int								fCheckBounds;
	/* Ndr library version. */
	unsigned long					Version;
	VOID PTR_T						pMallocFreeStruct;
	long							MIDLVersion;
	const COMM_FAULT_OFFSETS  PTR_T	CommFaultOffsets;
	// New fields for version 3.0+
	const VOID PTR_T				aUserMarshalQuadruple;
	// Notify routines - added for NT5, MIDL 5.0
	const VOID PTR_T				NotifyRoutineTable;
	/*
	 * Reserved for future use.
	 */
	ULONG_PTR_T						mFlags;
	// International support routines - added for 64bit post NT5
	const VOID	PTR_T				CsRoutineTables;
	void  PTR_T						ProxyServerInfo;
	const NDR_EXPR_DESC_T	PTR_T	pExprInfo;
	// Fields up to now present in win2000 release.
} MIDL_STUB_DESC_T, PTR_T PMIDL_STUB_DESC_T;


/*
 * Server Interpreter's information strucuture.
 */
typedef struct  _MIDL_SERVER_INFO_T {
	PMIDL_STUB_DESC_T				pStubDesc;
	const VOID	PTR_T	PTR_T		DispatchTable;
	const unsigned char		PTR_T	ProcString;
	const unsigned short	PTR_T	FmtStringOffset;
	const VOID PTR_T PTR_T			ThunkTable;
	RPC_IF_ID PTR_T					pTransferSyntax;
	ULONG_PTR_T						nCount;
	VOID PTR_T						pSyntaxInfo;
} MIDL_SERVER_INFO_T, PTR_T PMIDL_SERVER_INFO_T;

//==============================================================================
// Common private structures from rpctr4.dll.
// These structures seems to be constant on all the runtime versions.
//
#pragma pack(1)
typedef struct _SIMPLE_DICT_T {
	VOID PTR_T PTR_T	pArray;
	UINT				ArraySizeInBytes;	//to change : countof array elements
	UINT				NumberOfEntries;
	VOID PTR_T			SmallArray[SIMPLE_DICT_SMALL_ARRAY];
}SIMPLE_DICT_T, PTR_T PSIMPLE_DICT_T;

typedef struct _QUEUE_T {
	VOID PTR_T	Tail;
	VOID PTR_T	Head;
	ULONG		Lentgh;
	VOID PTR_T	SmallArray[SIMPLE_DICT_SMALL_ARRAY];
}QUEUE_T;

typedef struct _MUTEX_T {
	RTL_CRITICAL_SECTION_T	CriticalSection;
}MUTEX_T;

typedef struct _EVENT_T {
	ULONG	hEvent;
} EVENT_T;

#pragma pack()

#define RPC_ADDRESS_TYPE_DG		0x400000
#define RPC_ADDRESS_TYPE_LRPC	0x800000
#define RPC_ADDRESS_TYPE_OSF	0x800

#pragma pack(1)
typedef struct _RPC_SERVER_T {
	MUTEX_T						Mutex;
	ULONG							__bIslistening;
	ULONG							bIsListening;
	ULONG							MinimumCallThreads;
	ULONG							Wait;
	ULONG							OutCalls;
	ULONG							Unk1;
	ULONG							InCalls;
	ULONG							Unk2;
	SIMPLE_DICT_T					AddressDict;
	ULONG							lAvailableCalls;
	ULONG							Unk3;
	SIMPLE_DICT_T					_ProtSeqQueue;
	ULONG							Unk4[8];
	ULONG							OutPackets;
	ULONG							Unk5;
	MUTEX_T						Mutex2;
	ULONG							MaxCalls;
	ULONG							Unk6;
	VOID PTR_T					hEvent;
	ULONG							Unk7[4];
	SIMPLE_DICT_T					InterfaceDict;
	ULONG							_bIsListening;
	ULONG							bIsMaxCalls1234;
	ULONG							Unk8[6];
	ULONG							InPackets;
	ULONG							Unk9;
	RPC_FORWARD_FUNCTION PTR_T	pRpcForwardFunction;
	ULONG							Unk10[6];
	SIMPLE_DICT_T					AuthenInfoDict;
	LIST_ENTRY_T					RpcIfGroupListEntry;
	ULONG	PTR_T					__SRWLock;
	LIST_ENTRY_T					field_1E0;
}RPC_SERVER_T, PTR_T PRPC_SERVER_T;

typedef struct _RPC_INTERFACE_T
{
	PRPC_SERVER_T				pRpcServer;
	ULONG						Flags;
	ULONG						Unk1;
	MUTEX_T					Mutex;
	ULONG						EpMapperFlags;
	ULONG						Unk2;
	RPC_MGR_EPV PTR_T			pMgrEpv;
	RPC_IF_CALLBACK_FN PTR_T	IfCallbackFn;
	RPC_SERVER_INTERFACE_T	RpcServerInterface;
	PMIDL_SYNTAX_INFO			pSyntaxInfo;
	VOID PTR_T				pTransfertSyntaxes;
	ULONG						TransfertSyntaxesCount;
	ULONG						__Field_C4;
	ULONG						NbTypeManager;
	ULONG						MaxRpcSize;
	UUID_VECTOR  PTR_T		pUuidVector;
	SIMPLE_DICT_T				RpcInterfaceManagerDict;
	UCHAR						Annotation[MAX_RPC_INTERFACE_ANNOTATION];
	ULONG						IsCallSizeLimitReached;
	ULONG						currentNullManagerCalls;
	ULONG						__Field_150;
	ULONG						__Field_154;
	ULONG						__Field_158;
	ULONG						SecurityCallbackInProgress;
	ULONG						SecurityCacheEntry;
	ULONG						field_164;
	VOID PTR_T				__SecurityCacheEntries[16];
	SIMPLE_DICT_T				FwEpDict;
	ULONG						Unk3[6];
	struct RPCP_INTERFACE_GROUP PTR_T pRpcpInterfaceGroup;
}RPC_INTERFACE_T, PTR_T PRPC_INTERFACE_T;

#define RPC_ADDRESS_TYPE_DG		0x400000
#define RPC_ADDRESS_TYPE_LRPC	0x800000
#define RPC_ADDRESS_TYPE_OSF	0x800

typedef struct _RPC_ADDRESS_T {
	VOID PTR_T				pVtable;
	ULONG					Magic;
	ULONG					TypeOfAddress;
	ULONG					ReferenceCounter;
	ULONG					Unk1[3];
	WCHAR PTR_T				Name;
	WCHAR PTR_T				Protocole;
	WCHAR PTR_T				Address;
	ULONG					bNamed;
	ULONG					EpAddAddressFlag;
	SIMPLE_DICT_T			__LRPCSassociationSimpleDict;
	ULONG					__Field_68;
	ULONG					Unk3;
	ULONG					NbActiveCalls;
	ULONG					__Field_74;
	ULONG					Unk4[6];
	ULONG					__Field_90;
	MUTEX_T					Mutex;
}RPC_ADDRESS_T;

#pragma pack()



#define RPC_CORE_EXPORT_SYMBOL			"RpcCoreHelper"
#define MAX_RPC_INTERFACE_ANNOTATION	64
#define MAX_GUID						40
#define MAX_CLSID_NAME					128



////////////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////////////

#define RPC_PROCESS_INFO_DEFAULT	0x0
#define	RPC_PROCESS_INFO_MISC		0x1
#define	RPC_PROCESS_INFO_RPC		0x2
#define	RPC_PROCESS_INFO_ALL		(RPC_PROCESS_INFO_MISC | RPC_PROCESS_INFO_RPC)

typedef enum _RpcProcessType_T{
	RpcProcessType_UNKNOWN = 0,
	RpcProcessType_RPC,
	RpcProcessType_DCOM,
	RpcProcessType_HYBRID
}RpcProcessType_T;

typedef struct _RpcProcessInfo_T{
	//Default
	DWORD				Pid;
	DWORD				ParentPid;
#ifdef _WIN64
	BOOL				bIsWow64;
#endif
	//Misc
	WCHAR				Name[MAX_PATH];
	WCHAR				Path[MAX_PATH];
	WCHAR				CmdLine[MAX_PATH];
	WCHAR				User[MAX_PATH];
	WCHAR				Desktop[MAX_PATH];
	DWORD				Session;
	UINT64				Version;
	HICON				hIcon;
	WCHAR				Description[MAX_PATH];
	FILETIME			CreationTime;
	//Rpc
	RpcProcessType_T	RpcProcessType;
	DWORD				MaxCalls;
	BOOL				bIsServer;
	BOOL				bIsListening;
	UINT				InterfacesCount;
	UINT				EndpointsCount;
	UINT				SspCount;
	DWORD				InCalls;
	DWORD				OutCalls;
	DWORD				InPackets;
	DWORD				OutPackets;
	//...
}RpcProcessInfo_T;


typedef struct _NdrInfo_T{
	ULONG		Version;		// NDR version required for the stub.
	ULONG		MIDLVersion;	// Version of the MIDL compiler used to compile the .idl file
	ULONG_PTR	mFlags;			// Flag describing the attributes of the stub (RPCFLG_HAS_MULTI_SYNTAXES, RPCFLG_HAS_CALLBACK, or RPC_INTERFACE_HAS_PIPES)
}NdrInfo_T;


typedef enum _TypeOfStub_T{
	TypeOfStub_Unknown = 0,
	TypeOfStub_Interpreted,
	TypeOfStub_Inlined,
	TypeOfStub_TypeLib,
	TypeOfStub_Hybrid
}TypeOfStub_T;

typedef enum _IfType_T{
	IfType_Unknown = 0,
	IfType_RPC,
	IfType_DCOM,
	IfType_OLE,
}IfType_T;

#define RPC_INTERFACE_INFO_DEFAULT	0x0
#define RPC_INTERFACE_INFO_MISC		0x1
#define RPC_INTERFACE_INFO_RPC		0x2
#define RPC_INTERFACE_INFO_NDR		0x4
#define RPC_INTERFACE_INFO_DCOM		0x8
#define RPC_INTERFACE_INFO_ALL		(RPC_INTERFACE_INFO_MISC | RPC_INTERFACE_INFO_RPC | RPC_INTERFACE_INFO_NDR |RPC_INTERFACE_INFO_DCOM)


typedef struct _RpcInterfaceInfo_T{
	//Default
	DWORD					Pid;
#ifdef _WIN64
	BOOL					bWow64Process;
#endif
	RPC_IF_ID				If;
	RPC_SYNTAX_IDENTIFIER	TransfertSyntax;
	UINT					Flags;
	IfType_T				IfType;
	//Misc
	WCHAR					Name[MAX_PATH];
	WCHAR					Location[MAX_PATH];
	DWORD					LocationState;
	DWORD					LocationType;
	VOID*					pLocationBase;
	SIZE_T					LocationSize;
	WCHAR					Description[MAX_PATH];
	//RPC
	BOOL					bIsRegistered;									//EP mapper
	UCHAR					Annotation[MAX_RPC_INTERFACE_ANNOTATION];		//annotation is natively in UCHAR
	RPC_IF_CALLBACK_FN*		IfCallbackFn;									//security callback if any
	//NDR
	UINT					NumberOfProcedures;
	NdrInfo_T				NdrInfo;
	TypeOfStub_T			TypeOfStub;
	ULONG*					ppProcAddressTable;								//Table containing the address of each function
	USHORT*					pFormatStringOffsetTable;						//Table containing the offset of each function encoding
	UCHAR*					pProcFormatString;			// Midl server info
	UCHAR*					pTypeFormatString;			// Midl stub desc
	VOID*					apfnExprEval;				// Midl stub desc
	USHORT*					pExprOffset;				// Midl stub desc
	UCHAR*					pExprFormatString;			// Midl stub desc
	//DCOM information
	WCHAR					ProxyStubClsid32[MAX_GUID];
	WCHAR					TypeLib[MAX_GUID];
	WCHAR					TypeLibName[MAX_PATH];
	WCHAR					ClsidName[MAX_CLSID_NAME];
	WCHAR					InprocServer32[MAX_PATH];
	WCHAR					TypeLibVersion[20];
	WCHAR					TypeLibPath[MAX_PATH];
	//...
}RpcInterfaceInfo_T;



typedef ULONG	RVA_T;
#define RPC_MAX_LENGTH				260

typedef struct _RpcDecompilerInfo_T{
	UINT					Pid;
	RPC_IF_ID*				pIfId;
	UINT64					pModuleBase;
	UINT					NDRVersion;
	UINT					MIDLVersion;
	UINT					NDRFags;
	UINT					NumberOfProcedures;
	RVA_T*					ppProcAddressTable;			//A table containing the address of each function
	RVA_T*					ppDispatchProcAddressTable;
	USHORT*					pFormatStringOffsetTable;	//A table containing the FormatStringOffset of each function
	WCHAR**					ppProcNameTable;			//A table containing the name of each function if possible or a NULL pointer else
	RVA_T/*UCHAR* */		pTypeFormatString;			//Just a pointer to the type format string: must be read using 
	RVA_T/*UCHAR* */		pProcFormatString;
	//RVA_T /*VOID* */		pExprInfo;					// FC_EXPR : to be removed (replaced by following 2 elements)
	RVA_T					pExprOffset;
	RVA_T					pExprFormatString;
	RVA_T					apfnExprEval;				// Callbacks
	BOOL					bIs64Bits;
	WCHAR					InterfaceName[RPC_MAX_LENGTH];
	RPC_SYNTAX_IDENTIFIER*	pSyntaxId;
	BOOL					bIsInlined;
	RVA_T*					ppProcFormatInlined;
	BOOL*					pbFunctionInterpreted;		// array containing for every function a boolean indicating if function is interpreted
	//...
}RpcDecompilerInfo_T;

typedef VOID*	(__fastcall* RpcDecompilerInitFn_T)(PVOID pRpcViewHelper, RpcDecompilerInfo_T* pDecompilerInfo);	//returns NULL in case of failure
typedef VOID(__fastcall* RpcDecompilerUninitFn_T)(VOID* pRpcDecompilerCtxt);
typedef BOOL(__fastcall* RpcDecompilerPrintProcedureFn_T)(VOID* pRpcDecompilerCtxt, UINT ProcIndex);
typedef BOOL(__fastcall* RpcDecompilerPrintAllProceduresFn_T)(VOID* pRpcDecompilerCtxt);

typedef struct _RpcDecompilerHelper_T{
	RpcDecompilerInitFn_T				RpcDecompilerInitFn;
	RpcDecompilerUninitFn_T				RpcDecompilerUninitFn;
	RpcDecompilerPrintProcedureFn_T		RpcDecompilerPrintProcedureFn;
	RpcDecompilerPrintAllProceduresFn_T	RpcDecompilerPrintAllProceduresFn;
}RpcDecompilerHelper_T;
