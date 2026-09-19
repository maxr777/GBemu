#!/bin/bash
set -e

run_cpu=false

for arg in "$@"; do
    case "$arg" in
        --cpu)
            run_cpu=true
            ;;
        *)
            echo "Unknown argument: $arg" >&2
            echo "Usage: $0 [--cpu]" >&2
            exit 2
            ;;
    esac
done

clear
./build.sh

if [ "$run_cpu" = true ]; then
    ./tests/cpu_test_runner
fi

./tests/test_runner
./GBemu
