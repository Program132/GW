# Native Module: System

This module provides bindings for interacting with the operating system, executing commands, and managing process flow.

## Exposed Primitive Functions

### Command Execution

#### `__native_exec_cmd(command: String)`
Executes a system shell command. The output is printed directly to the standard output.
*   **command**: The shell command to run (e.g., `ls -la`, `dir`).

---

### OS Detection

These functions allow scripts to be cross-platform by detecting the host operating system.

#### `__native_is_windows() -> Boolean`
Returns `true` if the interpreter is running on **Windows**, `false` otherwise.

#### `__native_is_linux() -> Boolean`
Returns `true` if the interpreter is running on **Linux**, `false` otherwise.

#### `__native_is_mac() -> Boolean`
Returns `true` if the interpreter is running on **MacOS**, `false` otherwise.

---

### Process Management

#### `__native_exit(code: Int)`
Terminates the GW program immediately with the specified exit code.
*   **code**: The status code to return to the parent process (0 usually indicates success).

---

## Example: System Wrapper Class

```kotlin
class System {
    static func exec(cmd: String) {
        __native_exec_cmd(cmd);
    }

    static func exit(code: Int) {
        __native_exit(code);
    }

    static func isWindows() -> Boolean {
        return __native_is_windows();
    }
    
    static func getPlatform() -> String {
        if (__native_is_windows()) { return "Windows"; }
        if (__native_is_linux()) { return "Linux"; }
        if (__native_is_mac()) { return "MacOS"; }
        return "Unknown";
    }
}

# Usage
if (System.isWindows()) {
    System.exec("dir");
} else {
    System.exec("ls");
}

System.exit(0);
```