# Cycles Shader Editor

Cycles Shader Editor is a cross-platform C++ library that provides a graphical editor for creating [Cycles](https://www.cycles-renderer.org/) shader graphs with a simple drag and drop interface. It is intended to be used as part of a plugin that integrates Cycles with a DCC app and was created as part of [my own plugin for 3ds Max](https://cyclesformax.net/).

This version is a ground-up rewrite that I hope will correct many of the issues present in the [original Cycles Shader Editor](https://github.com/jlwitthuhn/cycles-shader-editor) that caused it to be difficult to maintain and extend.

I hope to make this version better in two primary ways:
* The GUI is now entirely based on Dear ImGui rather than the custom NanoVG GUI used by the old shader editor.
* The code now contains a clean seperation between logic related to the editor itself (src/editor) and logic for manipulating graphs and nodes (src/graph).

This version is still incomplete.

## Prerequisites

This should build on any plaform that GLFW builds on, but I have only tested it with Visual Studio 2015, Visual Studio 2017, GCC 9.3.1, and Clang 9.0.1.

Libraries you will need are:
- GLFW 3.x
- Boost
- Dear ImGui
  - A compatible version is bundled in this repository, but any moderately recent version should work.


## License

This project is available under the GPLv2 (or later) license. The full text of the license is available in [LICENSE](LICENSE)
