# Standard Library: OS

The `OS` module provides a high-level API for interacting with the operating system, managing environment variables, and controlling processes. It wraps the low-level `__native_` system calls into a clean, object-oriented interface.

To use it: include `std/os.gw`.

## Class `OS`

The static `OS` class contains all system-related utility functions.

### Process Management

#### `OS.exit(code: Integer)`
Termitanes the program immediately with the specified exit code.
*   **code**: Exit status (usually 0 for success).

#### `OS.getPID() -> Integer`
Returns the Process ID (PID) of the current GW instance.

#### `OS.kill(pid: Integer) -> String`
Attempts to terminate a process by its ID using native system commands (`taskkill` on Windows, `kill -9` on Unix).
*   **Returns**: The output or error message from the system command.

---

### Command Execution

#### `OS.exec(command: String) -> String`
Executes a shell command and returns its standard output.
*   **command**: The raw shell command to execute.
*   **Returns**: The output as a string.

---

### Environment & Filesystem

#### `OS.getEnv(name: String) -> String`
Retrieves the value of an environment variable.
*   **name**: The variable name (e.g., `"PATH"`).

#### `OS.setEnv(name: String, value: String)`
Sets or updates an environment variable for the current process.

#### `OS.getCurrentDirectory() -> String`
Returns the current working directory of the process.

---

### System Information

#### `OS.getPlatform() -> String`
Returns the platform name: `"Windows"`, `"Linux"`, or `"Mac"`.

#### `OS.isWindows() -> Boolean`
Checks if the OS is Windows.

#### `OS.isLinux() -> Boolean`
Checks if the OS is Linux.

#### `OS.isMac() -> Boolean`
Checks if the OS is macOS.

#### `OS.getHostName() -> String`
Returns the network name of the computer.

#### `OS.getUsername() -> String`
Returns the name of the currently logged-in user.

---

## Example Usage

```kotlin
# Print system info
println("Platform: " + OS.getPlatform());
println("User:     " + OS.getUsername());
println("PID:      " + OS.getPID());

# Work with environment
var path = OS.getEnv("PATH");
OS.setEnv("MY_VAR", "GW_Power");
println("New variable: " + OS.getEnv("MY_VAR"));

# Execute command
var files = OS.exec(OS.isWindows() ? "dir" : "ls");
println("Current files:\n" + files);

# Exit with error if needed
if (OS.getHostName() == "Forbidden") {
    OS.exit(1);
}
```