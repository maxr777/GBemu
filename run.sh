#!/bin/bash
set -e

clear
./build.sh
./tests/test_runner
./GBemu
