#!/bin/bash
#Help
if [ $# != 2 ]
then
	echo "Input order:"
	echo "1:   In  patch name"
	echo "2:   Out patch name"
	exit 1
fi

#Save patch
inlet=$1
outlet=$2

#Load OpenFOAM
source $HOME/OpenFOAM/OpenFOAM-2.3.1/etc/bashrc


rm -f p.in p.out

patchAverage p $inlet -latestTime > p.in
patchAverage p $outlet -latestTime > p.out

