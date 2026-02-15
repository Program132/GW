# GW Programming Language

<div align="center">

![Latest version](https://img.shields.io/github/v/release/Program132/GW?style=for-the-badge&include_prereleases)
![Code size](https://img.shields.io/github/languages/code-size/Program132/GW?style=for-the-badge&logo=github)
![Downloads](https://img.shields.io/github/downloads/Program132/GW/total?color=%2324cc24&style=for-the-badge&logo=github)
![Open Issues](https://img.shields.io/github/issues/Program132/GW?style=for-the-badge&logo=github)

[Documentation](#documentation) • [Example](#quick-peek)

</div>

---

## 🚀 Unleash the Power of GW

**GW** is an object-oriented programming language crafted to be the ultimate developer's tool. By distilling the power of **C++**, the elegance of **Python**, the safety of **Java**, and the modern feel of **Kotlin**, GW offers a coding experience that is both remarkably powerful and refreshingly simple.

Currently an high-performance interpreted language, GW is evolving towards native transpilation, promising the speed of compiled languages with the ease of modern syntax.

---

## ⚡ Quick Peek (Not working yet)

See how clean object-oriented programming can be in GW:

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

print("Vector Addition: " + (p + q).str()); # -> (4, 6)
print("Point + Scalar:  " + (p + 5).str()); # -> (6, 7)
```

---

## 📚 Documentation

Dive into the details and start building with GW:

| Topic | Description |
| :--- | :--- |
| 🏁 **[Hello World](doc/hello_world.md)** | Your first steps with GW. |
| 📦 **[Variable](doc/variable.md)** | Learn about state management. |

## Compile & Run

Windows:
```powershell
git clone https://github.com/Program132/GW.git
cd GW
build.bat
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