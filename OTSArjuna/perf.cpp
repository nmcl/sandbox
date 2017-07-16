Raw performance figures.

C++

time for 1000 iterations of reading 1 bytes: 6238 microseconds. (6.2 microseconds per read).
time for 1000 iterations of writing 350 bytes: 8460970 microseconds. (8.5 milliseconds per write).

Java

time to read 1000 transactions is 53 milliseconds. 53 microseconds per read.

time to write 1000 transactions is 34524 milliseconds. 34.5 milliseconds per write.

txoj performance tests are missing the timing code!

performance tests using JDBCStore are meaningless!

--

TXOJ tests using ShadowingStore

Perf1:

average time taken for a write operation over 1000 iterations: 0.041649 seconds.(41 milliseconds per transaction).

Java --->

time for 1000 write transactions is 68880 milliseconds (68 milliseconds per transaction).

----

Raw transactions tests

creating/aborting transactions

C++: Average time to create, start and abort a transaction 0.003 milliseconds.

Java: time for 1000 write transactions is 3326 milliseconds (3.326 milliseconds per transaction).

--

creating/registering/committing transactions

C++: Average time to create, start and commit a transaction 32.1 milliseconds (with ShadowingStore).
C++: Average time to create, start and commit a transaction 8 milliseconds (with ActionLogStore). Though increases as number of transactions increase (log size increases).

C++: Average time to create, start and commit a transaction 8 milliseconds (with ActionLogStore). Multi-threaded. Time remains constant as the number of transactions increases.

Java: time for 1000 write transactions is 12190 milliseconds (12.2 milliseconds per transaction).
