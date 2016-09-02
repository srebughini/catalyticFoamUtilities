#!/bin/bash
#Help
if [ $# != 6 ]
then
	echo "Input order:"
	echo "1:   Min reactor length [m]"
	echo "2:   Max reactor length [m]"
	echo "3:   Number of cells in axial direction"
	echo "4:   Axial direction x:1 y:2 z:3"
	echo "5:   SOLID region name"
	echo "6:   FIELD name"
	exit 1
fi

#Create variables
Lmin=$1
Lmax=$2
N=$3
dir=$4
region=$5
name=$6
L=$(echo "$Lmax - $Lmin" | bc -l)
dz=$(echo "$L/($N-1)" | bc -l)

#Source OpenFOAM 2.3.1
source $HOME/OpenFOAM/OpenFOAM-2.3.1/etc/bashrc

#Make controlDict

cd system
mv controlDict controlDict.old
rm -f controlDict
touch controlDict
echo "/*--------------------------------*- C++ -*----------------------------------*/" >> controlDict
echo "FoamFile" >> controlDict
echo "{" >> controlDict
echo "    version     2.0;" >> controlDict
echo "    format      ascii;" >> controlDict
echo "    class       dictionary;" >> controlDict
echo "    location    "system";" >> controlDict
echo "    object      ControlDict;" >> controlDict
echo "}" >> controlDict
echo "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //" >> controlDict
echo "application     catalyticFoam;" >> controlDict
echo " " >> controlDict
echo "startFrom       latestTime;" >> controlDict
echo " " >> controlDict
echo "startTime       0;" >> controlDict
echo " " >> controlDict
echo "stopAt	writeNow;" >> controlDict
echo " " >> controlDict
echo "endTime         10.;" >> controlDict
echo " " >> controlDict
echo "deltaT          0.1;" >> controlDict
echo " " >> controlDict
echo "writeControl    adjustableRunTime;" >> controlDict
echo " " >> controlDict
echo "writeInterval   1;" >> controlDict
echo " " >> controlDict
echo "purgeWrite      0;" >> controlDict
echo " " >> controlDict
echo "writeFormat     binary;" >> controlDict
echo " " >> controlDict
echo "writePrecision  6;" >> controlDict
echo " " >> controlDict
echo "writeCompression compressed;" >> controlDict
echo " " >> controlDict
echo "timeFormat      general;" >> controlDict
echo " " >> controlDict
echo "timePrecision   6;" >> controlDict
echo " " >> controlDict
echo "runTimeModifiable yes;" >> controlDict
echo " " >> controlDict
echo "adjustTimeStep  yes;" >> controlDict
echo " " >> controlDict
echo "maxCo           0.1;" >> controlDict
echo " " >> controlDict
echo "functions" >> controlDict
echo "{" >> controlDict

rm -f ../grid-$region.txt
touch ../grid-$region.txt
for i in $(seq 1 $N)
do
	#Create grid file
	newL=$(echo "$i*$dz-$dz+$Lmin" | bc -l)
	echo "$newL" >> ../grid-$region.txt

	echo "z$i"  >> controlDict
	echo "{" >> controlDict
	echo "	type            faceSource;" >> controlDict
	echo '	functionObjectLibs ("libfieldFunctionObjects.so");' >> controlDict
	echo " " >> controlDict
	echo "	enabled         true;" >> controlDict
	echo "	outputControl   outputTime; //outputTime timeStep" >> controlDict
	echo " " >> controlDict
	echo "	// Output to log&file (true) or to file only" >> controlDict
	echo "	log             true;" >> controlDict
	echo " " >> controlDict
	echo "	// Output field values as well" >> controlDict
	echo "	valueOutput     false;  //true;" >> controlDict
	echo " " >> controlDict
	echo "	//Which region	" >> controlDict
	echo "	region              $region;" >> controlDict
	echo " " >> controlDict
	echo "	// Type of source: patch/faceZone/sampledSurface" >> controlDict
	echo "	source          sampledSurface;" >> controlDict

	echo "	sampledSurfaceDict" >> controlDict
	echo "	{" >> controlDict
	echo "		type 			cuttingPlane;" >> controlDict
	echo "		planeType		pointAndNormal;" >> controlDict
	echo "		pointAndNormalDict" >> controlDict
	echo "		{" >> controlDict
	if [ "$dir" -eq 1 ]
	then
		echo "			basePoint       ($newL 0. 0.);">> controlDict
		echo '			normalVector    (1 0 0);'>> controlDict
	fi
	if [ "$dir" -eq 2 ]
	then
		echo "			basePoint       (0. $newL 0.);">> controlDict
		echo '			normalVector    (0 1 0);'>> controlDict
	fi
	if [ "$dir" -eq 3 ]
	then
		echo "			basePoint       (0. 0. $newL);">> controlDict
		echo '			normalVector    (0 0 1);'>> controlDict
	fi
	echo "		}" >> controlDict
	echo "		source cells; // sample cells or boundaryFaces" >> controlDict
	echo "		interpolate true;" >> controlDict
	echo "	}" >> controlDict
	echo " " >> controlDict
	echo "	// Operation: areaAverage/sum/weightedAverage ..." >> controlDict
	echo "	operation      areaAverage; //areaAverage; //areaIntegrate;" >> controlDict

	echo "	fields          ($name);" >> controlDict
	echo "}" >> controlDict
	echo " " >> controlDict
done
echo "}" >> controlDict
echo "// ************************************************************************* //" >> controlDict
cd ..

rm -f log.area
catalyticMultiRegionSimpleFoamCupMix > log.area

rm -f $name-$region.txt
cat log.area | grep "areaAverage(sampledSurface) for ${name}" | awk '{print$5}' > $name-$region.txt

rm -rf postProcessing

