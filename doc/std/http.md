# Standard Library: Network & HTTP

The `HTTP` module provides high-level APIs for interacting with the network, sending HTTP requests (Client), and hosting web applications (Server). It relies on the native TCP socket wrapper (`class Socket`).

To use it: include `std/http.gw`.

---

## Class `Socket`

A static wrapper for native TCP socket functions (`Network.h`). Used under the hood by the HTTP classes.

### `Socket.create() -> Integer`
Creates a TCP socket and returns its unique ID.

### `Socket.close(id: Integer)`
Closes the specified socket.

### `Socket.connect(id: Integer, host: String, port: Integer)`
Connects the socket to a remote host and port via DNS resolution or IP address.

### `Socket.send(id: Integer, data: String)`
Sends data (payload) over an established socket connection.

### `Socket.receive(id: Integer, size: Integer) -> String`
Listens and receives up to `size` bytes from the socket.

---

## Class `HttpResponse`

Represents an HTTP response. Can be parsed from a raw string or constructed manually for server responses.

### Fields
*   **`status: Integer`**: The HTTP status code (e.g., 200, 404).
*   **`headers: Map<String, String>`**: Response headers.
*   **`body: String`**: The response body content.

### Methods
*   **`setStatus(code: Integer)`**: Sets the response status code.
*   **`addHeader(name: String, value: String)`**: Adds or updates a header.
*   **`setBody(content: String)`**: Sets the body and automatically updates `Content-Length`.
*   **`toString() -> String`**: Serializes the response for transmission.

---

## Class `HttpRequest`

Represents an incoming HTTP request.

### Fields
*   **`method: String`**: The HTTP method (GET, POST, PUT, DELETE, etc.).
*   **`path: String`**: The requested path.
*   **`headers: Map<String, String>`**: Request headers.
*   **`body: String`**: The request body.

### Methods
*   **`toString() -> String`**: Serializes the request (Method + Path + Headers + Body). Useful for debugging.

---

## Class `HttpClient`

A client for interacting with web services.

### `HttpClient.get(host: String, path: String) -> HttpResponse`
Sends a GET request.

### `HttpClient.post(host: String, path: String, body: String) -> HttpResponse`
Sends a POST request with the provided payload.

### `HttpClient.put(host: String, path: String, body: String) -> HttpResponse`
Sends a PUT request to update a resource.

### `HttpClient.delete(host: String, path: String) -> HttpResponse`
Sends a DELETE request to remove a resource.

---

## Class `HttpServer`

A Web Server to host web applications and APIs in GW.

### `HttpServer.startBasic(port: Integer, responseBody: String)`
Listen on a port and return the same static body for all requests.

### `HttpServer.listen(port: Integer, handler: Function)`
Starts a dynamic server. The `handler` receives an `HttpRequest` and should return an `HttpResponse` object.

> [!TIP]
> While the server accepts primitive strings as return values, using `HttpResponse` is recommended for full control over status codes and headers.

#### Example:
```kotlin
import "std/http.gw";

HttpServer.listen(8080, func(req: HttpRequest) {
    if (req.path == "/hello") {
        var res = new HttpResponse("Hello World!");
        res.addHeader("Content-Type", "text/plain");
        return res;
    }
    
    var res = new HttpResponse("Page not found");
    res.setStatus(404);
    return res;
});
```

---

## Class `HttpRouter`

A utility to simplify request dispatching by mapping methods and paths to handlers.

### `HttpRouter.add(method: String, path: String, handler: Function)`
Registers a handler for a specific HTTP method (e.g., "GET", "POST") and path.

### `HttpRouter.handle(request: HttpRequest) -> HttpResponse`
Dispatches the request to the matching handler. Returns a default 404 response if no match is found.

#### Full Routing Example:
```kotlin
import "std/http.gw";

var router = new HttpRouter();

# HTML Home Page
router.add("GET", "/", func(req: HttpRequest) { 
    return new HttpResponse("<h1>Welcome</h1>"); 
});

# API Endpoint
router.add("POST", "/data", func(req: HttpRequest) { 
    var res = new HttpResponse("Data received");
    res.setStatus(201);
    return res;
});

HttpServer.listen(8080, func(req: HttpRequest) {
    return router.handle(req);
});
```
