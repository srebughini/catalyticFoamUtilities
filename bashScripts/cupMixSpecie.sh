#!/bin/bash
#Help
if [ $# != 4 ]
then
	echo "Input order:"
	echo "1:   Min reactor length [m]"
	echo "2:   Max reactor length [m]"
	echo "3:   Number of sampling points"
	echo "4:   Normal direction x:1 y:2 z:3"
	exit 1
fi
#Reading parameters for LIGGGHTS simulation from shell
Lmin=$1
Lmax=$2
N=$3
dir=$4
L=$(echo "$Lmax - $Lmin" | bc -l)
dz=$(echo "$L/($N-1)" | bc -l)

#Creating controlDictFile
cd system
rm -f controlDict
touch controlDict
echo '/*--------------------------------*- C++ -*----------------------------------*/' >> controlDict
echo 'FoamFile' >> controlDict
echo '{' >> controlDict
echo '    version     2.0;' >> controlDict
echo '    format      ascii;' >> controlDict
echo '    class       dictionary;' >> controlDict
echo '    location    "system";' >> controlDict
echo '    object      controlDict;' >> controlDict
echo '}' >> controlDict
echo '// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //' >> controlDict
echo 'application     	postProcessorCupMix;' >> controlDict
echo 'startFrom       	latestTime;' >> controlDict
echo 'startTime       	0;' >> controlDict
echo 'stopAt		writeNow;' >> controlDict
echo 'endTime         	1000000;' >> controlDict
echo 'deltaT          	0.01;' >> controlDict
echo 'writeControl    	adjustableRunTime;' >> controlDict
echo 'writeInterval   	5e-07;' >> controlDict
echo 'purgeWrite      	0;' >> controlDict
echo 'writeFormat    	binary;' >> controlDict
echo 'writePrecision  	18;' >> controlDict
echo 'writeCompression 	compressed;' >> controlDict
echo 'timeFormat      	general;' >> controlDict
echo 'timePrecision   	6;' >> controlDict
echo 'runTimeModifiable yes;' >> controlDict
echo 'adjustTimeStep 	yes;' >> controlDict
echo 'maxCo           	0.05;' >> controlDict
echo 'libs 		("libCatalyticWall.so");' >> controlDict
echo 'functions' >> controlDict
echo '{' >> controlDict

for i in $(seq 1 $N)
do
	newL=$(echo "$i*$dz-$dz+$Lmin" | bc -l)
	echo "	z$i"  >> controlDict
	echo '	{' >> controlDict
	echo '		functionObjectLibs ("libfieldFunctionObjects.so" "libutilityFunctionObjects.so" "libIOFunctionObjects.so" "libFVFunctionObjects.so" "libcloudFunctionObjects.so");'>> controlDict
	echo '		type	faceSource'>> controlDict 
	echo '		enabled         false;'>> controlDict 
	echo '		outputControl   outputTime;'>> controlDict 
	echo '		log             false;'>> controlDict 
	echo '		valueOutput     false;'>> controlDict 
	echo '		surfaceFormat   vtk;'>> controlDict 
	echo '		source          sampledSurface;'>> controlDict
	echo '		interpolationScheme ?; //cellPoint;'>> controlDict
	echo '		sampledSurfaceDict'>> controlDict 
	echo '		{'>> controlDict 
	echo '			type            plane;'>> controlDict 
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
	echo '			source cells;'>> controlDict 
	echo '			interpolate false;'>> controlDict 
	echo '		}'>> controlDict 
	echo '		operation       areaIntegrate;'>> controlDict 
	echo '		fields'>> controlDict 
	echo '		('>> controlDict
	echo '			gas::phiz'>> controlDict
	echo '			gas::CO'>> controlDict
	echo '			mole_CO'>> controlDict
	echo '		);'>> controlDict
	echo '	}' >> controlDict
done
echo '}' >> controlDict
echo '// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //' >> controlDict
cd ..

#Run post processor cup mix
source $HOME/OpenFOAM/OpenFOAM-2.3.x/etc/bashrc
rm -rf postProcessing
foamCalc mag U > log
postProcessorCupMix > log
rm -r log CatalyticReactors

time=$(ls -t postProcessing/z1/ | sed -n "1p")
rm -f specie.txt grid.txt cupMix.txt
touch specie.txt
touch grid.txt
for i in $(seq 1 $N)
do
	newL=$(echo "$i*$dz+$Lmin" | bc -l)
	z="z"
	folderName=$z$i
	if [ $i -eq 1 ]
	then
		sed '1,2d' postProcessing/$folderName/$time/faceSource.dat >> specie.txt
		echo "#mm" >> grid.txt
		echo "$newL" >> grid.txt
	else
		sed '1,3d' postProcessing/$folderName/$time/faceSource.dat >> specie.txt
		echo "$newL" >> grid.txt
	fi
done
paste grid.txt specie.txt >> cupMix.txt
rm -f specie.txt grid.txt

rm -rf plots
mkdir plots
cd plots
touch plotResults
echo "set terminal pdf enhanced font 'Helvetica,9'" >> plotResults
echo "set termoption dash" >> plotResults
echo "set output 'MassFlux.pdf'" >> plotResults
echo "unset label" >> plotResults
echo "unset key" >> plotResults
echo "set ylabel 'Mass flux [Kg/s]'" >> plotResults
echo "set xlabel 'Axial length [mm]'" >> plotResults
echo "plot \\" >> plotResults
echo '"../cupMix.txt" u ($1*1000):4 w p linecolor rgb "#000000" pt 7 ps 0.65 title " ", \' >> plotResults
echo " " >> plotResults
echo "set terminal pdf enhanced font 'Helvetica,9'" >> plotResults
echo "set termoption dash" >> plotResults
echo "set output 'cupMixCO.pdf'" >> plotResults
echo "unset label" >> plotResults
echo "unset key" >> plotResults
echo "set ylabel 'Mass fraction [-]'" >> plotResults
echo "set xlabel 'Axial length [mm]'" >> plotResults
echo "plot \\" >> plotResults
echo '"../cupMix.txt" u ($1*1000):($5/$4) w p linecolor rgb "#000000" pt 7 ps 0.5 title " ", \' >> plotResults
echo " " >> plotResults
echo "set terminal pdf enhanced font 'Helvetica,9'" >> plotResults
echo "set termoption dash" >> plotResults
echo "set output 'CO.pdf'" >> plotResults
echo "unset label" >> plotResults
echo "unset key" >> plotResults
echo "set ylabel 'Mole fraction [-]'" >> plotResults
echo "set xlabel 'Axial length [mm]'" >> plotResults
echo "plot \\" >> plotResults
echo '"../cupMix.txt" u ($1*1000):6 w p linecolor rgb "#000000" pt 7 ps 0.5 title " ", \' >> plotResults
echo " " >> plotResults
gnuplot plotResults
cd ..



