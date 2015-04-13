{
	Info<< "Properties evaluation..." << endl;

	OpenSMOKE::OpenSMOKEVectorDouble HVector(thermodynamicsMapXML->NumberOfSpecies());
	OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
	OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(thermodynamicsMapXML->NumberOfSpecies());
	Eigen::MatrixXd SpecieAtomicComposition;
	Eigen::VectorXd Element;
	std::vector<std::string> ElementsNames;

	// Internal fields
	{
		const scalarField& TCells = T.internalField();
		const scalarField& pCells = p.internalField(); 

		scalarField& cTotCells = cTot.internalField();

		forAll(TCells, celli)
		{
			cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
		}
	}

	// Boundaries
	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pp = p.boundaryField()[patchi];
		fvPatchScalarField& pT = T.boundaryField()[patchi];

		fvPatchScalarField& pcTot = cTot.boundaryField()[patchi];

		forAll(pT, facei)
		{
			pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
		}
	}
}
