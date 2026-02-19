# Native Module: Network

This module provides **TCP Socket** primitives to build network clients and servers directly in GW.
It uses **Winsock2** on Windows and standard **BSD Sockets** on Linux/MacOS.

## Exposed Primitive Functions

### Socket Management

#### `__native_socket_create() -> ID`
Creates a new TCP socket.
*   **Returns**: An Integer representing the Socket ID (File Descriptor or Handle). Returns `< 0` on failure.

#### `__native_socket_close(id: Int) -> Boolean`
Closes an existing socket.
*   **id**: The Socket ID to close.
*   **Returns**: `true` on success, `false` otherwise.

### Connection (Client)

#### `__native_socket_connect(id: Int, host: String, port: Int) -> Boolean`
Connects the socket to a remote host.
*   **id**: The Socket ID.
*   **host**: The hostname (e.g., "google.com") or IP address string.
*   **port**: The target port (e.g., 80 for HTTP, 443 for HTTPS).
*   **Returns**: `true` if connected successfully, `false` otherwise.

### Data Transfer

#### `__native_socket_send(id: Int, data: String) -> Boolean`
Sends data to the connected peer.
*   **id**: The Socket ID.
*   **data**: The string data to send.
*   **Returns**: `true` on success.

#### `__native_socket_receive(id: Int, size: Int) -> String`
Receives data from the socket into a buffer of `size` bytes.
*   **id**: The Socket ID.
*   **size**: Maximum number of bytes to read per call (e.g., 4096).
*   **Returns**: The received data as a String. Returns an **empty string** `""` if the connection is closed or no data is available.

---

### Server Management

#### `__native_socket_bind(id: Int, port: Int) -> Boolean`
Binds the socket to a specific port on the local machine.
*   **id**: The Socket ID.
*   **port**: The port number to listen on (e.g., 8080).
*   **Returns**: `true` on success, `false` otherwise (e.g., port already in use).

#### `__native_socket_listen(id: Int) -> Boolean`
Sets the socket to listen for incoming connections.
*   **id**: The Socket ID.
*   **Returns**: `true` on success.

#### `__native_socket_accept(id: Int) -> ID`
Blocks execution until a client connects, then returns a **new Socket ID** for that client.
*   **id**: The Server Socket ID.
*   **Returns**: A new integer Socket ID representing the client connection.

---

## Example: Simple HTTP Client

```kotlin
class HttpClient {
    static func get(host: String, path: String) -> String {
        var sock = __native_socket_create();
        if (sock < 0) { return "Error: Socket creation failed"; }

        var connected = __native_socket_connect(sock, host, 80);
        if (!connected) {
            __native_socket_close(sock);
            return "Error: Connection failed";
        }

        var request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
        __native_socket_send(sock, request);

        var response = "";
        var chunk = "";
        while (true) {
            chunk = __native_socket_receive(sock, 4096);
            if (chunk == "") { break; }
            # In a real implementation, you'd perform string concatenation more efficiently
            response = response + chunk;
        }

        __native_socket_close(sock);
        return response;
    }
}

# Usage
var content = HttpClient.get("www.example.com", "/");
println(content);

## Example: Web Server

```kotlin
print("Starting Server on 8080...");
var server = __native_socket_create();
__native_socket_bind(server, 8080);
__native_socket_listen(server);

var body = "<h1>Hello from GW Server!</h1>";
var res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n" + body;
var client = 0;
var req = "";

while (true) {
    client = __native_socket_accept(server);
    req = __native_socket_receive(client, 1024);

    println("=" * 40 + " CLIENT SOCKET ID " + "=" * 40);
    println(client);
    println("=" * 40 + " REQUEST INFO " + "=" * 40);
    println(req);
    
    __native_socket_send(client, res);
    __native_socket_close(client);
}
```
