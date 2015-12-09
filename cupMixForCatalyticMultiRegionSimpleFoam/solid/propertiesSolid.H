forAll(solidRegions, j)
{
    OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    // Internal fields
    {
        const scalarField& TCells = TSolid[j].internalField();
        const scalarField& pCells = pSolid[j].internalField(); 

        scalarField& muCells      = muSolid[j].internalField();
        scalarField& lambdaCells  = lambdaSolid[j].internalField();
        scalarField& cpCells      = cpSolid[j].internalField();
        scalarField& MWmixCells   = MWmixSolid[j].internalField();
        scalarField& psiCells     = psiSolid[j].internalField();
        scalarField& rhoCells     = rhoSolid[j].internalField();

        forAll(TCells, celli)
        {
            thermodynamicsMapXML->SetPressure(pCells[celli]);
            thermodynamicsMapXML->SetTemperature(TCells[celli]);
            
            transportMapXML->SetPressure(pCells[celli]);
            transportMapXML->SetTemperature(TCells[celli]);
                
    
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                XSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = moleFractions[i+1];
            }

            transportMapXML->DynamicViscosity(muCells[celli],moleFractions);

            transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);
            cpCells[celli] = cpCells[celli]/MWmixCells[celli];
			
			psiCells[celli]  = MWmixCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
			rhoCells[celli]  = psiCells[celli]*pCells[celli];

			if ( diffusivityLaw[j] == "EffectiveDiffusivityModel" )
            {
				transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				{
					DmixSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = Dmixvector[i+1]*(epsiM[j]/tauM[j])*symmTensor::I;
					rDSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = Dmixvector[i+1]*(epsiM[j]/tauM[j])*symmTensor::I*pCells[celli]*MWmixCells[celli]/(PhysicalConstants::R_J_kmol*TCells[celli]);
				}
			}
        }
    }

    // Boundaries
    forAll(TSolid[j].boundaryField(), patchi)
    {
        fvPatchScalarField& pp      = pSolid[j].boundaryField()[patchi];
        fvPatchScalarField& pT      = TSolid[j].boundaryField()[patchi];
        
        fvPatchScalarField& pmu     = muSolid[j].boundaryField()[patchi];
        fvPatchScalarField& plambda = lambdaSolid[j].boundaryField()[patchi];
        fvPatchScalarField& pcp     = cpSolid[j].boundaryField()[patchi];
        fvPatchScalarField& pMWmix  = MWmixSolid[j].boundaryField()[patchi];
        fvPatchScalarField& ppsi    = psiSolid[j].boundaryField()[patchi];
        fvPatchScalarField& prho    = rhoSolid[j].boundaryField()[patchi];
        
        forAll(pT, facei)
        {
            thermodynamicsMapXML->SetPressure(pp[facei]);
            thermodynamicsMapXML->SetTemperature(pT[facei]);

            transportMapXML->SetPressure(pp[facei]);
            transportMapXML->SetTemperature(pT[facei]);
            
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                XSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = moleFractions[i+1];
            }

            transportMapXML->DynamicViscosity(pmu[facei],moleFractions);

            transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);
            pcp[facei] = pcp[facei]/pMWmix[facei];

			ppsi[facei]  = pMWmix[facei]/PhysicalConstants::R_J_kmol/pT[facei];
			prho[facei]  = ppsi[facei]*pp[facei];

			if ( diffusivityLaw[j] == "EffectiveDiffusivityModel" )
            {
				transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
				for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				{
					DmixSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = Dmixvector[i+1]*(epsiM[j]/tauM[j])*symmTensor::I;
					rDSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei]   = Dmixvector[i+1]*(epsiM[j]/tauM[j])*symmTensor::I*pp[facei]*pMWmix[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
				}
			}
        }
    }
}

