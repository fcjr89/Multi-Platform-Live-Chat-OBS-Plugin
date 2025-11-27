# Building from Source

## Prerequisites

- CMake 3.16+
- Qt 6.5+
- C++17 compiler
- OBS Studio development files

---

## Windows Build

### Requirements
- Visual Studio 2019 or newer
- Qt 6.5+ (with WebSockets module)
- CMake

### Build Steps

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64"
cmake --build . --config Release
cmake --install . --config Release
