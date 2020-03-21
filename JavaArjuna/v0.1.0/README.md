When building make sure to install imake and set CLASSPATH to the classes directory at the root and in the tests subdirectory if you're going to build them. You may see some instances of the following warning message, which you can safely ignore.

"Note: Some input files use or override a deprecated API.
Note: Recompile with -Xlint:deprecation for details."

Or

"/classes/com/arjuna/JavaArjuna/Template/BasicList.java:32: warning: no @param for t
public synchronized void add (Object t)"

On the Mac there's some weirdness with the Xcode version of cpp and string substitutions during pre-processing (even the
standard # doesn't seem to work). So added the ability to override cpp during configure and use the official gcc version (gcc-8
in my current build) and that works fine. See https://github.com/nmcl/sandbox/issues/80 Running the configure script should now
auto-detect the fact that gcc-8 should be used on Mac OS X but you can override this during the configuration execution.

When cleaning up a build by running 'make purge' make sure to do this in the tests directory first.

----

JavaArjuna requires a  minimum object store to save/load  the states of
transaction  logs and persistent  created objects.  The default  location for
this  is the  root  of  the JavaArjuna  installation  (the system  will
automatically  create  an   ObjectStore  subdirectory).   For  further
details, see the documentation.

If you want to override the location of the object store, then use the
OBJECTSTORE_DIR property  variable, either on the command  line, or in
the properties file.
