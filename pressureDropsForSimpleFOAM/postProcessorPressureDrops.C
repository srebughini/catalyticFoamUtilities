#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "meshTools.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
int main(int argc, char *argv[])
{

	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"

	#include "readTimeControls.H"

	Info<< "Time = " << runTime.timeName() << nl << endl;

	Info<< "Reading field U" << endl;
	volVectorField U
	(
		IOobject
		(
			"U",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	Info<< "Reading field p" << endl;
	volScalarField p
	(
		IOobject
		(
			"p",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::NO_WRITE
		),
		mesh
	);
		

	#include "createPhi.H"

	Info<< "\nStarting FAKE time loop\n" << endl;

	bool cupMixExit = true;
	while (runTime.run())
	{
		if (cupMixExit != true)
		{
			Info << "ERROR: please set writeNow in controlDict options" << endl;
			exit(-1);
		}
		runTime++;
		runTime.write();
		cupMixExit = false;
	}

	return 0;
	
}

// ************************************************************************* //
