# Native Module: String

This module provides low-level string manipulation primitives.
While basic, they allow building powerful string utilities in GW.

## Exposed Primitive Functions

#### `__native_string_length(str: String) -> Int`
Returns the length of the string.
*   **str**: The input string.
*   **Returns**: The number of characters in the string.

#### `__native_string_at(str: String, index: Int) -> Char`
Returns the character at the specified index.
*   **str**: The input string.
*   **index**: The 0-based index.
*   **Returns**: The character as a specialized Value (Char type) or String of length 1.
*   **Note**: Throws error if index is out of bounds.

---

## Example: StringBuilder Class (Pure GW)

```kotlin
class StringBuilder {
    content: String
    
    constructor() {
        this.content = "";
    }

    func append(str: String) {
        this.content = this.content + str;
    }

    func toString() -> String {
        return this.content;
    }
    
    func length() -> Int {
        return __native_string_length(this.content);
    }
    
    func charAt(index: Int) -> String {
        return __native_string_at(this.content, index);
    }
    
    func printChars() {
        var len = this.length();
        var i = 0;
        var c = '';
        print("Chars: ");
        while (i < len) {
            c = this.charAt(i);
            print("[" + c + "] ");
            i = i + 1;
        }
        print("\n");
    }

    func substring(start: Int, end: Int) -> String {
        var res = "";
        var i = start;
        while (i < end) {
            res = res + this.charAt(i);
            i = i + 1;
        }
        return res;
    }
}

var sb = StringBuilder();
sb.append("Hello");
sb.append(" World");
sb.printChars();
print("Sub: " + sb.substring(0, 5));
```
