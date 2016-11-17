{
    Info<< "Properties evaluation ";
    double tStart = OpenSMOKE::OpenSMOKEGetCpuTime();
    OpenSMOKE::OpenSMOKEVectorDouble HVector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble Dmixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    // Internal fields
    {
        const scalarField& TCells = T.internalField();
        const scalarField& pCells = p.internalField(); 

        scalarField& cTotCells    = cTot.internalField();
        scalarField& psiCells     = psi.internalField();
        scalarField& muCells      = mu.internalField();
        scalarField& lambdaCells  = lambda.internalField();
        scalarField& cpCells      = cp.internalField();
        scalarField& hCells       = h.internalField();
        scalarField& MWmixCells   = MWmix.internalField();

        forAll(TCells, celli)
        {
            thermodynamicsMapXML->SetPressure(pCells[celli]);
            thermodynamicsMapXML->SetTemperature(TCells[celli]);
            
            transportMapXML->SetPressure(pCells[celli]);
            transportMapXML->SetTemperature(TCells[celli]);
    
            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = Y[i].internalField()[celli];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,MWmixCells[celli],massFractions);

            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                X[i].internalField()[celli] = moleFractions[i+1];
            }

            cTotCells[celli] = pCells[celli]/PhysicalConstants::R_J_kmol/TCells[celli];
            psiCells[celli]  = cTotCells[celli]*MWmixCells[celli]/pCells[celli];
            thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(hCells[celli], moleFractions);
            hCells[celli] /= MWmixCells[celli];

            transportMapXML->DynamicViscosity(muCells[celli],moleFractions);

            transportMapXML->ThermalConductivity(lambdaCells[celli],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(cpCells[celli], moleFractions);
            cpCells[celli] = cpCells[celli]/MWmixCells[celli];

            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector, moleFractions);
                for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].internalField()[celli] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient = lambdaCells[celli]/(pCells[celli]*psiCells[celli])/cpCells[celli];
                for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].internalField()[celli] = coefficient/LewisNumbers(i);
                }
            }

            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                rD[i].internalField()[celli] = Dmix[i].internalField()[celli]*cTotCells[celli]*MWmixCells[celli];
            }
        }
    }

    // Boundaries
    forAll(T.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = p.boundaryField()[patchi];
        fvPatchScalarField& pT = T.boundaryField()[patchi];
        
        fvPatchScalarField& pcTot   = cTot.boundaryField()[patchi];
        fvPatchScalarField& ppsi    = psi.boundaryField()[patchi];
        fvPatchScalarField& pmu     = mu.boundaryField()[patchi];
        fvPatchScalarField& plambda = lambda.boundaryField()[patchi];
        fvPatchScalarField& pcp     = cp.boundaryField()[patchi];
        fvPatchScalarField& ph      = h.boundaryField()[patchi];
        fvPatchScalarField& pMWmix  = MWmix.boundaryField()[patchi];
        
        forAll(pT, facei)
        {
            thermodynamicsMapXML->SetPressure(pp[facei]);
            thermodynamicsMapXML->SetTemperature(pT[facei]);

            transportMapXML->SetPressure(pp[facei]);
            transportMapXML->SetTemperature(pT[facei]);
            
            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                massFractions[i+1] = Y[i].boundaryField()[patchi][facei];
            }

            thermodynamicsMapXML->MoleFractions_From_MassFractions(moleFractions,pMWmix[facei],massFractions);

            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                X[i].boundaryField()[patchi][facei] = moleFractions[i+1];
            }

            pcTot[facei] = pp[facei]/(PhysicalConstants::R_J_kmol*pT[facei]);
            ppsi[facei]  = pcTot[facei]*pMWmix[facei]/pp[facei];
            thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(ph[facei], moleFractions);
            ph[facei] /= pMWmix[facei];

            transportMapXML->DynamicViscosity(pmu[facei],moleFractions);

            transportMapXML->ThermalConductivity(plambda[facei],moleFractions);
            
            thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(pcp[facei], moleFractions);
            pcp[facei] = pcp[facei]/pMWmix[facei];
            
            if (diffusivityModel == DIFFUSIVITY_MODEL_MULTICOMPONENT)
            {
                transportMapXML->MassDiffusionCoefficients(Dmixvector,moleFractions);
                for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].boundaryField()[patchi][facei] = Dmixvector[i+1];
                }
            }
            else if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
            {
                const double coefficient =  plambda[facei]/(pp[facei]*ppsi[facei])/pcp[facei];
                for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    Dmix[i].boundaryField()[patchi][facei] = coefficient/LewisNumbers(i);
                }
            }
            
            for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                rD[i].boundaryField()[patchi][facei] = Dmix[i].boundaryField()[patchi][facei]*pcTot[facei]*pMWmix[facei];
            }
            
        }
    }

    double tEnd = OpenSMOKE::OpenSMOKEGetCpuTime();
    Info << "in " << tEnd - tStart << " s" << endl;
}
