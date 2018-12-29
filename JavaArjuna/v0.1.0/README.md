This is not the original JavaArjuna but something I tried to recreate
from current sources and memory. The baseline source is the earliest
version of JTSArjuna I could find, which is also in the repository elsewhere.

This is hear not for future development (for that, find a version of Narayana) but
so that we don't lose a point in time which is important from a historical perspective.

Differences to the original:

- The build system is not going to be exactly the same as I had in the
1995/1996 build but that was never the important part of the system. It's also
possible that I'll hack a pre-existing build system just to get the code to
build rather than spend a lot of time (initially or at all) to make the build
system the way it was and extensible.

- Some of the code was updated with bug fixes, new features etc.

- The package names are going to be closer to JTSArjuna than they were
  in the original code simply because I can't recall what I used orignally.

- The build system is just enough to create the necessary components but may be closer
to JavaArjunaLite than the original. It is currently based on the W3OTrans JavaStubGen
repository as that's the oldest non-Gandiva Java build system I can find. Gandiva came in later.

- This is built with the latest version of Java and there may be differences between that and Oak, which was the version used originally. Where possible I've tried not to use more advanced language features but I may have missed some opportunities.

- Some classes may be in different packages to where they were originally. Sometimes it was personal preference at the time.

- Use JAVAARJUNA_HOME to locate property file. https://github.com/nmcl/sandbox/issues/73

- Some tidying up, specifically around deprecated features and warnings, e.g., finalize usage. Some of these are commented out, some deleted.

----

When building make sure to install imake and set CLASSPATH to classes directory. Also make sure to set JAVAARJUNA_HOME to the installation directory where the build artefacts will eventually go.
