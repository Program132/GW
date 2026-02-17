# Native Functions & Constants

GW supports **Native Functions**, which are low-level C++ implementations directly bound to the interpreter. These primitives act as the foundational layer for the GW Standard Library, enabling interactions with the operating system, file system, and hardware that would otherwise be impossible in pure GW code.

> **Note:** Native functions are typically prefixed with `__native_` (e.g., `__native_time`) to distinguish them from user-land code. They are **not** intended to be used directly by end-users. Instead, they should be wrapped inside high-level GW classes.

## Example: Time Wrapper

The following example demonstrates how to wrap low-level C++ primitives (`__native_time`, `__native_sleep`) into a clean, object-oriented GW class.

```kotlin
// Example wrapper class
class Time {
    // Returns the current timestamp (seconds since epoch)
    static func now() -> Int {
        return __native_time();
    }
    
    // Pauses execution for the specified number of seconds
    static func sleep(seconds: Int) {
        __native_sleep(seconds);
    }
}
```

By using this pattern (`Native Primitives` -> `Standard Library Wrapper`), we keep the core language simple while allowing powerful system integrations.