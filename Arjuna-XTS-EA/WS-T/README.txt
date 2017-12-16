WS-T stuff
==========

Prerequisites
-------------

- ant 1.5.1 (ant-1.4 & 1.5 could also work).
- Sun's jwsdp-1.0 (for JAXM support)
- Sun's jdk-1.3.1 & 1.4.1 (jdk-1.3.0 & 1.4.0 could also work).

Installation for JAXM
---------------------

From WS-T directory 

- edit "build.xml" to reflect the location of your jwsdp installation.
                   ensure "support.jaxm" is "true" and "support.axis" is "false".
- issue command "ant tests-deploy"
- ensure jwsdk's web-server listens on port 80.
- start jwsdk's running (bin/startup[.sh/.bat]).
- point browser at "http://localhost/ws-t-tests/index.html".
- press run, then reload a few times, to see if any of the tests pass.

Know issues
-----------

- No message correlation!
- No receive timeouts.

To do list
----------

- Add comments.

Copyright
---------

Copyright (c) 2003, Arjuna Technologies Limited.
