# Native Module: Input/Output

This module provides native bindings for console interactions, file operations, and standard stream manipulation.

## Exposed Primitive Functions

### Console Output

#### `__native_print(value: String)`
Prints the value to the standard output without a newline.

---

### Console Input

#### `__native_input_string(prompt: String) -> String`
Displays a prompt and reads a line of text from standard input.

#### `__native_input_int(prompt: String) -> Int`
Displays a prompt and reads an integer from standard input. Throws an error if input is invalid.

#### `__native_input_double(prompt: String) -> Number`
Displays a prompt and reads a floating-point number from standard input.

---

### File System

#### `__native_file_read(path: String) -> String`
Reads the entire content of the file at the specified path. Returns the content as a string.

#### `__native_file_write(path: String, content: String) -> Boolean`
Writes the content to the file at the specified path. Overwrites existing content. Returns `true` on success.

#### `__native_file_append(path: String, content: String) -> Boolean`
Appends the content to the end of the file at the specified path. Returns `true` on success.

---

### Standard Streams (Pipes)

#### `__native_read_stdin() -> String`
Reads all remaining content from the Standard Input (stdin). Useful for reading data piped from other programs.

#### `__native_write_stdout(content: String)`
Writes content directly to Standard Output (stdout).

---

### Console Control (Windows/ANSI)

#### `__native_clear_console()`
Clears the console screen and moves the cursor to (0,0).

#### `__native_set_cursor(x: Int, y: Int)`
Moves the console cursor to the specified coordinates.

#### `__native_get_cursor() -> Struct`
Returns the current cursor position as a struct `{x: Int, y: Int}`.
```kotlin
var pos = __native_get_cursor();
println("Cursor Position: (" + pos.x + "," + pos.y + ")");
```

---

## Example: IO Wrapper Class

```kotlin
class IO {
    static func readFile(path: String) -> String {
        return __native_file_read(path);
    }

    static func writeFile(path: String, content: String) {
        __native_file_write(path, content);
    }

    static func appendFile(path: String, content: String) {
        __native_file_append(path, content);
    }

    static func clear() {
        __native_clear_console();
    }

    static func moveCursor(x: Int, y: Int) {
        __native_set_cursor(x, y);
    }
}

IO.writeFile("log.txt", "Start Log\n");
IO.appendFile("log.txt", "Entry 1: Success\n");
var logs = IO.readFile("log.txt");
println(logs);
```
