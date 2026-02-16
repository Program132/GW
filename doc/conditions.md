# Conditions in GW

GW supports conditional execution using `if` and `else` statements.

## Syntax

The basic syntax for an `if` statement is:

```gw
if (condition) {
    // code to execute if condition is true
}
```

You can also add an `else` block:

```gw
if (condition) {
    // code to execute if condition is true
} else {
    // code to execute if condition is false
}
```

## Examples

### Simple If

```gw
var x = 10;
if (x > 5) {
    print("x is greater than 5");
}
```

### If-Else

```gw
var age = 17;
if (age >= 18) {
    print("You are an adult.");
} else {
    print("You are a minor.");
}
```

## Nested Conditions

You can nest conditions within each other:

```gw
var x = 10;
var y = 20;

if (x > 5) {
    if (y > 15) {
        print("Both conditions are met");
    }
}
```

## Recursive Example

Conditions are often used in recursive functions, like calculating the Fibonacci sequence:

```gw
func fibonacci(n:Integer) -> Integer {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

print("Fibonacci(7) is: ");
println(fibonacci(7)); // Output: 13
```