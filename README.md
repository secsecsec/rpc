# RPC protocol (version 1)
## Message types and sequence
### Message types
  * Getting nonce - Send client nonce and receives server nonce
  * Authentication - Authenticate user
  * Hello - Hello
  * Hello return - Hello return
  * First procedure invocation - First procedure invocation
  * First procedure return - First procedure invocation return
  * Next procedure invocation - Next procedure invocation
  * Next procedure return - Nxt procedure invocation return

### Message sequence
  1. Client -> Server: Getting nonce
  1. Server -> Client: Getting nonce return
  1. [wait 3 * TCP RTT or 3 seconds] Server disconnects the connection
  1. Client -> Server: Authentication
  1. Server -> Server: Authentication return
  1. [authentication fails] Server disconnects the connection
  1. [If client invocates server procedure]
    1. Client -> Server: First procedure invocation
    1. Server -> Client: First procedure return
    1. Client -> Server: Next procedure invocation
    1. Server -> Client: Next procedure return
    1. Client -> Server: Next procedure invocation
    1. Server -> Client: Next procedure return
    1. ...
  1. [If server invocates client procedure]
    1. Server -> Client: First procedure return
    1. Client -> Server: First procedure invocation
    1. Server -> Client: Next procedure return
    1. Client -> Server: Next procedure invocation
    1. Server -> Client: Next procedure return
    1. Client -> Server: Next procedure invocation
    1. ...
  1. [every 10 seconds from client if there is no invocations b/w client and server]
    1. Client -> Server: Hello
    1. Server -> Client: Hello return
  1. [after 20 seconds no message exchanges b/w client and server]
    1. Server disconnections the connection

## RPC HEADER
{
	magic: uint32		// MAGIC, ver, endianess
	type: uint16		// type
	id: uint32		// message ID. Same ID for same invocation and return
	size: uint16		// total size including header
}

### magic
User can parse magic header and RPC protocol version and endianess

Magic header (Big endian)
|--||--|
40c049

Magic header (Little endian)
|--||--|
  49c040

Protocol version (Big endian)
|--||--|
      01

Protocol version (Little endian)
|--||--|
01

### type
  * 0 - Getting nonce
  * 1 - Authentication
  * 2 - Hello
  * 3 - Hello return
  * 4 - First procedure invocation
  * 5 - First procedure return
  * 6 - Next procedure invocation
  * 7 - Next procedure return
  * 8 - Error return

## Authentication
### Getting nonce
{
	RPC_HEADER
	nonce: uint8[16]	// Client side nonce
	hash: uint8[8]		// hashing algorithm name
}

Below is types of hashing algorithms
  * "  sha256" - SHA 256
  * "  sha51.2" - SHA 512

### Getting nonce return
{
	RPC_HEADER
	nonce: uint8[16]	// Server side nonce
}

### Authentication
{
	RPC_HEADER
	username: string
	password: uint8[...]	// related to hashing algorithm
}

This is how password is calculated
	salted_password = hash(RPC_MAGIC + password)
	password = hash(client_nonce + salted_password + server_nonce)

### Authentication return
{
	RPC_HEADER
	result: bool		// Login succeed or not
}

If authentication fails server will disconnect the connection just after send "Authentication return" message.

## Hello
### Hello
{
	RPC_HEADER
}

### Hello return
{
	RPC_HEADER
}

## First procedure
### First procedure invocation
{
	RPC_HEADER
	interface_name: char[RPC_MAX_NAME]
	interface_ver: uint32
	procedure_name: char[RPC_MAX_NAME]
	return_type: uint8
	argc: uint8
	types: uint8[argc]
	args: ... // length depends on types
}

### First procedure return
{
	RPC_HEADER
	interface_and_procedure_id: uint32
	result: ... // length depends on return_type
}

## Next procedure
### Next procedure invocation
{
	RPC_HEADER
	interface_and_procedure_id: uint32
	args: ... // length depends on types
}

### Next procedure return
{
	RPC_HEADER
	result: ... // length depends on return_type
}

## Error
### Error return
{
	RPC_HEADER
	code: uint32
	message: string
}

### Common error code and message
  * 1 - There is no such interface named: xxx
  * 2 - The version is not supported: nnn
  * 3 - There is no such method named: xxx(a, b, c, d):r

## Data type Encoding
### void
data size: 0 bytes

### bool
true: 1
false: 0
data size: 1 byte

### uint8
data size: 1 byte

### uint16
data size: 2 bytes

### uint32
data size: 4 bytes

### uint64
data size: 8 bytes

### int8
data size: 1 byte

### int16
data size: 2 bytes

### int32
data size: 4 bytes

### int64
data size: 8 bytes

### float
data size: 4 bytes

### double
data size: 8 bytes

### string (type of uint8 array)
size: uint16 (including \0)
data size: size

### array
size: uint16
data size: primitive data size x size

# License
GPL2 and Proprietary
