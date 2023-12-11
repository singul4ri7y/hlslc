<h1 align="center">High-Level Shading Language Compiler</h1>

A simple compiler for High-Level Shading Language, similar to Google's **glslc**, which compiles OpenGL shaders.

## Purpose of this project: 
<p align="justify">I wrote this compiler back in the day when I was doing Game Dev. and writing a Game Engine for myself. This compiler was designed to run against hardcore Windows favoring GCC compilers like <b>TDM GCC</b> (I know, I know...). There were already a shader compiler for HLSL and it was available in Visual Studio through VS Tools. I was really pissed off on Visual Studio (or maybe Windows, I forgot) and there was no compiler available to compile HLSL code directy (didn't want to use VS Tools either, which will kill the point). So, I decided to write one myself in C++ using the Direct3D API.</p>

## How to use it: 
Unlike Windows Dev. tools, the compiler is pretty straight forward.

- Use `-O` switch to specify the output file. E.g. `-O vertex.cso`. You can also specify multiple output files with different name, but the output content will be the same. E.g. `-O vertex.cso vertex_final.cso`.
- Use `-E` switch to specify the entry point to the shader. E.g. `-Evert_entry`.
- Use `-D` to add macro definitions (constants). E.g. `-D ANY_CONSTANT_NAME_IT=value,IN_DEBUG=true`.
- Use `-DEBUG` to compile it in debug mode.
- Use `-T` to specify target shader version. E.g. `-Tps_5_0`. Learn more [here](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/specifying-compiler-targets).
- Finally use `-I` to provide input files (or provide them before using any of the switches prior mentioned).

## Example use:
Say, if I want to compile a Geometry Shader named `default_geometry.hlsl` in debug mode, targeting shader version 5.0 and want to generate two files named `default_geometry.cso` and `geometry.cso` respectively, which has entry point (main function) name `voila()` I would use the command like following: 
```bash
hlslc -I default_geometry.hlsl -DEBUG -D DEBUG=true -Tgs_5_0 -Evoila -O default_geometry.cso geometry.cso
```
<i><b>Peace.</b></i>
