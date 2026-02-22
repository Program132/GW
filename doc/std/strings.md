# Standard Library: Strings

The `Strings` module provides the `StringUtils` and `StringBuilder` classes for extensive string manipulation, searching, and efficient concatenation.

To use it: include `std/strings.gw`.

---

## Class `StringUtils`

A utility class providing static methods for string operations. **Note**: Strings in GW are immutable. These functions return new string instances.

### `StringUtils.toLower(str: String) -> String`
Converts all characters in the string to lowercase.

### `StringUtils.toUpper(str: String) -> String`
Converts all characters in the string to uppercase.

### `StringUtils.substring(str: String, start: Integer, end: Integer) -> String`
Extracts a portion of a string.
*   **start**: The starting index (inclusive).
*   **end**: The ending index (exclusive).
*   **Returns**: The extracted substring.

### `StringUtils.contains(str: String, search: String) -> Boolean`
Checks if the string contains the specified substring.
*   **Returns**: `true` if found, `false` otherwise.

### `StringUtils.indexOf(str: String, search: String) -> Integer`
Finds the first occurrence of a substring within the string.
*   **Returns**: The starting index of the substring, or `-1` if not found.

### `StringUtils.trim(str: String) -> String`
Removes leading and trailing space characters (`" "`) from the string.

---

## Class `StringBuilder`

A mutable class designed for efficient string building and concatenation, avoiding the performance overhead of creating many intermediate immutable string objects.

### Constructors

#### `StringBuilder()`
Initializes an empty string builder.

#### `StringBuilder(initial: String)`
Initializes the builder with the given starting string.

### Methods

#### `StringBuilder.append(str: String)`
Appends the given string to the current content.

#### `StringBuilder.appendLine(str: String)`
Appends the given string followed by a newline character (`\n`).

#### `StringBuilder.setContent(str: String)`
Overwrites the current content with the new string.

#### `StringBuilder.toString() -> String`
Returns the fully constructed string.

#### `StringBuilder.length() -> Integer`
Returns the current number of characters in the builder.

#### `StringBuilder.clear()`
Empties the builder's content.

#### `StringBuilder.toLower()`
Converts the internal content to lowercase in-place.

#### `StringBuilder.toUpper()`
Converts the internal content to uppercase in-place.

---

## Example Usage

```kotlin
# String Utilities
var text = "  Hello GW World  ";
println("Trimmed: '" + StringUtils.trim(text) + "'");
println("Index of 'GW': " + StringUtils.indexOf(text, "GW"));
println("Contains 'World': " + StringUtils.contains(text, "World"));
println("Substring: " + StringUtils.substring(text, 2, 7)); # "Hello"

# String Building
var sb = StringBuilder("SELECT * FROM users");
sb.append(" WHERE age > 18");
sb.appendLine(";");
sb.toUpper();

println("Query:\n" + sb.toString());
```
