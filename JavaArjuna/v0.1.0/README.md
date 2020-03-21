This is not the original JavaArjuna but something I tried to recreate
from current sources and memory. The baseline source is the earliest
version of JTSArjuna I could find, which is also in the repository elsewhere.

This is here not for future development (for that, find a version of Narayana) but
so that we don't lose a point in time which is important from a historical perspective.

Differences to the original:

- The build system is not going to be exactly the same as I had in the
1995/1996 build but that was never the important part of the system. It's also
possible that I'll hack a pre-existing build system just to get the code to
build rather than spend a lot of time (initially or at all) to make the build
system the way it was and extensible.

- Some of the code taken as the basis of this work was updated with bug fixes, new features etc. The basis for this code is
the earliest version of JTSArjuna available (1.2.2). That is based on Gandiva and the original JavaArjuna was not so it required
some backing out of Gandiva. Gandiva took on the role of dynamic interface/implementation separation and binding but initially the types
were fairly statically maintained, as they were in the original Arjuna. This was probably defined within the various implementations
themselves but for this re-write I'll use something more structured and referenced with https://github.com/nmcl/sandbox/issues/79 And in
some cases it may still look closer to Gandiva than the original JavaArjuna.

- The package names are going to be closer to JTSArjuna than they were
  in the original code simply because I can't recall what I used in 1995.

- The build system is just enough to create the necessary components but may be closer
to JavaArjunaLite than the original. It is currently based on the W3OTrans JavaStubGen
repository as that's the oldest non-Gandiva Java build system I can find. Gandiva came in later. Furthermore, some of the rules used
by the Imakefiles may be closer to JTSArjuna than JavaArjuna given the strong JavaGandiva influence on the initial
codebase used here.

- This is built with the latest version of Java and there may be differences between that and Oak, which was the version used originally.
Where possible I've tried not to use more advanced language features but I may have missed some opportunities.

- Some classes may be in different packages to where they were originally. Sometimes it was personal preference at the time.

- Some tidying up, specifically around deprecated features and warnings, e.g., finalize usage. Some of these are commented out, some deleted.
Some I didn't change at all because it would affect the rest of the code too much and take it much further away from what JavaArjuna looked
like at the start. Arguably none of these changes should have been made and the warnings continue so the code is as close as possible. In fact upon reflection I decided not to change any further deprecated code to try to keep the code as pure to the original as possible. Rather than revert the current list of deprecated code, I decided to leave as is because it's just various finalize and minor code changes. Most (all?) of those changes have a suitable comment next to them anyway so people can see what was there originally if needed.

----

When building make sure to install imake and set CLASSPATH to classes directory. You may see some instances of the following warning message, which you can safely ignore.

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
