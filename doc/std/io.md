# Standard Library: IO

The `IO` module provides high-level classes for file manipulation and console interaction. It builds upon the native `__native_file_` and `__native_input_` primitives.

To use it: include `std/io.gw`.

---

## Class `File`

Provides static methods for file system operations.

### `File.read(path: String) -> String`
Reads the entire content of a file and returns it as a string.
*   **path**: Path to the file.
*   **Returns**: File content.
*   **Throws**: Error if file cannot be opened.

### `File.write(path: String, content: String)`
Writes content to a file. **Warning**: This will overwrite any existing content in the file.
*   **path**: Path to the file.
*   **content**: String to write.

### `File.append(path: String, content: String)`
Appends content to the end of an existing file.
*   **path**: Path to the file.
*   **content**: String to append.

---

## Class `Console`

Provides static methods for terminal interaction, user input, and screen control.

### `Console.println(text: String)`
Displays text followed by a newline on the standard output.

### `Console.readString(prompt: String) -> String`
Displays a prompt and reads a string from the user.

### `Console.readInt(prompt: String) -> Integer`
Displays a prompt and reads an integer. Throws an error if input is not a valid integer.

### `Console.readNumber(prompt: String) -> Number`
Displays a prompt and reads a number (float/double).

### `Console.clear()`
Clears the terminal screen.

### `Console.setCursor(x: Integer, y: Integer)`
Moves the cursor to the specified coordinates.
*   **x**: Column (0-indexed).
*   **y**: Row (0-indexed).

### `Console.getCursor() -> Struct`
Returns a struct `CursorPos { x, y }` containing the current cursor position.

### `Console.readAllStdin() -> String`
Reads all available data from standard input (useful for pipes) until EOF.

---

## Example Usage

```kotlin
# File operations
var logFile = "app.log";
File.write(logFile, "Session started\n");
File.append(logFile, "User logged in\n");

# Console interaction
Console.clear();
var name = Console.readString("Enter your name: ");
Console.println("Welcome, " + name + "!");

# Cursor manipulation
Console.setCursor(10, 5);
Console.println("Centered Text");
```
