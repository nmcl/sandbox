#
# Copyright (C) 2001,
#
# Hewlett-Packard Company,
# Newcastle upon Tyne,
# Tyne and Wear,
# UK.
#

#
# $Id: Makefile,v 1.1.2.2.4.1.6.1.4.2 2001/07/19 14:20:13 nmcl Exp $
#

MAKE	= make
IMAKE	= imake
RM	= rm -f
RMDIR	= rmdir

all: ModuleBuildSystem JavaHarness ArjunaCommon ArjunaCore OrbPortability \
	  CosServices/CosTransactions CosServices/CosRecovery Resources/JDBC \
	  Resources/JDBC2 JTSArjuna TeT


boot: boot-ModuleBuildSystem boot-JavaHarness boot-ArjunaCommon \
	  boot-ArjunaCore boot-OrbPortability \
	  boot-CosServices/CosTransactions boot-CosServices/CosRecovery \
	  boot-Resources/JDBC boot-Resources/JDBC2 boot-JTSArjuna boot-TeT

install: install-ModuleBuildSystem install-JavaHarness install-ArjunaCommon \
	  install-ArjunaCore install-OrbPortability \
	  install-CosServices/CosTransactions install-CosServices/CosRecovery \
	  install-Resources/JDBC install-Resources/JDBC2 install-JTSArjuna \
	  install-TeT

clean: clean-ModuleBuildSystem clean-JavaHarness clean-ArjunaCommon \
	  clean-ArjunaCore clean-OrbPortability \
	  clean-CosServices/CosTransactions clean-CosServices/CosRecovery \
	  clean-Resources/JDBC clean-Resources/JDBC2 clean-JTSArjuna clean-TeT

tidy: tidy-ModuleBuildSystem tidy-JavaHarness tidy-ArjunaCommon \
	  tidy-ArjunaCore tidy-OrbPortability \
	  tidy-CosServices/CosTransactions tidy-CosServices/CosRecovery \
	  tidy-Resources/JDBC tidy-Resources/JDBC2 tidy-JTSArjuna tidy-TeT

purge: purge-ModuleBuildSystem purge-JavaHarness purge-ArjunaCommon \
	  purge-ArjunaCore purge-OrbPortability \
	  purge-CosServices/CosTransactions purge-CosServices/CosRecovery \
	  purge-Resources/JDBC purge-Resources/JDBC2 purge-JTSArjuna purge-TeT
	(cd config && ${RM} toplevel.answers toplevel.cf)
	${RMDIR} config
	${RM} unpackcf

pack:
	./packcf

modulebuildsystem:: ModuleBuildSystem
ModuleBuildSystem: boot-ModuleBuildSystem
	(cd ModuleBuildSystem && \
	    echo Making all in ModuleBuildSystem ...; \
	    ${MAKE} ${MAKEFLAGS})

install-modulebuildsystem:: install-ModuleBuildSystem
install-ModuleBuildSystem: boot-ModuleBuildSystem
	(cd ModuleBuildSystem && \
	    echo Making install in ModuleBuildSystem ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-ModuleBuildSystem: ModuleBuildSystem/config/ModuleBuildSystem.cf
	( cd ModuleBuildSystem && \
	    echo Making bootstrap in ModuleBuildSystem ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

ModuleBuildSystem/config/ModuleBuildSystem.cf:
	@(echo "*** You must configure ModuleBuildSystem first"; exit 1)

clean-modulebuildsystem: clean-ModuleBuildSystem
clean-ModuleBuildSystem:
	( cd ModuleBuildSystem && make clean )

tidy-modulebuildsystem: tidy-ModuleBuildSystem
tidy-ModuleBuildSystem:
	( cd ModuleBuildSystem && make tidy )

purge-modulebuildsystem: purge-ModuleBuildSystem
purge-ModuleBuildSystem:
	( cd ModuleBuildSystem && make purge )

javaharness:: JavaHarness
JavaHarness: boot-JavaHarness
	(cd JavaHarness && \
	    echo Making all in JavaHarness ...; \
	    ${MAKE} ${MAKEFLAGS})

install-javaharness:: install-JavaHarness
install-JavaHarness: boot-JavaHarness
	(cd JavaHarness && \
	    echo Making install in JavaHarness ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-JavaHarness: JavaHarness/config/JavaHarness.cf
	( cd JavaHarness && \
	    echo Making bootstrap in JavaHarness ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

JavaHarness/config/JavaHarness.cf: ModuleBuildSystem/config/ModuleBuildSystem.cf
	@(echo "*** You must configure JavaHarness first"; exit 1)

clean-javaharness: clean-JavaHarness
clean-JavaHarness:
	( cd JavaHarness && make clean )

tidy-javaharness: tidy-JavaHarness
tidy-JavaHarness:
	( cd JavaHarness && make tidy )

purge-javaharness: purge-JavaHarness
purge-JavaHarness:
	( cd JavaHarness && make purge )

arjunacommon: ArjunaCommon
ArjunaCommon: boot-ArjunaCommon
	(cd ArjunaCommon && \
	    echo Making all in ArjunaCommon ...; \
	    ${MAKE} ${MAKEFLAGS})

install-arjunacommon: install-ArjunaCommon
install-ArjunaCommon: boot-ArjunaCommon
	(cd ArjunaCommon && \
	    echo Making install in ArjunaCommon ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-ArjunaCommon: ArjunaCommon/config/ArjunaCommon.cf
	( cd ArjunaCommon && \
	    echo Making bootstrap in ArjunaCommon ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

ArjunaCommon/config/ArjunaCommon.cf: JavaHarness/config/JavaHarness.cf
	@(echo "*** You must configure ArjunaCommon first"; exit 1)

clean-arjunacommon: clean-ArjunaCommon
clean-ArjunaCommon:
	( cd ArjunaCommon && make clean )

tidy-arjunacommon: tidy-ArjunaCommon
tidy-ArjunaCommon:
	( cd ArjunaCommon && make tidy )

purge-arjunacommon: purge-ArjunaCommon
purge-ArjunaCommon:
	( cd ArjunaCommon && make purge )

arjunacore: ArjunaCore
ArjunaCore: boot-ArjunaCore
	(cd ArjunaCore && \
	    echo Making all in ArjunaCore ...; \
	    ${MAKE} ${MAKEFLAGS})

install-arjunacore: install-ArjunaCore
install-ArjunaCore: boot-ArjunaCore
	(cd ArjunaCore && \
	    echo Making install in ArjunaCore ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-ArjunaCore: ArjunaCore/config/ArjunaCore.cf
	( cd ArjunaCore && \
	    echo Making bootstrap in ArjunaCore ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

ArjunaCore/config/ArjunaCore.cf: JavaHarness/config/JavaHarness.cf
	@(echo "*** You must configure ArjunaCore first"; exit 1)

clean-arjunacore: clean-ArjunaCore
clean-ArjunaCore:
	( cd ArjunaCore && make clean )

tidy-arjunacore: tidy-ArjunaCore
tidy-ArjunaCore:
	( cd ArjunaCore && make tidy )

purge-arjunacore: purge-ArjunaCore
purge-ArjunaCore:
	( cd ArjunaCore && make purge )

orbportability: OrbPortability
OrbPortability: boot-OrbPortability
	(cd OrbPortability && \
	    echo Making all in OrbPortability ...; \
	    ${MAKE} ${MAKEFLAGS})

install-orbportability: install-OrbPortability
install-OrbPortability: boot-OrbPortability
	(cd OrbPortability && \
	    echo Making install in OrbPortability ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-OrbPortability: OrbPortability/config/OrbPortability.cf
	( cd OrbPortability && \
	    echo Making bootstrap in OrbPortability ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

OrbPortability/config/OrbPortability.cf: JavaHarness/config/JavaHarness.cf
	@(echo "*** You must configure OrbPortability first"; exit 1)

clean-orbportability: clean-OrbPortability
clean-OrbPortability:
	( cd OrbPortability && make clean )

tidy-orbportability: tidy-OrbPortability
tidy-OrbPortability:
	( cd OrbPortability && make tidy )

purge-orbportability: purge-OrbPortability
purge-OrbPortability:
	( cd OrbPortability && make purge )

costransactions:: CosServices/CosTransactions
CosTransactions:: CosServices/CosTransactions
CosServices/CosTransactions: boot-CosServices/CosTransactions
	(cd CosServices/CosTransactions && \
	    echo Making all in CosServices/CosTransactions ...; \
	    ${MAKE} ${MAKEFLAGS})

install-costransactions:: install-CosServices/CosTransactions
install-CosTransactions:: install-CosServices/CosTransactions
install-CosServices/CosTransactions: boot-CosServices/CosTransactions
	(cd CosServices/CosTransactions && \
	    echo Making install in CosServices/CosTransactions ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-CosServices/CosTransactions: \
	CosServices/CosTransactions/config/CosTransactions.cf
	( cd CosServices/CosTransactions && \
	    echo Making bootstrap in CosServices/CosTransactions ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

CosServices/CosTransactions/config/CosTransactions.cf: \
	ArjunaCommon/config/ArjunaCommon.cf \
	ArjunaCore/config/ArjunaCore.cf \
	OrbPortability/config/OrbPortability.cf
	@(echo "*** You must configure CosTransactions first"; exit 1)

clean-costransactions: clean-CosServices/CosTransactions
clean-CosTransactions: clean-CosServices/CosTransactions
clean-CosServices/CosTransactions:
	( cd CosServices/CosTransactions && make clean )

tidy-costransactions: tidy-CosServices/CosTransactions
tidy-CosTransactions: tidy-CosServices/CosTransactions
tidy-CosServices/CosTransactions:
	( cd CosServices/CosTransactions && make tidy )

purge-costransactions: purge-CosServices/CosTransactions
purge-CosTransactions: purge-CosServices/CosTransactions
purge-CosServices/CosTransactions:
	( cd CosServices/CosTransactions && make purge )

cosrecovery:: CosServices/CosRecovery
CosRecovery:: CosServices/CosRecovery
CosServices/CosRecovery: boot-CosServices/CosRecovery
	(cd CosServices/CosRecovery && \
	    echo Making all in CosServices/CosRecovery ...; \
	    ${MAKE} ${MAKEFLAGS})

install-cosrecovery:: CosServices/CosRecovery
install-CosRecovery:: CosServices/CosRecovery
install-CosServices/CosRecovery: boot-CosServices/CosRecovery
	(cd CosServices/CosRecovery && \
	    echo Making install in CosServices/CosRecovery ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-CosServices/CosRecovery: CosServices/CosRecovery/config/CosRecovery.cf
	( cd CosServices/CosRecovery && \
	    echo Making bootstrap in CosServices/CosRecovery ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

CosServices/CosRecovery/config/CosRecovery.cf: \
	CosServices/CosTransactions/config/CosTransactions.cf
	@(echo "*** You must configure CosRecovery first"; exit 1)

clean-cosrecovery: clean-CosServices/CosRecovery
clean-CosRecovery: clean-CosServices/CosRecovery
clean-CosServices/CosRecovery:
	( cd CosServices/CosRecovery && make clean )

tidy-cosrecovery: tidy-CosServices/CosRecovery
tidy-CosRecovery: tidy-CosServices/CosRecovery
tidy-CosServices/CosRecovery:
	( cd CosServices/CosRecovery && make tidy )

purge-cosrecovery: purge-CosServices/CosRecovery
purge-CosRecovery: purge-CosServices/CosRecovery
purge-CosServices/CosRecovery:
	( cd CosServices/CosRecovery && make purge )

jdbc:: Resources/JDBC
JDBC:: Resources/JDBC
Resources/JDBC: boot-Resources/JDBC
	(cd Resources/JDBC && \
	    echo Making all in Resources/JDBC ...; \
	    ${MAKE} ${MAKEFLAGS})

install-jdbc:: install-Resources/JDBC
install-JDBC:: install-Resources/JDBC
install-Resources/JDBC: boot-Resources/JDBC
	(cd Resources/JDBC && \
	    echo Making install in Resources/JDBC ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-Resources/JDBC: Resources/JDBC/config/JDBC.cf
	( cd Resources/JDBC && \
	    echo Making bootstrap in Resources/JDBC ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

Resources/JDBC/config/JDBC.cf: \
	CosServices/CosTransactions/config/CosTransactions.cf
	@(echo "*** You must configure JDBC first"; exit 1)

clean-jdbc: clean-Resources/JDBC
clean-JDBC: clean-Resources/JDBC
clean-Resources/JDBC:
	( cd Resources/JDBC && make clean )

tidy-jdbc: tidy-Resources/JDBC
tidy-JDBC: tidy-Resources/JDBC
tidy-Resources/JDBC:
	( cd Resources/JDBC && make tidy )

purge-jdbc: purge-Resources/JDBC
purge-JDBC: purge-Resources/JDBC
purge-Resources/JDBC:
	( cd Resources/JDBC && make purge )

jdbc2:: Resources/JDBC2
JDBC2:: Resources/JDBC2
Resources/JDBC2: boot-Resources/JDBC2
	(cd Resources/JDBC2 && \
	    echo Making all in Resources/JDBC2 ...; \
	    ${MAKE} ${MAKEFLAGS})

install-jdbc2:: install-Resources/JDBC2
install-JDBC2:: install-Resources/JDBC2
install-Resources/JDBC2: boot-Resources/JDBC2
	(cd Resources/JDBC2 && \
	    echo Making install in Resources/JDBC2 ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-Resources/JDBC2: Resources/JDBC2/config/JDBC2.cf
	( cd Resources/JDBC2 && \
	    echo Making bootstrap in Resources/JDBC2 ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

Resources/JDBC2/config/JDBC2.cf: \
	CosServices/CosTransactions/config/CosTransactions.cf
	@(echo "*** You must configure JDBC2 first"; exit 1)

clean-jdbc2: clean-Resources/JDBC2
clean-JDBC2: clean-Resources/JDBC2
clean-Resources/JDBC2:
	( cd Resources/JDBC2 && make clean )

tidy-jdbc2: tidy-Resources/JDBC2
tidy-JDBC2: tidy-Resources/JDBC2
tidy-Resources/JDBC2:
	( cd Resources/JDBC2 && make tidy )

purge-jdbc2: purge-Resources/JDBC2
purge-JDBC2: purge-Resources/JDBC2
purge-Resources/JDBC2:
	( cd Resources/JDBC2 && make purge )

jtsarjuna:: JTSArjuna
JTSArjuna: boot-JTSArjuna
	(cd JTSArjuna && \
	    echo Making all in JTSArjuna ...; \
	    ${MAKE} ${MAKEFLAGS})

install-jtsarjuna:: install-JTSArjuna
install-JTSArjuna: boot-JTSArjuna
	(cd JTSArjuna && \
	    echo Making install in JTSArjuna ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-JTSArjuna: JTSArjuna/config/JTSArjuna.cf
	( cd JTSArjuna && \
	    echo Making bootstrap in JTSArjuna ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

JTSArjuna/config/JTSArjuna.cf: \
	CosServices/CosRecovery/config/CosRecovery.cf \
	Resources/JDBC/config/JDBC.cf \
	Resources/JDBC2/config/JDBC2.cf
	@(echo "*** You must configure JTSArjuna first"; exit 1)

clean-jtsarjuna: clean-JTSArjuna
clean-JTSArjuna:
	( cd JTSArjuna && make clean )

tidy-jtsarjuna: tidy-JTSArjuna
tidy-JTSArjuna:
	( cd JTSArjuna && make tidy )

purge-jtsarjuna: purge-JTSArjuna
purge-JTSArjuna:
	( cd JTSArjuna && make purge )

tet:: TeT
TeT: boot-TeT
	(cd TeT && \
	    echo Making all in TeT ...; \
	    ${MAKE} ${MAKEFLAGS})

install-tet:: install-TeT
install-TeT: boot-TeT
	(cd TeT && \
	    echo Making install in TeT ...; \
	    ${MAKE} ${MAKEFLAGS} install)

boot-TeT: TeT/config/TeT.cf
	( cd TeT && \
	    echo Making bootstrap in TeT ...; \
	    ${MAKE} ${MAKEFLAGS} -f MakefileBoot && \
	    ${MAKE} ${MAKEFLAGS} Makefiles )

TeT/config/TeT.cf: JTSArjuna/config/JTSArjuna.cf
	@(echo "*** You must configure TeT first"; exit 1)

clean-tet: clean-TeT
clean-TeT:
	( cd TeT && make clean )

tidy-tet: tidy-TeT
tidy-TeT:
	( cd TeT && make tidy )

purge-tet: purge-TeT
purge-TeT:
	( cd TeT && make purge )

