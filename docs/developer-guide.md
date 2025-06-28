# Developer Guide

<p>
The following are instructions for building the code from source. This is mainly targeted for developers looking to make code changes and running from source. This is not the documentation for using the software.
</p>

<p>
Developers can setup their environment with all the necessary dependencies or the provided docker files can be used to create development containers. See the "Using Containers" section for instructions on using containers.
</p>

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
apt install libzmq3-dev
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

## Using Containers

<p>
The following instructions assume docker but other containerization software can be made to work.
</p>

<p>
Run the following script to build the docker images.
</p>

```
# From the /containers dir:
sudo ./build-dev-image.sh
```

<p>
This will create the development image which has all the build dependencies installed. Next create a container based off of the image using the following command:
</p>

```
# docker create \
#    --interactive \
#    --tty \
#    --mount type=bind,src=../.,dst=/app \
#    --network host \
#    --name sensor-processor-dev \
#    sensor-processor-dev:1.0
#
# Copy paste version:
docker create --interactive --tty --mount type=bind,src=../.,dst=/home/developer/SensorProcessor --network host --name sensor-processor-dev sensor-processor-dev:1.0
```

<p>
Double check or list your created container with the following:
</p>

```
docker ps -a
```

<p>
From here you can start and terminal into your container. VS code provides a nice setup via dev containers. Navigate to remote explorer (requires remote development extension). Select dev containers as the target.

TODO: VSCode Insructions
</p>

<p>
Optionally you can simply shell into the container with the following commands:

</p>

```
TODO: Need to handle folder permissions better.
# Start the container
docker start sensor-processor-dev

# Execute interactive terminal on conainer
docker exec -it sensor-processor-dev sh

# CD into mounted source code directory
cd ~/SensorProcessor && ls
```