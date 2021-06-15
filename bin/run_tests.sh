#!/bin/bash
cd $(dirname $0)
set -u

for i in ../test_data/input/*.DAT
do
    ./himawari_io $i ../test_data/output
    exit_code=$?

    if [[ $exit_code -ne 0 ]]
    then
        echo "ERROR: File $i failed comparision and returned code $exit_code"
        exit 1
    fi
done
