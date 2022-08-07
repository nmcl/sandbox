The MonitorDaemon code. Sets up an application level ping.

Start the pingdaemon (-help to see options).

Create configuration file in /var/tmp/monitor.conf with format <host> <port> for machines to monitor (one per line). Then start the MonitorDriver and it will use the file to ping each machine periodically.

If the MonitorDriver is run with multiple iterations (-iter option) then invoke Download with the relevant hostname to receive the contents of that machine's /var/tmp/monitor.out file.

If a client sends the TERMINATE_CODE value to the pingdaemon and/or MonitorDriver then those services will be shutdown automatically.

This code builds.
