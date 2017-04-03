#include "rpc.h"

void rpc_init(RPC* rpc,
		int(*connect)(RPC* rpc, uint32_t addr, uint16_t port),
		int(*disconnect)(RPC* rpc),
		int(*send)(RPC* rpc, void* buf, int size),
		int(*receive)(RPC* rpc, void* buf, int size),
		int priv_size) {
	
	bzero(rpc, sizeof(RPC) + priv_size);
	rpc->connect = connect;
	rpc->disconnect = disconnect;
	rpc->send = send;
	rpc->receive = receive;
}

int rpc_add(RPC* rpc, PRC_Procedure* procedure) {
	// TODO: Implement it
	// 1. find empty space
	// 2. insert the procedure
	// 3. return the procedure index
	// 4. if no space, return error number
}

void* rpc_invoke(RPC* rpc, const char* name, ...) {
	// TODO: Implement it
	// 1. parse args
	// 2. Make a message
	// 3. Send the message
	// 4. Wait for a while
	//    If time out, set errno and return NULL
	// 5. Parse return value
	// 6. Return it
}
