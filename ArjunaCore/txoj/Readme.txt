->	Configuration properties

The Ant properties that can be set (and their defaults and possible values)
are :

	Property				Values

	com.hp.mw.ts.properties			product.properties **
	(property file to load properties	(any filename)
	from) 

	com.hp.mw.ts.date			YYYY/MM/DD hh:mm **
	(date of build)				(any string)

	com.hp.mw.ts.installationdirectory	install **
	(installation directory)		(any directory) [1]

	com.hp.mw.ts.sourceid			unknown **
	(source identifier)			(any string)

	com.hp.mw.ts.version			unknown **
	(product version)			(any string)

	com.hp.mw.ts.builder			Hewlett Packard [user] (OS) **
	(product builder)			(any string)

	com.hp.mw.ts.notes			<empty> **
	(any notes)				(any string)

	com.hp.mw.ts.txoj.tests.compile	no **
	(compile tests)				yes

	com.hp.mw.ts.txoj.tests.install	no **
	(install tests)				yes

	com.hp.mw.ts.txoj.utilities.compile	yes **
	(compile utilities)			no

	com.hp.mw.ts.txoj.utilities.install	yes **
	(install utilities)			no


->	Build targets

The txoj 'build.xml' file contains the targets :

	Long name				Short name

	com.hp.mw.ts.txoj.compile		compile
	(compile java classes)

	com.hp.mw.ts.txoj.jar			jar **
	(generate module jar file)

	com.hp.mw.ts.txoj.install		install
	(install module)

	com.hp.mw.ts.txoj.clean		clean
	(clean generated files)


 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  $Id: Readme.txt,v 1.5 2002/03/04 13:31:05 rbegg Exp $
