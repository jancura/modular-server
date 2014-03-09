
#ifndef _HANDLER_MODULAR_SERVER_1_0 
#define _HANDLER_MODULAR_SERVER_1_0

typedef void    (* tPushMsgHandler )(char* Sender, char* Receiver, DWORD Code, const char* Message); 

typedef void    (* tCommandHandler)(int clients[], int iSize, SOCKET socket, int Id, tPushMsgHandler fPushMessage, char * pParameters);
typedef size_t  (* tGetNameHandler)(char * pName, size_t iSize);
typedef int			(* tGetCodeHandler)();
typedef void		(* tFunctionHandler)();
typedef void		(* tSendMsgHandler)(tPushMsgHandler fPushMessage);
typedef void		(* tProcessCommand)(const char* Sender, const char* Receiver, int clients[], int iSize, SOCKET socket, int Code, const char* Data);
typedef void		(* tProcessHandler)(const char* Sender, int iLenSend, const char* Receiver, int iLenRecv, int Code, const char* Data, int iLenData);

typedef tCommandHandler		CommandHandler;
typedef tGetNameHandler		GetNameHandler;
typedef tFunctionHandler	FunctionHandler;
typedef tSendMsgHandler		SendMsgHandler;
typedef tProcessHandler		ProcessHandler;
typedef tProcessCommand		ProcCmdHandler;
typedef tGetCodeHandler		GetCodeHandler;

#endif