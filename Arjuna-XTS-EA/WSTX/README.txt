The build script now deploys  configuration files within the final jar
so that  they can be  read when deployed  in the SOAP server.  Look at
wstx.xml and  wst.xml in the  xml directory before building  to ensure
that the values  are correct. The only ones you  should need to change
are   ArjunaCore  values   and   the  type   of  UserTransaction   and
TransactionManager implementation.
