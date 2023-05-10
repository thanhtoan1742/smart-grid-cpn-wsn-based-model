# Smart Grid Using WSN Based Model

## Build
Use cmake to build the project
```
cmake -S . -B build
cmake --build build
```

## Test
After building with cmake, run the `tests` executable.

### Export compile command
```
cmake -S . -B build -G Ninja -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -W no-dev
cp build/compile_commands.json .
```
