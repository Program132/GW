# Class

Classes are templates for creating objects. They allow grouping data (attributes) and behaviors (methods, operator overloading, etc.) within a single entity.

Unlike `struct`, classes support constructors, methods, and operator overloading.

## Basic Syntax

```kotlin
class MyObject {
    c1: Type,
    c2: Type

    constructor(cc1: Type, cc2: Type) {
        this.c1 = cc1;
        this.c2 = cc2;
    }

    func myMethod() -> Type {
        return this.c1;
    }
}
```

## Constructors

A class can have multiple constructors. The appropriate constructor is chosen based on the number of arguments provided during instantiation.

```kotlin
constructor(x: Number, y: Number) {
    this.x = x;
    this.y = y;
}

constructor(x: Number) {
    this.x = x;
    this.y = 0;
}
```

## Methods

Methods are functions defined inside a class. They have access to the instance's fields via the `this` keyword.

```kotlin
func str() -> String {
    return "(" + this.x + ", " + this.y + ")";
}
```

## Operator Overloading

GW allows you to redefine the behavior of operators (such as `+`, `-`, `*`, etc.) for your classes.

```kotlin
operator +(other: Point) {
    return Point(this.x + other.x, this.y + other.y);
}

operator +(val: Number) {
    return Point(this.x + val, this.y + val);
}
```

## Full Example

Here is a full example using a `Point` class with multiple constructors and operator overloading:

```kotlin
class Point {
    x: Number,
    y: Number 

    constructor(x: Number, y: Number) {
        this.x = x;
        this.y = y;
    }

    constructor(x: Number) {
        this.x = x;
        this.y = 0;
    }

    func str() -> String {
        return "(" + this.x + ", " + this.y + ")";
    }
  
    operator +(other: Point) {
        return Point(this.x + other.x, this.y + other.y);
    }

    operator +(a: Integer) {
        return Point(this.x + a, this.y + a);
    }
}

var p = Point(1, 2);
var q = Point(3, 4);

print("-> " + (p + q).str());
print("-> " + (p + 5).str());
```
