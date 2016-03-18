#!/bin/bash

for n in $(seq 1 100000)
do
	echo -e "new b$n Value $n" | ./zob
	if [ -x $? ]
	then
		break
	fi
done

