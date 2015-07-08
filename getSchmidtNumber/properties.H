{
    OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    // Internal fields
    {
        const scalarField& TCells = T.internalField();
        const scalarField& pCells = p.internalField(); 

        scalarField& muCells      = mu.internalField();
        scalarField& lambdaCells  = lambda.internalField();
        scalarField& cpCells      = cp.internalField();
        scalarField& MWmixCells   = MWmix.internalField();

        forAll(TCells, celli)
        {
            thermodynamicsMapXML->SetPressure(pCells[celli]);
            thermodynamicsMapXML->SetTemperature(TCells[celli]);
            
            transportMapXML->SetPressure(pCells[celli]);
            transportMapXML->SetTemperature(TCells[celli]);
                
    
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = Y[i].internalField()[celli];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

            transportMapXML->DynamicViscosity(muCells[celli],moleFractions);


            if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
                
                thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);
                cpCells[celli] = cpCells[celli]/MWmixCells[celli];
            }

            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].internalField()[celli] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient = lambdaCells[celli]/(MWmixCells[celli]*pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli])/cpCells[celli];
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].internalField()[celli] = coefficient/LewisNumbers(i);
                }
            }
        }
    }

    // Boundaries
    forAll(T.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = p.boundaryField()[patchi];
        fvPatchScalarField& pT = T.boundaryField()[patchi];
        fvPatchScalarField& pmu     = mu.boundaryField()[patchi];
        fvPatchScalarField& plambda = lambda.boundaryField()[patchi];
        fvPatchScalarField& pcp     = cp.boundaryField()[patchi];
        fvPatchScalarField& pMWmix  = MWmix.boundaryField()[patchi];
        
        forAll(pT, facei)
        {
            thermodynamicsMapXML->SetPressure(pp[facei]);
            thermodynamicsMapXML->SetTemperature(pT[facei]);

            transportMapXML->SetPressure(pp[facei]);
            transportMapXML->SetTemperature(pT[facei]);
            
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = Y[i].boundaryField()[patchi][facei];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

            transportMapXML->DynamicViscosity(pmu[facei],moleFractions);

            if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
                
                thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);            //[J/Kmol/K]
                pcp[facei] = pcp[facei]/pMWmix[facei];
            }
            
            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].boundaryField()[patchi][facei] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient =  plambda[facei]/(pMWmix[facei]*pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]))/pcp[facei];
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].boundaryField()[patchi][facei] = coefficient/LewisNumbers(i);
                }
            }
        }
    }
}
