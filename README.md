# ObjViewer

ObjViewer is a lightweight 3D object viewer built with modern OpenGL. It enables users to load `.obj` files, control lighting, and explore the scene with intuitive camera controls.

## Features

- Support for `.obj` file loading, including material and texture support.
- Dynamic lighting management with multiple light sources.
- First-person camera controls.

## Building the Project

### Requirements

- C++20-compatible compiler
- CMake 3.27 or newer
- Dependencies: GLFW, GLAD, GLM, TinyOBJLoader, stb_image (automatically fetched via `FetchContent`)

### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/SoggyRihno/ObjViewer
   cd ObjViewer
   ```
2. Create a build directory and navigate into it:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   cmake --build .
   ```

## Running the Application

1. Prepare a `.obj` file and its associated material/texture files. The obj, mtl, png, ect must be in the same folder. The ones I used can be found here https://casual-effects.com/data/index.html.
3. Run the application from the command line, specifying the path to the `.obj` file:
   ```bash
   ./ObjViewer path/to/model.obj
   ```

## Controls

### Camera Controls

- **Move Forward**: `W`
- **Move Backward**: `S`
- **Strafe Left**: `A`
- **Strafe Right**: `D`
- **Sprint (Fast Movement)**: `Left Shift`
- **Slow Movement**: `Left Control`
- **Adjust View**: Move mouse
- **Exit Application**: `Escape`

### Light Management

- **Spawn a Light at Current Camera Position**: `;`
- **Delete Current Light**: `'`
- **Select Next Light**: `Right Arrow`
- **Select Previous Light**: `Left Arrow`
- **Set Current Light to Camera Position**: `P`
- **Increment Light Color** (hold `Up Arrow` or `Down Arrow` while pressing a color key):
  - **Red**: `8` + `Up Arrow`/`Down Arrow`
  - **Green**: `9` + `Up Arrow`/`Down Arrow`
  - **Blue**: `0` + `Up Arrow`/`Down Arrow`
- **Adjust Light Intensity**:
  - **Increase**: `Up Arrow` only (without `8`, `9`, `0`)
  - **Decrease**: `Down Arrow` only (without `8`, `9`, `0`)

