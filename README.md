# Soft Trace

A CPU Ray Tracing application that renders basic Global Illumination effects.
The application displays a Qt UI with two windows: the left window shows the scene rendered using OpenGL (immediate mode), while the right window displays the equivalent ray-traced scene.

## Project Structure

```plaintext
soft-trace/
├── src/                 # Source code files
├── assets/              # Static assets (.obj and .mtl files)
├── build/               # Generated build files
├── bin/                 # Generated executable files
├── soft-trace.pro       # QMake project file
└── README.md            # Project README
```

## Build

```bash
qmake
make
```

## Run

```bash
bin/soft-trace <.obj> <.mtl>
```

Example:

```shell
bin/soft-trace assets/cornell_box.obj assets/cornell_box.mtl
```

The asset files must be well-formed for the application to work. See `Material.h` for custom .mtl properties used.
Example files are provided.

## Technologies

* **C++**: `>= C++17`
* **Qt**: `5.12.x`
* **OpenGL**: `>= 3.0`
* **OpenMP**: `>= 4.5`

Newer versions of Qt might work correctly, if no breaking changes that affect the application were introduced.

## Showcase

https://github.com/user-attachments/assets/1e5a3f52-17b8-4727-ada5-8ae3649a8f86
