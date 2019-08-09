# multiqueue
Simple mutithreaded queue test program

To build:
make

Usage: ./build/queue_test [send|recv thread count]

Usage example:
./build/queue_test 3

Result:

Receive message with thread uid = 139893543708416
Receive message with thread uid = 139893552101120
Receive message with thread uid = 139893535315712
Send message from thread with uid = 139893526923008
Send message from thread with uid = 139893510137600
Received message from thread with uid = 139893526923008
Received message from thread with uid = 139893510137600
Send message from thread with uid = 139893518530304
Received message from thread with uid = 139893518530304
