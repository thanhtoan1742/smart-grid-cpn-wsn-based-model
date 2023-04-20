# Smart Grid Using WSN Based Model

## Build Instructions
Use cmake to build the project
```
cmake -S . -B build
cmake --build build
```

### Export compile command
```
cmake -S . -B build -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
cp build/compile_commands.json .
```
