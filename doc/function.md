# Function

A function is defined by the `func` keyword, followed by the function name, parameters in parentheses, and the return type after an arrow `->`.

```kotlin
func myFunction(params...) -> Type {
    return ...;
}
```

A function can be called anywhere in the code and can be used within an expression.
```kotlin
myFunction(argument values...);
```

## Example

```kotlin
func fibonacci(n: Integer) -> Integer {
    var a = 0;
    var b = 1;
    var i = 0;
    var temp = 0;
    while (i < n) {
        temp = a + b;
        a = b;
        b = temp;
        i = i + 1;
    }
    return a;
}
```

```kotlin
var r = fibonacci(10);
print("Fibonacci: " + r);
```