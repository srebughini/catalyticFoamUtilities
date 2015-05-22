forAll(fluidRegions, j)
{
    Info<< "Properties evaluation..." << endl;

    OpenSMOKE::OpenSMOKEVectorDouble HVector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    // Internal fields
    {
        const scalarField& TCells = TFluid[j].internalField();
        const scalarField& pCells = pFluid[j].internalField(); 

        scalarField& cTotCells   = cTotFluid[j].internalField();
        scalarField& psiCells    = psiFluid[j].internalField();
        scalarField& muCells     = muFluid[j].internalField();
        scalarField& lambdaCells = lambdaFluid[j].internalField();
        scalarField& cpCells     = cpFluid[j].internalField();
        scalarField& MWmixCells  = MWmixFluid[j].internalField();

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

            cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
            psiCells[celli]  = cTotCells[celli]*MWmixCells[celli]/pCells[celli];

            transportMapXML->DynamicViscosity(muCells[celli],moleFractions);

            if (energyEquation == true || diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
                
                thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);
                cpCells[celli] = cpCells[celli]/MWmixCells[celli];

                if (iMassDiffusionInEnergyEquation == true)
                {
                    thermodynamicsMapXML->hMolar_Species(HVector);
                    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                    {
                        HFluidSpecies[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = HVector[i+1]/thermodynamicsMapXML->MW()[i+1];
                    }
                }
            }
            
            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient = lambdaCells[celli]/(pCells[celli]*psiCells[celli])/cpCells[celli];
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli] = coefficient/LewisNumbers(i);
                }
            }
        }
    }

    // Boundaries
    forAll(TFluid[j].boundaryField(), patchi)
    {
        fvPatchScalarField& pp = pFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pT = TFluid[j].boundaryField()[patchi];

        fvPatchScalarField& pcTot   = cTotFluid[j].boundaryField()[patchi];
        fvPatchScalarField& ppsi    = psiFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pmu     = muFluid[j].boundaryField()[patchi];
        fvPatchScalarField& plambda = lambdaFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pcp     = cpFluid[j].boundaryField()[patchi];
        fvPatchScalarField& pMWmix  = MWmixFluid[j].boundaryField()[patchi];
        
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

            pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
            ppsi[facei]  = pcTot[facei]*pMWmix[facei]/pp[facei];

            transportMapXML->DynamicViscosity(pmu[facei],moleFractions);

            if (energyEquation == true || diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
                
                thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);            //[J/Kmol/K]
                pcp[facei] = pcp[facei]/pMWmix[facei];
            
                if (iMassDiffusionInEnergyEquation == true)
                {
                    thermodynamicsMapXML->hMolar_Species(HVector);
                    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                    {
                        HFluidSpecies[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = HVector[i+1]/thermodynamicsMapXML->MW()[i+1];
                    }
                }
            }
            
            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient =  plambda[facei]/(pp[facei]*ppsi[facei])/pcp[facei];
                for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    DmixFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei] = coefficient/LewisNumbers(i);
                }
            }
        }
    }
}

