#!/bin/bash
#Help
if [ $# != 6 ]
then
	echo "Input order:"
	echo "1:   Min reactor length [m]"
	echo "2:   Max reactor length [m]"
	echo "3:   Number of cells in axial direction"
	echo "4:   Axial direction x:1 y:2 z:3"
	echo "5:   Latest time"
	echo "6:   FLUID region name"
	exit 1
fi

#Create variables
Lmin=$1
Lmax=$2
N=$3
dir=$4
time=$5
region=$6
swakFolder="swakExpression_z"
L=$(echo "$Lmax - $Lmin" | bc -l)
dz=$(echo "$L/($N-1)" | bc -l)

#Modified for heterogenous or homogeneous
SpecieList=("C3H8" "CO" "H2")
NS=$(echo "${#SpecieList[@]}" | bc -l)

#Source OpenFOAM 2.3.1
source $HOME/OpenFOAM/OpenFOAM-2.3.1/etc/bashrc
foamCalc mag U

#Run
for k in $(seq 0 $NS)
do
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
	echo "application     catalyticConductiveFoam;" >> controlDict
	echo "startFrom       latestTime;" >> controlDict
	echo "startTime       0;" >> controlDict
	echo "stopAt	      writeNow;" >> controlDict
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
	echo "libs (\"libgroovyBC.so\" \"libsimpleSwakFunctionObjects.so\");" >> controlDict
	echo "functions" >> controlDict
	echo "{" >> controlDict

	rm -f ../grid.txt
	touch ../grid.txt
	for i in $(seq 1 $N)
	do
		#Create grid file
		newL=$(echo "$i*$dz-$dz+$Lmin" | bc -l)
		if [ "$i" -eq 1 ]
		then
			echo "#mm" >> ../grid.txt
			echo "$newL" >> ../grid.txt
		else
			echo "$newL" >> ../grid.txt
		fi

		echo "	z$i"  >> controlDict
		echo "    {" >> controlDict
		echo "        type                swakExpression;" >> controlDict
		echo "        outputControlMode   outputTime;" >> controlDict
		echo "        valueType           surface;" >> controlDict
		echo "        surfaceName         z$i;" >> controlDict
		echo "        region              $region;" >> controlDict
		echo "	      surface" >> controlDict
		echo "	      {" >> controlDict
		echo "	          type        plane;" >> controlDict
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
		echo "            source      cells;  // sample cells or boundaryFaces" >> controlDict
		echo "            interpolate false;	    " >> controlDict
		echo "	      }" >> controlDict
		if [ "$k" -eq "$NS" ]
		then
			echo "        expression \"rho*U & Sf()\";	" >> controlDict
		else
			echo "        expression \"rho*U*${SpecieList[k]} & Sf()\";	" >> controlDict
		fi
		echo "        accumulations (" >> controlDict
		echo "		sum" >> controlDict
		echo "        );" >> controlDict
		echo "        verbose true;" >> controlDict
		echo "        autoInterpolate true;" >> controlDict
		echo "        warnAutoInterpolate false;" >> controlDict
		echo "    }" >> controlDict

	done
	echo "}" >> controlDict
	echo "// ************************************************************************* //" >> controlDict
	cd ..
	rm -rf postProcessing
	postProcessorCupMixForCatalyticConductivePimpleFoam
	folderToRemove=$(ls -t | sed -n "1p")
	rm -rf $folderToRemove

 
	#Extract solution
	cd postProcessing
	if [ "$k" -eq "$NS" ]
	then
		rm -f ../rho.txt
		touch ../rho.txt
		for i in $(seq 1 $N)
		do
			folderName=$swakFolder$i
			if [ "$i" -eq 1 ]
			then
				echo "#cTot" >> ../rho.txt
				value=$(sed -n "2p" $swakFolder$i/$time/z$i | awk '{print $2}')
				echo "$value" >> ../rho.txt
			else
				value=$(sed -n "2p" $swakFolder$i/$time/z$i | awk '{print $2}')
				echo "$value" >> ../rho.txt
			fi
		done
	else
		rm -f ../${SpecieList[k]}.txt
		touch ../${SpecieList[k]}.txt
		for i in $(seq 1 $N)
		do
			folderName=$swakFolder$i
			if [ "$i" -eq 1 ]
			then
				echo "#${SpecieList[k]}" >> ../${SpecieList[k]}.txt
				value=$(sed -n "2p" $swakFolder$i/$time/z$i | awk '{print $2}')
				echo "$value" >> ../${SpecieList[k]}.txt
			else
				value=$(sed -n "2p" $swakFolder$i/$time/z$i | awk '{print $2}')
				echo "$value" >> ../${SpecieList[k]}.txt
			fi
		done
	fi
	cd ..
done

#Create final file
mv grid.txt temp.txt
for k in $(seq 0 $NS)
do
	if [ "$k" -eq "$NS" ]
	then
		paste temp.txt rho.txt >> temp1.txt
		rm -f temp.txt
		mv temp1.txt temp.txt
		rm -f rho.txt
	else	
		paste temp.txt ${SpecieList[k]}.txt >> temp1.txt
		rm -f temp.txt
		mv temp1.txt temp.txt
		rm -f ${SpecieList[k]}.txt
	fi
done
mv temp.txt cupMix.txt
