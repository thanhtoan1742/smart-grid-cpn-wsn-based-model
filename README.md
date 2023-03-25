# Smart Grid Using WSN Based Model

## Build Instructions
Run cmake from the root directory
```
cmake -S src -B build
```
Then build
```
cmake --build build
```

### Export compile command
```
cmake -S src -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cp build/compile_commands.json .
```