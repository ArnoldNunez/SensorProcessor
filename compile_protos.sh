#!/bin/bash

# protoc -I=protos --cpp_out=protos protos/addressbook.proto
if [ ! -d "./generated" ]; 
then
mkdir generated
fi

protoc -I=protos --cpp_out=generated protos/*.proto protos/DiagnosticsService/*.proto protos/SessionService/*.proto
    
echo "Generation complete!"