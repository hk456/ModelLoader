# 🛠️ ModelLoader
A lightweight, high-performance 3D Model Loader and Interactive Viewer engineered from scratch using modern OpenGL.

---

## 📌 Project Overview
This project is an entry-level graphics exploration focused on asset processing pipelines, real-time shader pipelines, and low-overhead rendering. By moving away from rigid framework architectures, it handles manual memory layout mappings, camera space transformations, and custom mesh buffer allocation natively.

> **Portability Note:** Built as a vendored project using native Visual Studio macros (`$(ProjectDir)`). Zero external system dependencies required. Clone, open the `.sln` workspace, and run.

---

## 🚀 Key Architectural Features
*   **Asset Processing:** Efficient extraction of vertex attributes (positions, normals, texture mappings).
*   **Custom Shader Pipeline:** Modular GLSL pipeline configuration with real-time transformation logic.
*   **Advanced Normal Mapping:** Tangent, Bitangent, and Normal (TBN) coordinate space evaluations for fine-detail depth simulation.
*   **Dynamic Coordinate Systems:** Full fly-through 3D camera matrices tracking local pitch, yaw, and translation vectors.

---

## 📂 Portable Project Architecture
The workspace uses a local vendored setup so that dependencies are completely contained inside the repository structure:

```text
ModelLoader/
├── src/                  # Application source logic (.cpp)
├── include/              # Modular component definitions (.h)
├── vendor/               # Portable runtime dependencies
│   ├── include/          # Static header declarations (GLFW, GLM, GLAD)
│   └── lib/              # Precompiled static binaries (x64)
├── ModelLoader.sln       # Visual Studio Solution Workspace
└── .gitignore            # Clean environment configuration filters
