# pattern-generation-lib

This is a simple library to generate textures with given pattern templates, namely flat, gradient, checkerboard and Perlin noise.
These textures are fully compatible with [Gazebo] robotics simulator and consist of a generated image file and material description.

This repository was originally designed as a support tool for [GAP] - a set of tools to interact programatically with Gazebo for automatic dataset generation.

### Dependencies

The library uses OpenCV 3.

### Compilation

Simply clone the repository and build from source, with a traditional CMake build procedure.

### Generate textures

We provide a test script to generate textures:
```
usage:   ./build/pattern_generation_test [options]
options: -n <number of textures generate>
         -i <index of the first texure>
         -d <output directory>
         -t <texture type>
         -r <image resolution>
```

[Gazebo]: http://gazebosim.org/
[GAP]: https://github.com/jsbruglie/gap/
