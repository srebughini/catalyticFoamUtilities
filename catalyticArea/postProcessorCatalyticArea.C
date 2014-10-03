using namespace std;

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "meshTools.H"
#include "PatchArea.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
	// Solver setup (folders, mesh, etc.)
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

	#include "readSolverOptions.H"

	// Evaluate Inlet patch area
	scalar PatchInletArea  = 0.;
	for (int j=0;j<listInletBoundaries.size();j++)
	{
		PatchInletArea +=  PatchArea(mesh, listInletBoundaries[j]);
	}
	// Evaluate Oulet patch area
	scalar PatchOutletArea  = 0.;
	for (int j=0;j<listOutletBoundaries.size();j++)
	{
		PatchOutletArea +=  PatchArea(mesh, listOutletBoundaries[j]);
	}
	// Evaluate catalyticArea patch area
	scalar PatchCatArea  = 0.;
	for (int j=0;j<listCatalyticBoundaries.size();j++)
	{
		PatchCatArea +=  PatchArea(mesh, listCatalyticBoundaries[j]);
	}
	
	//Write on terminal
	std::cout << "---------------------------------------------------" << std::endl;
	cout.setf(std::ios::scientific);
	std::cout << "Inlet area 	= " <<  PatchInletArea << "	[m2]" << std::endl;
	std::cout << "Oulet area 	= " <<  PatchOutletArea << "	[m2]" << std::endl;
	std::cout << "Catalytic area 	= " <<  PatchCatArea << "	[m2]" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

	//Write on file
	ofstream fmassFlux;
	fmassFlux.open("MeshDimensions.txt", ios::out);
	fmassFlux.setf(std::ios::scientific);
	fmassFlux.precision(12);
	fmassFlux << "Inlet area 	= " <<  PatchInletArea << "	[m2]" << endl;;
	fmassFlux << "Oulet area 	= " <<  PatchOutletArea << "	[m2]" << endl;
	fmassFlux << "Catalytic area 	= " <<  PatchCatArea << "	[m2]" << endl;
	fmassFlux << " " << endl;
	fmassFlux.close();
	return 0;
	
}

// ************************************************************************* //
