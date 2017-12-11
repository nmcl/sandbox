WS-C stuff
==========

Prerequisites
-------------

- ant 1.5.1 (ant-1.4 & 1.5 could also work).
- Sun's jwsdp-1.0 (for JAXM support)
- Apache's axis-1.0 & tomcat-4.1.12 (for Axis support)
- Sun's jdk-1.3.1 & 1.4.1 (jdk-1.3.0 & 1.4.0 could also work).

Source structure
----------------

+---com
    +---arjuna
        \---wsc
            +---deploy
            |   \---servlet
            +---dispatcher
            |   \---deploy
            |       \---servlet
            +---impl
            |   +---dispatcher
            |   \---messaging
            |       \---saaj
            \---messaging
                +---axis
                +---deploy
                |   \---servlet
                \---jaxm

Installation for JAXM
---------------------

From WS-C directory 

- edit "build.xml" to reflect the location of your jwsdp installation.
                   ensure "support.jaxm" is "true" and "support.axis" is "false".
- issue command "ant tests-deploy"
- ensure jwsdk's web-server listens on port 80.
- start jwsdk's running (bin/startup[.sh/.bat]).
- point browser at "http://localhost/ws-c-tests/index.html".
- press run, then reload a few times, to see if any of the tests pass.

Installation for Axis
---------------------

From WS-C directory 

- edit "build.xml" to reflect the location of your axis & tomcat installations.
                   ensure "support.axis" is "true" and "support.jaxm" is "false".
- ensure tomcat listens on port 80.
- start tomcat running (bin/startup[.sh/.bat]).
- issue command "ant tests-deploy"
- shutdown tomcat running (bin/shutdown[.sh/.bat]).
- start tomcat running (bin/startup[.sh/.bat]).

- wonder what to do next.

Know issues
-----------

- No receive timeouts.
- Activation service does not support current context.
- Error codes are sent a number not "qualified names".
- Axis responds with an empty soap message.
- ...

To do list
----------

- Add comments.
- Pool of dispatcher threads.

Copyright
---------

Copyright (c) 2002, 2003, Arjuna Technologies Limited.
