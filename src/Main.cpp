#include <iostream>
#include <map>
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <fstream>

#include "utils.h"
#include "rpc.h"
using namespace std;



void foreach_process(std::function<void(int)> proc) {
	PROCESSENTRY32 process;
	BOOL ret;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	ret = Process32First(snapshot, &process);
	do {
		proc(process.th32ProcessID);
		ret = Process32Next(snapshot, &process);
	} while (ret == TRUE);
}

int main(int argc, char** argv)
{
	foreach_process([](int procId) {


		DWORD interfaceCount = 0;
		DWORD functionCount = 0;
		HANDLE hProcess;
		char InterfaceId[0x1000], *outstr;
		char tmpstr[0x1000];
		MIDL_SERVER_INFO_T midlServerInfo;
		MIDL_STUB_DESC_T midlStubDesc;

		Process* p = new Process(procId);
		if (p->Open() == 0) {
			return;
		}

		RPC rpcHunter(p);
		RPC_SERVER_T* rpcServer = rpcHunter.GetGlobalRPCServer();
		if (rpcServer == NULL) {
			return;
		}

		RPC_INTERFACE_T* rpcInterfaces = rpcHunter.GetInterfaces(rpcServer, &interfaceCount);
		if (rpcInterfaces == NULL || interfaceCount == 0) {
			return;
		}
		printf("PID : %x\n", procId);
		for (DWORD i = 0; i < interfaceCount; i++) {

			//Utils::PrintGUID(rpcInterfaces[i].RpcServerInterface.InterfaceId.Uuid);
			memcpy(InterfaceId, Utils::ConvertGUID(rpcInterfaces[i].RpcServerInterface.InterfaceId.Uuid).c_str(), Utils::ConvertGUID(rpcInterfaces[i].RpcServerInterface.InterfaceId.Uuid).length());

			void** functions = rpcHunter.GetMethodForInterface(&rpcInterfaces[i], &functionCount);
			if (functions == NULL) {
				continue;
			}

			// infos de l'interface
			printf(	"\t%s\n"
					"\t\tRaw flags : %x \n"
					"\t\tRPC_IF_OLE : %d \n"
					"\t\tRPC_IF_ALLOW_LOCAL_ONLY : %d\n"
					"\t\tRPC_IF_ALLOW_SECURE_ONLY : %d\n"
					"\t\tRPC_IF_SEC_NO_CACHE : %d\n"
					"\t\tRPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH : %d\n"
					"\t\tSecurity callback addr : %p \n"
					"\t\tEPMapper flags : %x\n",
				InterfaceId,
				rpcInterfaces[i].Flags,
				(rpcInterfaces[i].Flags & RPC_IF_OLE) ? 1 : 0,
				(rpcInterfaces[i].Flags & RPC_IF_ALLOW_LOCAL_ONLY) ? 1 : 0,
				(rpcInterfaces[i].Flags & RPC_IF_ALLOW_SECURE_ONLY) ? 1 : 0,
				(rpcInterfaces[i].Flags & RPC_IF_SEC_NO_CACHE) ? 1 : 0,
				(rpcInterfaces[i].Flags & RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH) ? 1 : 0,
				rpcInterfaces[i].IfCallbackFn,
				rpcInterfaces[i].EpMapperFlags
				);


			// infos MIDL et cie
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
			if (hProcess != INVALID_HANDLE_VALUE) {
				SIZE_T dwRead;
				if (ReadProcessMemory(hProcess, rpcInterfaces[i].RpcServerInterface.InterpreterInfo, &midlServerInfo, sizeof(midlServerInfo), &dwRead) && dwRead == sizeof(midlServerInfo)) {

					if (ReadProcessMemory(hProcess, midlServerInfo.pStubDesc, &midlStubDesc, sizeof(midlStubDesc), &dwRead) && dwRead == sizeof(midlStubDesc)) {

						printf("\t\tMIDL Stub Version : %x\n"
							"\t\tMIDLVersion : %x\n"
							"\t\tMIDL Flags : %x\n",
							midlStubDesc.Version,
							midlStubDesc.MIDLVersion,
							midlStubDesc.mFlags);

						if (midlStubDesc.MIDLVersion != NULL && ReadProcessMemory(hProcess, midlStubDesc.pFormatTypes, &tmpstr, 0x260, &dwRead)) {
							printf("\t\tMIDL FormatTypes : %s\n", tmpstr);
						}
						if (midlServerInfo.ProcString && ReadProcessMemory(hProcess, midlServerInfo.ProcString, &tmpstr, 0x260, &dwRead)) {
							printf("\t\tMIDLSRV ProcString : %s\n", tmpstr);
						}

					}
				}
				CloseHandle(hProcess);
			}

			printf("\t\tFunctions:\n");

			for (DWORD j = 0; j < functionCount; j++) {

				DllInfo* info = p->GetModuleByAddress(functions[j]);
				Debug debug(p->GetPid());

				string* modname = p->GetModuleFilename(info->base);

				string* out = debug.GetSymbolForAddresss(modname->c_str(), info->base,(DWORD) info->length, functions[j] );
				outstr = "???";
				if (out != NULL) {
					outstr = (char*)out->c_str();
				}

				printf("\t\t\tModule : %s \n"
					"\t\t\tFunction name : %s \n"
					"\t\t\tFunction offset : %p \n",
					modname->c_str(),
					outstr,
					(size_t)functions[j] - (size_t)info->base
					);
				

			}


		}


	});

}
