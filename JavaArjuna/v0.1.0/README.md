This is not the original JavaArjuna but something I tried to recreate
from current sources and memory. The baseline source is the earliest
version of JTSArjuna I could find, which is also in the repository elsewhere.

Differences to the original:

- The build system is not going to be exactly the same as I had in the
1995/1996 build but that was never the importsant part of the system.

- Some of the code was updated with bug fixes, new features etc.

- The package names are going to be closer to JTSArjuna than they were
  in the original code simply because I can't recall what I used orignally.

- The build system is just enough to create the necessary components but may be closer
to JavaArjunaLite than the original. Is currently based on the W3OTrans JavaStubGen
repository as that's the oldest non-Gandiva Java build system I can find.

- This is build with the latest version of Java and there may be differences between that and Oak, which was the version used originally. Where possible I've tried not to use more advanced language features but I may have missed some opportunities.
