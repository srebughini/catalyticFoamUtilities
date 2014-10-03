scalar calculateMassFlowRate(const Foam::fvMesh &mesh, surfaceScalarField &phi, const std::string boundaryName)
{
	scalar massFlowRate = 0.;

	// find the identification number
	label patchID = mesh.boundaryMesh().findPatchID(boundaryName); 

	// if we don't have such a patch, warn the user
	if (patchID==-1) 
	{
		Info << "Failure to find patch named " << boundaryName << " for mass flow calculations" << endl;
		abort();
	}
	else   // calculate the result [kg/s]
	{
		massFlowRate = -1.0*sum(phi.boundaryField()[patchID]); 
	}

	return massFlowRate;
}

scalar calculateFlowRate(const Foam::fvMesh &mesh, surfaceScalarField &phi, const volScalarField &omega, const std::string boundaryName)
{
	scalar massFlowRate = 0.;

	// find the identification number
	label patchID = mesh.boundaryMesh().findPatchID(boundaryName); 

	// if we don't have such a patch, warn the user
	if (patchID==-1) 
	{
		Info << "Failure to find patch named " << boundaryName << " for mass flow calculations" << endl;
		abort();
	}
	else   // calculate the result [kg/s]
	{
		massFlowRate = -1.0*sum(phi.boundaryField()[patchID]*omega.boundaryField()[patchID]);
	}

	return massFlowRate;
}


scalar calculateMassFraction(const Foam::fvMesh &mesh, const volScalarField &omega, const std::string boundaryName)
{
	scalar massFlowRate = 0.;

	// find the identification number
	label patchID = mesh.boundaryMesh().findPatchID(boundaryName); 

	// if we don't have such a patch, warn the user
	if (patchID==-1) 
	{
		Info << "Failure to find patch named " << boundaryName << " for mass flow calculations" << endl;
		abort();
	}
	else   // calculate the result [kg/s]
	{
		massFlowRate = sum(omega.boundaryField()[patchID]);
	}

	return massFlowRate;
}

