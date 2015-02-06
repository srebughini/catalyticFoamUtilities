
{
	Info<< "Properties evaluation..." << endl;

	OpenSMOKE::OpenSMOKEVectorDouble HVector(thermodynamicsMapXML->NumberOfSpecies());
	OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
	OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(thermodynamicsMapXML->NumberOfSpecies());

	// Internal fields
	{
		const scalarField& TCells = T.internalField();
		const scalarField& pCells = p.internalField();
		const vectorField& UCells = U.internalField();

		scalarField& cTotCells = cTot.internalField();
		scalarField& MWmixCells = MWmix.internalField();
		scalarField& phixCells = phix.internalField();
		scalarField& phiyCells = phiy.internalField();
		scalarField& phizCells = phiz.internalField();
		scalarField& magUcells = magU.internalField();

		forAll(TCells, celli)
		{
			thermodynamicsMapXML->SetPressure(pCells[celli]);
			thermodynamicsMapXML->SetTemperature(TCells[celli]);
			
			transportMapXML->SetPressure(pCells[celli]);
			transportMapXML->SetTemperature(TCells[celli]);
				
	
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[i].internalField()[celli];
				
			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				x[i].internalField()[celli] = moleFractions[i+1];
			
			cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];

			phixCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(0);
			phiyCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(1);
			phizCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(2);

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massPerPhi[i].internalField()[celli]= cTotCells[celli]*MWmixCells[celli]*magUcells[celli]*Y[i].internalField()[celli];
		}
    }

	// Boundaries
	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pp = p.boundaryField()[patchi];
		fvPatchScalarField& pT = T.boundaryField()[patchi];
        fvPatchVectorField& pU = U.boundaryField()[patchi];
        
		fvPatchScalarField& pcTot = cTot.boundaryField()[patchi];
		fvPatchScalarField& pMWmix = MWmix.boundaryField()[patchi];
        fvPatchScalarField& pphix = phix.boundaryField()[patchi];
        fvPatchScalarField& pphiy = phiy.boundaryField()[patchi];
        fvPatchScalarField& pphiz = phiz.boundaryField()[patchi];
		fvPatchScalarField& pmagU = magU.boundaryField()[patchi];

		forAll(pT, facei)
		{
			thermodynamicsMapXML->SetPressure(pp[facei]);
			thermodynamicsMapXML->SetTemperature(pT[facei]);

			transportMapXML->SetPressure(pp[facei]);
			transportMapXML->SetTemperature(pT[facei]);
			
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[i].boundaryField()[patchi][facei];

			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				 x[i].boundaryField()[patchi][facei] = moleFractions[i+1];

			pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);

			pphix[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(0);
			pphiy[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(1);
			pphiz[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(2);

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massPerPhi[i].boundaryField()[patchi][facei] = pcTot[facei]*pMWmix[facei]*pmagU[facei]*Y[i].boundaryField()[patchi][facei];

		}
	}
}
