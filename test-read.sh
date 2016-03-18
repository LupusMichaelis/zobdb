#!/bin/bash

for n in $(seq 1 100000)
do
	echo -e "read b$n" | ./zob
	if [ -x $? ]
	then
		break
	fi
done

