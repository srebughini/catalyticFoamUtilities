
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
		scalarField&  psiCells =  psi.internalField();
		scalarField&  muCells  =  mu.internalField();
		scalarField& lambdaCells = lambda.internalField();
		scalarField& cpCells = cp.internalField();
		scalarField& hCells = h.internalField();
		scalarField& MWmixCells = MWmix.internalField();
		scalarField& phixCells = phix.internalField();
		scalarField& phiyCells = phiy.internalField();
		scalarField& phizCells = phiz.internalField();

		forAll(TCells, celli)
		{
			thermodynamicsMapXML->SetPressure(pCells[celli]);
			thermodynamicsMapXML->SetTemperature(TCells[celli]);
			
			transportMapXML->SetPressure(pCells[celli]);
			transportMapXML->SetTemperature(TCells[celli]);
				
	
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[i].internalField()[celli];
				
			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

			cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
			psiCells[celli]  = cTotCells[celli]*MWmixCells[celli]/pCells[celli];
			thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(hCells[celli], moleFractions);		// [J/kmol]
			hCells[celli] /= MWmixCells[celli];															// [J/kg]
			
			phixCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(0);
			phiyCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(1);
			phizCells[celli] = cTotCells[celli]*MWmixCells[celli]*UCells[celli].component(2);

			//if (viscositySpecies == true)
			{
				transportMapXML->DynamicViscosity(muCells[celli],moleFractions);
			}

            if (energyEquation == true || diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
				transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
                
                		thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);			//[J/Kmol/K]
				cpCells[celli] = cpCells[celli]/MWmixCells[celli];
			
				if (iMassDiffusionInEnergyEquation == true)
				{
					thermodynamicsMapXML->hMolar_Species(HVector);
					for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
						HSpecies[i].internalField()[celli] = HVector[i+1] / thermodynamicsMapXML->MW()[i+1];
				}
			}
			
			if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
			{
				transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
					Dmix[i].internalField()[celli] = Dmixvector[i+1];
			}
			else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
			{
				const double coefficient = lambdaCells[celli]/(pCells[celli]*psiCells[celli])/cpCells[celli];
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
					Dmix[i].internalField()[celli] = coefficient/LewisNumbers(i);
			}	

			// Thermal diffusion coefficients [-]
			if (soretEffect == true)
			{
				transportMapXML->ThermalDiffusionRatios(tetamixvector, moleFractions);		
				for(int i=0;i<transportMapXML->iThermalDiffusionRatios().size();i++)
				{
					unsigned int index = transportMapXML->iThermalDiffusionRatios()[i];
					Dsoret[i].internalField()[celli] =Dmix[index-1].internalField()[celli]*tetamixvector[index]*thermodynamicsMapXML->MW()[index]/MWmixCells[celli];
				} 
			}
		}
    }

	// Boundaries
	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pp = p.boundaryField()[patchi];
		fvPatchScalarField& pT = T.boundaryField()[patchi];
        fvPatchVectorField& pU = U.boundaryField()[patchi];
        
		fvPatchScalarField& pcTot = cTot.boundaryField()[patchi];
		fvPatchScalarField& ppsi  = psi.boundaryField()[patchi];
		fvPatchScalarField& pmu   = mu.boundaryField()[patchi];
		fvPatchScalarField& plambda = lambda.boundaryField()[patchi];
		fvPatchScalarField& pcp = cp.boundaryField()[patchi];
		fvPatchScalarField& ph = h.boundaryField()[patchi];
		fvPatchScalarField& pMWmix = MWmix.boundaryField()[patchi];
        fvPatchScalarField& pphix = phix.boundaryField()[patchi];
        fvPatchScalarField& pphiy = phiy.boundaryField()[patchi];
        fvPatchScalarField& pphiz = phiz.boundaryField()[patchi];

		forAll(pT, facei)
		{
			thermodynamicsMapXML->SetPressure(pp[facei]);
			thermodynamicsMapXML->SetTemperature(pT[facei]);

			transportMapXML->SetPressure(pp[facei]);
			transportMapXML->SetTemperature(pT[facei]);
			
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				massFractions[i+1] = Y[i].boundaryField()[patchi][facei];

			thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);


			pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
			ppsi[facei]  = pcTot[facei]*pMWmix[facei]/pp[facei];
			thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(ph[facei], moleFractions);		// [J/kmol]
			ph[facei] /= pMWmix[facei];																// [J/kg]
			
			pphix[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(0);
			pphiy[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(1);
			pphiz[facei] = pcTot[facei]*pMWmix[facei]*pU[facei].component(2);

			//if (viscositySpecies == true)
			{
				transportMapXML->DynamicViscosity(pmu[facei],moleFractions);
			}

            if (energyEquation == true || diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
				transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
				
				thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);			//[J/Kmol/K]
				pcp[facei] = pcp[facei]/pMWmix[facei];
			
				if (iMassDiffusionInEnergyEquation == true)
				{
					thermodynamicsMapXML->hMolar_Species(HVector);
					for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
						HSpecies[i].boundaryField()[patchi][facei] = HVector[i+1] / thermodynamicsMapXML->MW()[i+1];
				}
			}
			
			if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
			{
				transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
					Dmix[i].boundaryField()[patchi][facei] = Dmixvector[i+1];
			}
			else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
			{
				const double coefficient =  plambda[facei]/(pp[facei]*ppsi[facei])/pcp[facei];
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
					Dmix[i].boundaryField()[patchi][facei] = coefficient/LewisNumbers(i);
			}	

			// Thermal diffusion coefficients [-]
			if (soretEffect == true)
			{
				transportMapXML->ThermalDiffusionRatios(tetamixvector, moleFractions);
				for(int i=0;i<transportMapXML->iThermalDiffusionRatios().size();i++)
				{
					unsigned int index = transportMapXML->iThermalDiffusionRatios()[i];
					Dsoret[i].boundaryField()[patchi][facei] = Dmix[index-1].boundaryField()[patchi][facei]*tetamixvector[index]*thermodynamicsMapXML->MW()[index]/pMWmix[facei];
				}
			}		

            for(int i=0; i<indexCatalyst.size(); i++)
            {
                if (patchi == indexCatalyst[i])
                {
                    for(label i=0; i<Ygrad.size(); i++)
						Ygrad[i].boundaryField()[patchi][facei] *= thermodynamicsMapXML->MW()[i+1]*AlfaCatalyst/Dmix[i].boundaryField()[patchi][facei]/(pp[facei]*ppsi[facei]); 
			
                    if(energyEquation == true)
                    {
                     	  grad_T.boundaryField()[patchi][facei] *= AlfaCatalyst/plambda[facei];
                    }
                }
            }

		}
	}
}
