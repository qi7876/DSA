#!/bin/bash

cd ./build/ && rm -rf ./*
cmake ..
make
cp ./JPEGCompressor ../
