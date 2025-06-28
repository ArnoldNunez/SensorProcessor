#!/bin/bash

# Script Name: build-dev-image.sh
# Description: Build docker development images
# Author: Arnold Nunez
# Version: 1.0

# Create the base development image
echo "*** Creating Base Image ***"
docker build --file dockerfile-dev-base \
 --tag sensor-processor-dev-base:1.0 .

echo 
echo 

docker image list

# Create the application development image
docker build --file dockerfile-dev \
 --tag sensor-processor-dev:1.0 .

# Clean up
docker image rm sensor-processor-dev-base:1.0

echo
echo Build Complete!
echo
docker image list