scalar PatchArea(const Foam::fvMesh &mesh, const std::string boundaryName)
{
	scalar patchArea = 0.;

	// find the identification number
	label patchID = mesh.boundaryMesh().findPatchID(boundaryName); 
	
	// if we don't have such a patch, warn the user
	if (patchID==-1) 
	{
		Info << "Failure to find patch named " << boundaryName << " for mass flow calculations" << endl;
		abort();
	}
	else   // calculate the result [kg/m2]
	{
		const scalarField& Ap = mesh.magSf().boundaryField()[patchID];
		patchArea = sum(Ap);
	}

	return patchArea;
}
