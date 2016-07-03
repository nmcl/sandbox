The MonitorDaemon code. Sets up application level ping.

Start the pingdaemon (-help to see options).

Create configuration file in /var/tmp/monitor.conf with format <host> <port> for machines to monitor (one per line). Then start the MonitorDriver and it will use the file to ping each machine periodically.