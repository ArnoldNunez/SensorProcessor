# SensorProcessor
The core application for processing sensor inputs

## Build Instructions

<p>
This application uses protobuff files that need to be compiled into c++ code. The script 'compile_protos.sh' will compile the protos and place them in the expected directory for you. ZeroMq is also needed for messaging between clients and servers.
</p>

### Installing Protobuf

<p>
You can install the protobuf compiler via your OS package mananger or you can build from source.
</p>

```
# Istall on Ubuntu
apt install protobuf-compiler libprotobuf-dev
```

### Compiling protobuf files

<p>
Run the compile the protos script to generate the necessary c++ classes.
</p>

```
./compile_protos.sh
```

### Installing ZeroMQ
<p>
You can install the ZeroMQ development packages via your OS package manager our you can build from source.
</p>

```
# Install on Ubuntu
apt-get install libzmq3-dev
```

### Building

<p>
Build the source code.
</p>

```
mkdir build && cd build
cmake ..
make
```

## Running The Application

<p>
The application reads the AppConfig.json on startup to determine the client/server configurations. The file can be copied to the build directory if you wish to run from there. To run the application run the following commands:
</p>

```
cd build
./SensorProcessor
```