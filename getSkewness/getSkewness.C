// OpenFOAM
#include "fvCFD.H"
#include "cellQuality.H"
#include "fvIOoptionList.H"

int main(int argc, char *argv[])
{
	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"

	volScalarField cellSkewness
	(
		IOobject
		(
			"cellSkewness",
			runTime.timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::AUTO_WRITE
		),
		mesh,
		dimensionedScalar("zero",dimVolume,0.0)
	);

	cellQuality qualityCheck(mesh);
	cellSkewness.internalField() = qualityCheck.skewness();
	cellSkewness.write();
	
	Info<< "end" << endl;

	return 0;
}
