ZobDB
=====

ZobDB is a simple key-value database with a permanent storage.

IT IS NOT READY FOR PRODUCTION USE.

Build
-----

Building ZobDB requires GNU make and libraries for Berkley DB (package libdb-dev under
Debian based Linux distributions).

The compilation can be done through GCC and LLVM Clang under Debian GNU/Linux Wheezy. Other
compilers are not yet supported.

Unit tests request CGreen unit test tool. As it is installed, just lauch the tests:

        make tests

Usage
-----

Launch from command line the daemon:

        ./zobd

To stop the server:

        echo -e stop | ./zob

Create a new entry:

        echo -e new $key $value | ./zob

Update an existing entry:

        echo -e update $key $value | ./zob

Read an entry:

        echo -e read $key | ./zob
