#!/bin/sh

find -iname "*.generate.py" | while read script; do
	echo "Running $script"
	$script
done
