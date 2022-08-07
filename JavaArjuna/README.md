JavaArjuna was the world's very first 100% pure Java transaction implementation. It predates JTA, JTS and was available
soon after the first version of Java (aka Oak) was released.

v0.1.0 version is an attempt to recreate the original from current
sources. Since the original JavaArjuna was not based on Gandiva (as
far as I can recall), I haven't gone down that route. Does include some
extensions that were not present in the real version and I may remove
them later. https://github.com/nmcl/sandbox/issues/113

v0.2.0 builds on 0.1.0 and adds a few more classes and other code that
was probably not in the original but could have been.

v1.0.0.version is not the original source or binary but some partial
installation of the 1.0.0 release which was based on some ArjunaII
work, specifically Gandiva.

See specific READMEs in the sub-directories for further information.

JavaArjuna referenced in http://www.cs.ncl.ac.uk/research/pubs/trs/papers/651.pdf

The code builds.
