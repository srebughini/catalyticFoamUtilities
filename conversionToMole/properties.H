double dummy;
OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

// Internal fields
{
	const scalarField& TCells = T.internalField();
	const scalarField& pCells = p.internalField(); 

	forAll(TCells, celli)
	{
		thermodynamicsMapXML->SetPressure(pCells[celli]);
		thermodynamicsMapXML->SetTemperature(TCells[celli]);
		
		transportMapXML->SetPressure(pCells[celli]);
		transportMapXML->SetTemperature(TCells[celli]);

		for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			massFractions[i+1] = Y[i].internalField()[celli];

		thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,dummy,massFractions);

		for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			x[i].internalField()[celli] = moleFractions[i+1];
	}
}

// Boundaries
forAll(T.boundaryField(), patchi)
{
	fvPatchScalarField& pp = p.boundaryField()[patchi];
	fvPatchScalarField& pT = T.boundaryField()[patchi];

	forAll(pT, facei)
	{
		thermodynamicsMapXML->SetPressure(pp[facei]);
		thermodynamicsMapXML->SetTemperature(pT[facei]);

		transportMapXML->SetPressure(pp[facei]);
		transportMapXML->SetTemperature(pT[facei]);
		
		for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			massFractions[i+1] = Y[i].boundaryField()[patchi][facei];

		thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,dummy,massFractions);

		for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			 x[i].boundaryField()[patchi][facei] = moleFractions[i+1];
	}
}
