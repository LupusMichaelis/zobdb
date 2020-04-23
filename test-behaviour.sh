#!/bin/bash

EXEPATH=.

ZOB=zob
ZOBD=zobd

SOCKET=con
DATA=data

LOGDIR=tests/log
STDOUT=$LOGDIR/behaviour-out.log
STDERR=$LOGDIR/behaviour-error.log

main()
{
	action=$1
	shift

	case $action in
		behaviour)
			echo mkdir -p $LOGDIR
			echo rm -f $SOCKET $DATA $STDERR $STDOUT
			echo $EXEPATH/$ZOBD 2> $STDERR > $STDOUT
			;;
		unit)
			make tests $@
	esac
}

main $@
