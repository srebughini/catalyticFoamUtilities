forAll(fluidRegions, j)
{
	Info<< "Properties evaluation..." << endl;

	OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
	OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

	// Internal fields
	{
		const scalarField& TCells = TFluid[j].internalField();
		const scalarField& pCells = pFluid[j].internalField(); 

		scalarField& cTotCells = cTotFluid[j].internalField();
		scalarField& MWmixCells = MWmixFluid[j].internalField();

		forAll(TCells, celli)
		{
			thermodynamicsMapXML->SetPressure(pCells[celli]);
			thermodynamicsMapXML->SetTemperature(TCells[celli]);
			
			transportMapXML->SetPressure(pCells[celli]);
			transportMapXML->SetTemperature(TCells[celli]);

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
				
			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

			cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
		}
	}


	// Boundaries
	forAll(TFluid[j].boundaryField(), patchi)
	{
		fvPatchScalarField& pp = pFluid[j].boundaryField()[patchi];
		fvPatchScalarField& pT = TFluid[j].boundaryField()[patchi];

		fvPatchScalarField& pcTot = cTotFluid[j].boundaryField()[patchi];
		fvPatchScalarField& pMWmix = MWmixFluid[j].boundaryField()[patchi];

		forAll(pT, facei)
		{
			thermodynamicsMapXML->SetPressure(pp[facei]);
			thermodynamicsMapXML->SetTemperature(pT[facei]);

			transportMapXML->SetPressure(pp[facei]);
			transportMapXML->SetTemperature(pT[facei]);
			
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei];

			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

			pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
		}
	}
}
