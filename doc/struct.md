# Struct

`struct` (structures) are custom data types that allow grouping several variables (fields) under a single name. They are simpler than classes as they do not support constructors, methods, or operator overloading.

## Basic Syntax

```kotlin
struct MyStruct {
    field1: Type,
    field2: Type
}
```

## Instantiation

To create an instance of a `struct`, declare a variable with the struct name as the initial value. All fields are initialized to their default value (as defined in the declaration or `null`).

```kotlin
var instance = MyStruct;
```

## Accessing Fields

Fields of a struct are accessed using the dot operator `.`.

```kotlin
instance.field1 = value;
print(instance.field1);
```

## Example

```kotlin
struct Vector2 {
    x: Number,
    y: Number
}

# Create an instance from the Struct
var v = Vector2;

# Assign values
v.x = 10.5;
v.y = 20.0;

# Access values
print("Vector: (" + v.x + ", " + v.y + ")");
```
