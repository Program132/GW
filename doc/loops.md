# Loops

## While

The `while` loop is used to execute a block of code as long as a condition is true.

**Note**: It is important to define a condition that will eventually become false; otherwise, the loop will be infinite.

```kotlin
while (condition) {
    # code
}
```

## For

The `for` loop is used to execute a block of code a specific number of times. You define a starting value, a condition (which determines when to stop), and an increment.

```kotlin
for (var i = start; i < end; i = i + 1) {
    # code
}
```

## Example

```kotlin
func x(n: Integer) -> Integer {
    # for: 
    for (var i = 0; i < n; i = i + 1) {
        print(i);
    }

    # while: 
    var j = 0;
    while (j < n) {
        print(j);
        j = j + 1;
    }
    return 0;
}
```