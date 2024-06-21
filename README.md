# Simple OpenGL Renderer

This repository contains source files for my toy OpenGL Renderer project

## Supported features
- Diffuse/Specular/Normal/Emission maps
- Own models loading
- Point light, spotlight, directional light
- Gamma correction
- Uniform Buffer Objects (UBO) for light
- Hierarchical modelling

## Running
To successfully build and run this project, you need these libraries installed:
1. GLM
2. ASSIMP
3. GLFW
4. OpenGL **4.1**
5. CMake for building

Also, you need to generate GLAD files for your platform and place them inside `include/` fodler. Provided configuration is set up for MacOS under Apple M2 chip
1. Run inside the root of the project (or other desired folder): 
```bash
cmake .
make
```
This will generate the executable


## Credits

- Many thanks to [LearnOpenGL](https://learnopengl.com) for excellent free materials
- ["Witcher 3-Geralt"](https://skfb.ly/6sIyL) by biswayan is licensed under [Creative Commons Attribution-NonCommercial](http://creativecommons.org/licenses/by-nc/4.0/).
- ["Dark Souls - Bonfire"](https://skfb.ly/6ztTx) by UselessViking is licensed under [Creative Commons Attribution](http://creativecommons.org/licenses/by/4.0/).
