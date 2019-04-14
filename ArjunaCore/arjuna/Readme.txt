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

	com.hp.mw.ts.arjuna.objectstore.cloudscape36.compile
	(add Cloudscape 3.6 support)		no **
						yes

	com.hp.mw.ts.arjuna.objectstore.sequelink51.compile
	(add Sequelink 5.1 support)		yes **
						no

	com.hp.mw.ts.arjuna.objectstore.oracle81.compile
	(add Oracle 8.1 support)		no **
						yes

	com.hp.mw.ts.arjuna.objectstore.oracle81.compile
	(add Oracle 9.0 support)		no **
						yes

	com.hp.mw.ts.arjuna.tests.compile	no **
	(compile tests)				yes

	com.hp.mw.ts.arjuna.tests.install	no **
	(install tests)				yes

	com.hp.mw.ts.arjuna.utilities.compile	yes **
	(compile utilities)			no

	com.hp.mw.ts.arjuna.utilities.install	yes **
	(install utilities)			no


->	Build targets

The arjuna 'build.xml' file contains the targets :

	Long name				Short name

	com.hp.mw.ts.arjuna.compile		compile
	(compile java classes)

	com.hp.mw.ts.arjuna.jar			jar **
	(generate module jar file)

	com.hp.mw.ts.arjuna.install		install
	(install module)

	com.hp.mw.ts.arjuna.clean		clean
	(clean generated files)

	
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  $Id: Readme.txt,v 1.4 2002/03/04 13:23:31 rbegg Exp $
