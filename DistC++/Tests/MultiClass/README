This test demonstrates  the multiserver utility, which allows multiple
classes   per server.   Note, such  servers   cannot be used  with the
manager, i.e., they must be started by hand.

When starting up such a server, it is necessary  for each object class
manager  to give its port   number, so that  this  can be given to the
clients of that class manager. (Typically this information is obtained
from  the rpc manager.)  This information is  output by specifying the
NO_MANAGER  option on the command line.  (Other options, such as DGRAM
or STREAM are also possible to  specify the communications protocol to
use.)

To run:

start the servercode 'servercode NO_MANAGER'

take the two port numbers and pass them to Tester 'Tester -bufferport 43684 -dummyport 43685'
