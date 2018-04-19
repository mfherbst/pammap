#!/bin/bash

for f in *.generate.py; do
	BASE=$(basename $f .generate.py)
	if [ "$f" -nt "$BASE" ]; then
		echo "Running $f"
		./$f
	fi
done
