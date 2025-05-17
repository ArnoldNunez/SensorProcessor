# SensorProcessor
The core application for processing sensor inputs

## Build Instructions

<p>
This application uses protobuff files that need to be compiled into c++ code. The script 'compile_protos.sh' will compile the protos and place them in the expected directory for you.
</p>


```
mkdir build && cd build
cmake ..
make
```