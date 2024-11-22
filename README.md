# Soft Trace

CPU Ray Tracing application exhibiting basic Global Illumination effects.
The application displays a Qt UI with two windows: the left window shows the scene rendered using OpenGL (immediate mode), while the right window displays the equivalent ray-traced scene.

![soft-trace](https://github.com/user-attachments/assets/2e5d981c-8d5d-4b2b-bdb8-c9f5149da0f7)

## Project Structure

```plaintext
soft-trace/
├── src/                 # Source code
├── assets/              # Static assets (.obj and .mtl files)
├── soft-trace.pro       # QMake project
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
* **OpenGL**: `>= 4.0`
* **OpenMP**: `>= 4.5`
* **GLM**: `1.0.1`

Newer versions of Qt might work correctly, if no breaking changes that affect the application were introduced.
