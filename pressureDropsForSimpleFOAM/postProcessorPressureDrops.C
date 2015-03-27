using namespace std;

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "meshTools.H"
#include "Fluxes.H"
#include "PatchArea.H"
#include "MeanInternalField.H"
#include "MaxInternalField.H"
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

	//Evaluate Pressure drops
	double InletPressure;
	double OutletPressure;
	double PressureDrops;
	for (int j=0;j<listInletBoundaries.size();j++)
	{
		label patchID = mesh.boundaryMesh().findPatchID(listInletBoundaries[j]);
		forAll(p.boundaryField()[patchID] , facei)
		{
			double area = mesh.magSf().boundaryField()[patchID][facei];
			InletPressure += area*p.boundaryField()[patchID][facei];
		}
	}
	for (int j=0;j<listOutletBoundaries.size();j++)
	{
		label patchID = mesh.boundaryMesh().findPatchID(listOutletBoundaries[j]);
		forAll(p.boundaryField()[patchID] , facei)
		{
			double area = mesh.magSf().boundaryField()[patchID][facei];
			OutletPressure += area*p.boundaryField()[patchID][facei];
		}
	}
	InletPressure=InletPressure/PatchInletArea;
	OutletPressure=OutletPressure/PatchOutletArea;
	PressureDrops = InletPressure - OutletPressure;
	
	//- Calculate mean Pressure
	scalar pMean  = 0.;
	pMean = MeanInternalField(mesh, p);
	//- Print inlet velocity
	double Uinlet = 0.;
	for (int j=0;j<listInletBoundaries.size();j++)
	{
		label patchID = mesh.boundaryMesh().findPatchID(listInletBoundaries[j]);
		forAll(p.boundaryField()[patchID] , facei)
		{
			Uinlet = mag(U.boundaryField()[patchID][facei]);
		}
	}
	//- Calculate mean velocity
	scalar Umean  = 0.;
	Umean = MeanInternalField(mesh, mag(U));
	//- Calculate max velocity
	scalar Umax  = 0.;
	Umax = MaxInternalField(mesh, mag(U));

	//Write on terminal
	std::cout << "------------------------------------------------------" << std::endl;
	cout.setf(std::ios::scientific);
	std::cout << "Inlet velocity     = " << Uinlet << "	[m/s]" << std::endl;
	std::cout << "Mean velocity      = " << Umean << "	[m/s]" << std::endl;
	std::cout << "Max velocity       = " << Umax << "	[m/s]" << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout << "IN pressure        = " << InletPressure << "	[m2/s2]" << endl;
	std::cout << "OUT pressure       = " << OutletPressure << "	[m2/s2]" << endl;
	std::cout << "Pressure drops     = " << PressureDrops << "	[m2/s2]" << endl;
	std::cout << "Mean pressure      = " << pMean << "	[m2/s2]" << endl;
	std::cout << "------------------------------------------------------" << std::endl;
	
	// Write on file
	ofstream fmassFlux;
	fmassFlux.open("PressureDrops.txt", ios::out);
	fmassFlux.setf(std::ios::scientific);
	fmassFlux.precision(12);
	fmassFlux << "Inlet velocity    = " << Uinlet << "	[m/s]" << endl;
	fmassFlux << "Mean velocity     = " << Umean << "	[m/s]" << endl;
	fmassFlux << "Max velocity      = " << Umax << "	[m/s]\n" << endl;
	fmassFlux << "Pressure drops    = " <<  PressureDrops << "	[m2/s2]" << endl;
	fmassFlux << "IN pressure       = " << InletPressure << "	[m2/s2]" << endl;
	fmassFlux << "OUT pressure      = " << OutletPressure << "	[m2/s2]" << endl;
	fmassFlux << "Mean pressure      = " <<  pMean << "	[m2/s2]" << endl;
	fmassFlux.close();
	
	
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
