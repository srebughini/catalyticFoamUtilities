forAll(fluidRegions, j)
{
    OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    // Internal fields
    {
        const scalarField& TCells = TFluid[j].internalField();
        const scalarField& pCells = pFluid[j].internalField(); 

        scalarField& muCells      = muFluid[j].internalField();
        scalarField& lambdaCells  = lambdaFluid[j].internalField();
        scalarField& cpCells      = cpFluid[j].internalField();
        scalarField& MWmixCells   = MWmixFluid[j].internalField();
        scalarField& psiCells     = psiFluid[j].internalField();

        forAll(TCells, celli)
        {
            thermodynamicsMapXML->SetPressure(pCells[celli]);
            thermodynamicsMapXML->SetTemperature(TCells[celli]);
            
            transportMapXML->SetPressure(pCells[celli]);
            transportMapXML->SetTemperature(TCells[celli]);
                
    
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                XFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = moleFractions[i+1];
            }

            transportMapXML->DynamicViscosity(muCells[celli],moleFractions);

            transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);
            cpCells[celli] = cpCells[celli]/MWmixCells[celli];
			
			psiCells[celli]  = MWmixCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];

			transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			{
				DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = Dmixvector[i+1];
				rDFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli]   = Dmixvector[i+1]*pCells[celli]*MWmixCells[celli]/(PhysicalConstants::R_J_kmol*TCells[celli]);
			}

        }
    }

    // Boundaries
    forAll(TFluid[j].boundaryField(), patchi)
    {
        fvPatchScalarField& pp      = pFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pT      = TFluid[j].boundaryField()[patchi];
        
        fvPatchScalarField& pmu     = muFluid[j].boundaryField()[patchi];
        fvPatchScalarField& plambda = lambdaFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pcp     = cpFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pMWmix  = MWmixFluid[j].boundaryField()[patchi];
        fvPatchScalarField& ppsi    = psiFluid[j].boundaryField()[patchi];
        
        forAll(pT, facei)
        {
            thermodynamicsMapXML->SetPressure(pp[facei]);
            thermodynamicsMapXML->SetTemperature(pT[facei]);

            transportMapXML->SetPressure(pp[facei]);
            transportMapXML->SetTemperature(pT[facei]);
            
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                XFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = moleFractions[i+1];
            }

            transportMapXML->DynamicViscosity(pmu[facei],moleFractions);

            transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);
            pcp[facei] = pcp[facei]/pMWmix[facei];

			ppsi[facei]  = pMWmix[facei]/PhysicalConstants::R_J_kmol/pT[facei];

			transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
			{
				DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = Dmixvector[i+1];
				rDFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei]   = Dmixvector[i+1]*pp[facei]*pMWmix[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
			}
        }
    }
}

