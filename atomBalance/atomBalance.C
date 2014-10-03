// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"
#include "fvIOoptionList.H"

int main(int argc, char **argv)
{
    timeSelector::addOptions();
    #include "setRootCase.H"
    #include "createTime.H"
	#include "createMesh.H"
	#include "readSolverOptions.H"

	instantList timeDirs = timeSelector::select0(runTime, args);

    forAll(timeDirs, timeI)
    {
		runTime.setTime(timeDirs[timeI], timeI);
		Info<< "Time = " << runTime.timeName() << endl;

		#include "readFields.H"

		//- Variable definitio
		Eigen::VectorXd Element;
		std::vector<std::string> ElementsNames;
		Eigen::MatrixXd SpecieAtomicComposition;
		std::vector<double> IN;
		std::vector<double> OUT;
		int k;
		int j;

		// Evaluate Inlet patch area
		ElementsNames = thermodynamicsMapXML->elements();
		SpecieAtomicComposition = thermodynamicsMapXML->atomic_composition();
		Element.resize(thermodynamicsMapXML->NumberOfSpecies());

		IN.resize(ElementsNames.size());
		OUT.resize(ElementsNames.size());
		for (k=0;k<ElementsNames.size();k++)
		{
			IN[k] = 0.;
			OUT[k] = 0.;
		}

		for (int j=0;j<listInletBoundaries.size();j++)
		{
			label patchID = mesh.boundaryMesh().findPatchID(listInletBoundaries[j]); 
			forAll (p.boundaryField()[patchID], facei)
			{
				thermodynamicsMapXML->SetPressure(p.boundaryField()[patchID][facei]);
				thermodynamicsMapXML->SetTemperature(T.boundaryField()[patchID][facei]);
				for (k=0;k<ElementsNames.size();k++)
				{
					Element = SpecieAtomicComposition.col(k);
					for (j=0;j<thermodynamicsMapXML->NumberOfSpecies();j++)
						IN[k] += Y[j].boundaryField()[patchID][facei]*phi.boundaryField()[patchID][facei]*Element[j]/thermodynamicsMapXML->MW()[j+1];
				}
			}
		}

		for (int j=0;j<listOutletBoundaries.size();j++)
		{
			label patchID = mesh.boundaryMesh().findPatchID(listOutletBoundaries[j]); 
			forAll (p.boundaryField()[patchID], facei)
			{
				thermodynamicsMapXML->SetPressure(p.boundaryField()[patchID][facei]);
				thermodynamicsMapXML->SetTemperature(T.boundaryField()[patchID][facei]);
				for (k=0;k<ElementsNames.size();k++)
				{
					Element = SpecieAtomicComposition.col(k);
					for (j=0;j<thermodynamicsMapXML->NumberOfSpecies();j++)
						OUT[k] += Y[j].boundaryField()[patchID][facei]*phi.boundaryField()[patchID][facei]*Element[j]/thermodynamicsMapXML->MW()[j+1];
				}
			}
		}

	std::vector<double> AtomicError;
	std::ofstream error;
	error.open("atomicBalance.txt",std::ios::out);
	Info << "\nATOMIC BALANCE               " << endl;
	error << "\nATOMIC BALANCE               " << endl;
	AtomicError.resize(IN.size());
	for (int k=0;k<IN.size();k++)
	{
		AtomicError[k] = (IN[k] + OUT[k])*100./IN[k];
		if ( IN[k] > 1.e-16 )
		{
			Info << ElementsNames[k] << " :\t" << fabs(AtomicError[k]) << "\t[%]" << endl;
			error << ElementsNames[k] << " :\t" << fabs(AtomicError[k]) << "\t[%]" << endl;
		}
	}
	Info << "\n" << endl;
	
	error.close();


	}
	Info<< "End\n" << endl;
	return 0; 
}
