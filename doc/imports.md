# Modules and Imports

GW supports organizing code into multiple files using the `import` keyword. This works similarly to the C/C++ preprocessor: the content of the imported file is directly substituted in place of the `import` statement.

## Syntax

```gw
import "filename.gw"
```

The path is relative to the file containing the import statement.

## How it works

1. The `FileReader` scans the source code for lines starting with `import`.
2. When found, it resolves the path of the imported file.
3. It reads the content of that file and replaces the `import` line with it.
4. This process is recursive: imports inside imported files are also processed.
5. **Circular Dependency Protection**: The system keeps track of included files to prevent infinite loops (e.g., A imports B, B imports A). A file is only included once per compilation unit.

## Example

### `math_utils.gw`
```gw
func square(x: Number) {
    return x * x;
}
```

### `main.gw`
```gw
import "math_utils.gw"

var result = square(5);
println(result); // Output: 25
```

## Directory Structure

Imports work with subdirectories as well:

```
project/
├── main.gw
└── lib/
    └── utils.gw
```

In `main.gw`:
```gw
import "lib/utils.gw"
```

In `lib/utils.gw`, you can import other files relative to `lib/`:
```gw
import "other_util.gw" // Looks for project/lib/other_util.gw
```
