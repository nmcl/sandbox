Using ActionLogStore

With Posix thread:

Performance1

average time taken for a write operation over 1000 iterations: 0.024464 seconds.

Performance2

average time taken for a read operation over 1000 iterations: 2.1e-05 seconds.

Performance3

Average time to create, start and abort a transaction 0.007 milliseconds.

Performance4

time for 1000 write transactions is 9041 milliseconds
time for 10000 write transactions is 89248 milliseconds

--

Without threads:

Performance1

average time taken for a write operation over 1000 iterations: 0.02464 seconds.

Performance2

average time taken for a read operation over 1000 iterations: 1.4e-05 seconds.

Performance3

Average time to create, start and abort a transaction 0.001 milliseconds.

Performance4

time for 1000 write transactions is 9243 milliseconds
time for 10000 write transactions is 165691 milliseconds

