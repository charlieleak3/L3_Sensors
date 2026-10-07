cat << 'EOF' > CONTRIBUTING.md
# Contributing to L3_Sensors

## Adding a New Sensor Driver
1. **Public Header**: Place header in `include/sensors/<sensor_name>.h`. Wrap C functions with `extern "C"` blocks for C++ compatibility.
2. **Implementation**: Place source code in `src/<sensor_name>.c` or `src/<sensor_name>.cpp`.
3. **Example Demo**: Create a small, standalone main program inside `examples/<sensor_name>_demo/main.cpp`.
4. **Formatting**: Ensure files are formatted using the repository `.clang-format`.
EOF