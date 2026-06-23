#!/bin/bash
set -e

clear
./build.sh
# ./tests/cpu_test_runner
./tests/test_runner
./GBemu
