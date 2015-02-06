{
	Info << "Properties..." << endl;
	const scalarField& pCells = p.internalField();
	const scalarField& TCells = T.internalField();
	const vectorField& UCells = U.internalField();

	scalarField& mwCells = MWmix.internalField();
	scalarField& phixCells = phix.internalField();
	scalarField& phiyCells = phiy.internalField();
	scalarField& phizCells = phiz.internalField();

	OpenSMOKE::OpenSMOKEVectorDouble y(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble x(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble c(Y.size());

	forAll(TCells, celli)
	{
		// Sets the maps
		thermodynamicsMapXML->SetTemperature(TCells[celli]);
		thermodynamicsMapXML->SetPressure(pCells[celli]);

		transportMapXML->SetPressure(pCells[celli]);
		transportMapXML->SetTemperature(TCells[celli]);

		// Extract the mass fractions
		for(unsigned int i=1;i<=Y.size();i++)
			y[i] = Y[i-1].internalField()[celli];

		// Calculate the molecular weight and the mole fractions
		thermodynamicsMapXML->MoleFractions_From_MassFractions(x, mwCells[celli], y);

		// Extract the mass fractions
		for(unsigned int i=1;i<=Y.size();i++)
			x_[i-1].internalField()[celli] = x[i];

		// Calculate the density
		double const cTot  = pCells[celli] / (PhysicalConstants::R_J_kmol * TCells[celli]);
		double const rho  = cTot * mwCells[celli];
		double const ux   = UCells[celli].component(0);
		double const uy   = UCells[celli].component(1);
		double const uz   = UCells[celli].component(2);
		double const magU = std::sqrt(ux*ux + uy*uy + uz*uz);

		phixCells[celli] = rho*ux;
		phiyCells[celli] = rho*uy;
		phizCells[celli] = rho*uz;

		for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			massPerPhi_[i].internalField()[celli]= rho*magU*Y[i].internalField()[celli];
	}

	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pp = p.boundaryField()[patchi];
		fvPatchScalarField& pT = T.boundaryField()[patchi];
		fvPatchVectorField& pU = U.boundaryField()[patchi];

		fvPatchScalarField& pmw = MWmix.boundaryField()[patchi];
		fvPatchScalarField& pphix = phix.boundaryField()[patchi];
		fvPatchScalarField& pphiy = phiy.boundaryField()[patchi];
		fvPatchScalarField& pphiz = phiz.boundaryField()[patchi];

		forAll(pT, facei)
		{
			// Sets the maps
			thermodynamicsMapXML->SetTemperature(pT[facei]);
			thermodynamicsMapXML->SetPressure(pp[facei]);

			transportMapXML->SetPressure(pp[facei]);
			transportMapXML->SetTemperature(pT[facei]);

			// Extract the mass fractions
			for(unsigned int i=1;i<=Y.size();i++)
				y[i] = Y[i-1].boundaryField()[patchi][facei];

			// Calculate the molecular weight and the mole fractions
			thermodynamicsMapXML->MoleFractions_From_MassFractions(x, pmw[facei], y);

			// Extract the mass fractions
			for(unsigned int i=1;i<=Y.size();i++)
				x_[i-1].boundaryField()[patchi][facei] = x[i];

			// Calculate the density
			double const cTot = pp[facei]  / (PhysicalConstants::R_J_kmol * pT[facei]);
			double const rho  = cTot * pmw[facei];
			double const ux   = pU[facei].component(0);
			double const uy   = pU[facei].component(1);
			double const uz   = pU[facei].component(2);
			double const magU = std::sqrt(ux*ux + uy*uy + uz*uz);
			pphix[facei] = rho*ux;
			pphiy[facei] = rho*uy;
			pphiz[facei] = rho*uz;

			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massPerPhi_[i].boundaryField()[patchi][facei] = rho*magU*Y[i].boundaryField()[patchi][facei];
		}
	}
}
