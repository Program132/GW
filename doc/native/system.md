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

### Command Line Arguments

These functions allow access to arguments passed to the script (e.g., `GW script.gw arg1 arg2`).

#### `__native_get_arg_count() -> Int`
Returns the number of arguments passed to the script.

#### `__native_get_arg(index: Int) -> String`
Returns the argument at the specified index.
*   **index**: The 0-based index of the argument.
*   **Throws**: An error if the index is out of bounds.

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
    
    # Arguments Helper
    static func getArgs() -> List {
        # Note: Ideally returns a List, but here we simulate access via count/index
        # You could implement a proper List builder here if List class is available
        return null; 
    }
    
    static func getArg(index: Int) -> String {
        return __native_get_arg(index);
    }
    
    static func getArgCount() -> Int {
        return __native_get_arg_count();
    }
}

# Usage: GW script.gw --verbose
var count = System.getArgCount();
if (count > 0) {
    var firstArg = System.getArg(0);
    if (firstArg == "--verbose") {
        print("Verbose mode enabled.\n");
    }
}
