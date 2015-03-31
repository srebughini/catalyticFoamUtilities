#!/bin/bash
#Help
if [ $# != 4 ]
then
	echo "Input order:"
	echo "1:   First plane position [m]"
	echo "2:   Second plane position [m]"
	echo "3:   Axial direction x:1 y:2 z:3"
	echo "4:   Latest time"
	exit 1
fi

#Create variables
inlet=$1
outlet=$2
dir=$3
time=$4

#Source OpenFOAM 2.3.1
source $HOME/OpenFOAM/OpenFOAM-2.3.1/etc/bashrc
foamCalc mag U

#Run
cd system
rm controlDict
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
echo "startFrom       latestTime;" >> controlDict
echo "startTime       0;" >> controlDict
echo "stopAt	writeNow;" >> controlDict
echo "endTime         10.;" >> controlDict
echo "deltaT          0.1;" >> controlDict
echo "writeControl    adjustableRunTime;" >> controlDict
echo "writeInterval   1;" >> controlDict
echo "purgeWrite      0;" >> controlDict
echo "writeFormat     ascii;" >> controlDict
echo "writePrecision  18;" >> controlDict
echo "writeCompression uncompressed;" >> controlDict
echo "timeFormat      general;" >> controlDict
echo "timePrecision   6;" >> controlDict
echo "runTimeModifiable yes;" >> controlDict
echo "adjustTimeStep  yes;" >> controlDict
echo "maxCo           0.1;" >> controlDict
echo "libs (\"libCatalyticWall.so\" \"libgroovyBC.so\" \"libsimpleSwakFunctionObjects.so\");" >> controlDict
echo "functions" >> controlDict
echo "{" >> controlDict
echo "	outlet" >> controlDict
echo "	{" >> controlDict
echo "		type            faceSource;" >> controlDict
echo "		functionObjectLibs (\"libfieldFunctionObjects.so\");" >> controlDict
echo "		enabled         true;" >> controlDict
echo "		outputControl   outputTime;" >> controlDict
echo "		log             true;" >> controlDict
echo "    	valueOutput     true;" >> controlDict
echo "		surfaceFormat   vtk;" >> controlDict
echo "       	source         	sampledSurface; //patch;" >> controlDict
echo "        	sampledSurfaceDict" >> controlDict
echo " 	        {" >> controlDict
echo "	         	type        cuttingPlane;" >> controlDict
echo "	         	planeType           pointAndNormal;" >> controlDict
echo "	         	pointAndNormalDict" >> controlDict
echo " 	        	{" >> controlDict
if [ "$dir" -eq 1 ]
then
	echo "			basePoint       ($outlet 0. 0.);">> controlDict
	echo '			normalVector    (1 0 0);'>> controlDict
fi
if [ "$dir" -eq 2 ]
then
	echo "			basePoint       (0. $outlet 0.);">> controlDict
	echo '			normalVector    (0 1 0);'>> controlDict
fi
if [ "$dir" -eq 3 ]
then
	echo "			basePoint       (0. 0. $outlet);">> controlDict
	echo '			normalVector    (0 0 1);'>> controlDict
fi
echo "	      		}" >> controlDict
echo "            source      cells;  // sample cells or boundaryFaces" >> controlDict
echo "            interpolate false;	    " >> controlDict
echo "		}" >> controlDict
echo "		operation       areaAverage;" >> controlDict
echo "        	fields" >> controlDict
echo "		(" >> controlDict
echo "		p" >> controlDict
echo "		);" >> controlDict
echo "	}" >> controlDict
echo "  inlet" >> controlDict
echo "	{" >> controlDict
echo "		type            faceSource;" >> controlDict
echo "		functionObjectLibs (\"libfieldFunctionObjects.so\");" >> controlDict
echo "		enabled         true;" >> controlDict
echo "		outputControl   outputTime;" >> controlDict
echo "		log             true;" >> controlDict
echo "    	valueOutput     true;" >> controlDict
echo "		surfaceFormat   vtk;" >> controlDict
echo "       	source         	sampledSurface; //patch;" >> controlDict
echo "        	sampledSurfaceDict" >> controlDict
echo " 	        {" >> controlDict
echo "	         	type        cuttingPlane;" >> controlDict
echo "	         	planeType           pointAndNormal;" >> controlDict
echo "	         	pointAndNormalDict" >> controlDict
echo " 	        	{" >> controlDict
if [ "$dir" -eq 1 ]
then
	echo "			basePoint       ($inlet 0. 0.);">> controlDict
	echo '			normalVector    (1 0 0);'>> controlDict
fi
if [ "$dir" -eq 2 ]
then
	echo "			basePoint       (0. $inlet 0.);">> controlDict
	echo '			normalVector    (0 1 0);'>> controlDict
fi
if [ "$dir" -eq 3 ]
then
	echo "			basePoint       (0. 0. $inlet);">> controlDict
	echo '			normalVector    (0 0 1);'>> controlDict
fi
echo "	      		}" >> controlDict
echo "            source      cells;  // sample cells or boundaryFaces" >> controlDict
echo "            interpolate false;	    " >> controlDict
echo "		}" >> controlDict
echo "		operation       areaAverage;" >> controlDict
echo "        	fields" >> controlDict
echo "		(" >> controlDict
echo "		p" >> controlDict
echo "		);" >> controlDict
echo "	}" >> controlDict
echo "// ************************************************************************* //" >> controlDict
cd ..
rm -rf postProcessing

#Post processor, depens on type of simulation
postProcessorCupMixForRhoSimpleFoam

folderToRemove=$(echo "$time + 1" | bc -l)
rm -rf $folderToRemove

rm -f pressureDrops.txt
rm -f PressureDrops.txt
touch pressureDrops.txt
valueInlet=$(sed -n "4p" postProcessing/inlet/$time/faceSource.dat | awk '{print $3}')
valueOutlet=$(sed -n "4p" postProcessing/outlet/$time/faceSource.dat | awk '{print $3}')

echo "Pressure drops estimated somewhere in the reactors" >> pressureDrops.txt
echo " " >> pressureDrops.txt
echo "Inlet          [Pa]:  $valueInlet       at     $inlet [m]"  >> pressureDrops.txt
echo "Outlet         [Pa]:  $valueOutlet      at     $outlet [m]" >> pressureDrops.txt

rm -rf postProcessing
