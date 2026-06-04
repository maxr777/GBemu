#!/bin/bash
set -e

./build.sh
clear
./tests/test_runner
./GBemu
