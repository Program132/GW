# GW Programming Language

<div align="center">

![Latest version](https://img.shields.io/github/v/release/Program132/GW?style=for-the-badge&include_prereleases)
![Code size](https://img.shields.io/github/languages/code-size/Program132/GW?style=for-the-badge&logo=github)
![Downloads](https://img.shields.io/github/downloads/Program132/GW/total?color=%2324cc24&style=for-the-badge&logo=github)
![Open Issues](https://img.shields.io/github/issues/Program132/GW?style=for-the-badge&logo=github)
![Closed Issues](https://img.shields.io/github/issues-closed/Program132/GW?style=for-the-badge&color=%23560d75)

[Documentation](#documentation) • [Example](#quick-peek)

</div>

---

## 🚀 Unleash the Power of GW

**GW** is an object-oriented programming language crafted to be the ultimate developer's tool. By distilling the power of **C++**, the elegance of **Python**, the safety of **Java**, and the modern feel of **Kotlin**, GW offers a coding experience that is both remarkably powerful and refreshingly simple.

Currently an high-performance interpreted language, GW is evolving towards native transpilation, promising the speed of compiled languages with the ease of modern syntax.

---

## ⚡ Quick Peek

See how clean object-oriented programming can be in GW:

```kotlin
struct Vec2 {
    x: Number,
    y: Number
}

class Point {
    x: Number,
    y: Number

    constructor(x: Number, y: Number) {
        this.x = x;
        this.y = y;
    }

    constructor(x: Number) {
        this.x = x;
        this.y = x;
    }

    func add(p1: Point, p2: Point) -> Point {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }

    func add2(v: Vec2) -> Integer {
        this.x += v.x;
        this.y += v.y;
        return 0;
    }

    func str() -> String {
        return "(" + this.x + ", " + this.y + ")";
    }

    operator +(p1: Point, p2: Point) {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }

    operator +(p1: Point, a: Number) {
        return Point(p1.x + a, p1.y + a);
    }
}

var p = Point(1, 2);
var p2 = Point(3, 4);
var p3 = p2.add(p, p2);

println(p3.str());

var v = Vec2(5, 5);
p2.add2(v);
println(p2.str());

var p4 = p + p2;
println(p4.str());

var p5 = p + 5;
println(p5.str());
```

---

## 📚 Documentation

Dive into the details and start building with GW:

| Topic | Description |
| :--- | :--- |
| 🏁 **[Hello World](doc/hello_world.md)** | Your first steps with GW. |
| 💎 **[Types](doc/types.md)** | Explore the core data types. |
| 📦 **[Variable](doc/variable.md)** | Learn about state management. |
| ⚙️ **[Function](doc/function.md)** | Define reusable logic. |
| 🛣️ **[Conditions](doc/conditions.md)** | Logic flow and branching. |
| 🔁 **[Loops](doc/loops.md)** | Mastering iterations. |
| 🏗️ **[Struct](doc/struct.md)** | Lightweight data structures. |
| 🏛️ **[Class](doc/class.md)** | Advanced object-orientation. |
| 📂 **[Imports](doc/imports.md)** | Import other files. |

Native functions:
| Topic | Description |
| :--- | :--- |
| ❓ **[Introduction](doc/native/readme.md)** | C++ functions directly bound to the interpreter. |
| 🕛 **[Time](doc/native/time.md)** | C++ implementations of "time" (timestamps, sleep) functions. |
| ⌨️ **[Input/Output](doc/native/io.md)** | C++ implementations of input/output functions. |
| 🖥️ **[System](doc/native/system.md)** | C++ implementations of system (exec cmds, exit, ...) functions. |
| 🌐 **[Network](doc/native/network.md)** | TCP/IP Socket implementation for networking. |

## Compile & Run

Windows:
```powershell
git clone https://github.com/Program132/GW.git
cd GW
.\build.bat
```

Linux:
```bash
git clone https://github.com/Program132/GW.git
cd GW
chmod +x build.sh
./build.sh
```

Run (Debug or Release):
```bash
./build/debug/GW.exe
```
```bash
./build/release/GW.exe
```

---
<div align="center">
Developed with ❤️ by Program
</div>