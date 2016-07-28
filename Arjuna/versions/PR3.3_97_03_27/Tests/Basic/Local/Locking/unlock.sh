#!/bin/sh

#
# Copyright (C) 1993
#
# Department of Computing Science,
# The University,
# Newcastle upon Tyne,
# UK.
#
# $Id: unlock.sh,v 1.1 1994/10/14 11:09:11 nmcl Exp $
#

cat output | while read type id
do
	../../../../Utilities/Tools/bin/unlock_shmem -Type $type -Uid $id
done
