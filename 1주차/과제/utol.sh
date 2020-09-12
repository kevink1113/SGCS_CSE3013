#!/bin/bash

echo "Working directory:"
read directory

if [ $directory ]
then
	cd $directory
	if [ $? -ne 0 ]
	then
		echo "Error: Cannot access directory, check permissions"
		exit 0
	fi
fi

for filename in *
do
	if [ -f $filename ]
	then
		mv $filename `echo $filename | tr '[A-Z][a-z]' '[a-z][A-Z]'`	
	fi
done
