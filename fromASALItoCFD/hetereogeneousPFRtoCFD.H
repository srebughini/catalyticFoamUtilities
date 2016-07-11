Info << endl;

forAll(fluidRegions, j)
{
    Info << "ASALI --> CFD for region: ";
    #include "setRegionFluidFields.H"

    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    const volVectorField& centers          = mesh.C();
    unsigned int          NS               = centers.size();
             int          cataliticCounter = 0;

    std::vector<double> dzVector(Nasali);
    double min;
    forAll(T.internalField(), celli)
    {
        for (unsigned int i=0;i<Nasali;i++)
        {
            dzVector[i] = std::fabs(centers[celli][directionIndex] - Zasali[i]);
        }

        min = dzVector[0];
        for (unsigned int i=1;i<Nasali;i++)
        {
            min = std::min(dzVector[i],min);
        }

        for (unsigned int i=0;i<Nasali;i++)
        {
            if ( std::fabs(centers[celli][directionIndex] - Zasali[i]) == min)
            {
                if ( ResultsAsali == "mass" )
                {
                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if (fabs(omegaBULKasali[i][k]) < 1e-09)
                        {
                            YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = std::max(0.,omegaBULKasali[i][k]);
                        }
                    }
                }
                else if ( ResultsAsali == "mole" )
                {
                    double dummy = 0.;
                    double sum   = 0.;
                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if ( k != inertIndexFluid[j] )
                        {
                            if (fabs(omegaBULKasali[i][k]) < 1e-09)
                            {
                                moleFractions[k+1] = 0.;
                            }
                            else
                            {
                                moleFractions[k+1] = omegaBULKasali[i][k];
                                sum                = sum  + moleFractions[k+1];
                            }
                        }
                    }

                    moleFractions[inertIndexFluid[j]+1] = std::max(0.,1. - sum);
                    
                    thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if (fabs(massFractions[k+1]) < 1e-09)
                        {
                            YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = std::max(0.,massFractions[k+1]);
                        }
                    }
                }

                if ( energyFluid[j] == true )
                {
                    T.internalField()[celli] = TBULKasali[i];
                }

                for (unsigned int k=0;k<3;k++)
                {
                    if ( k == directionIndex)
                    {
                        U.internalField()[celli][k] = Vasali[i];
                    }
                    else
                    {
                        U.internalField()[celli][k] = 0.;
                    }
                }

                break;
            }
        }
    }

    //- Inert composition
    {
        forAll(TFluid[j].internalField(), celli)
        {
            double sum = 0.;

            for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                if ( i != inertIndexFluid[j] )
                {
                    sum += YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
                }
            }

            YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+inertIndexFluid[j]].internalField()[celli] = std::max(0.,1. - sum);
        }

        forAll(TFluid[j].boundaryField(), patchi)
        {
        
            forAll(TFluid[j].boundaryField()[patchi], facei)
            {
                double sum = 0.;

                for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    if ( i != inertIndexFluid[j] )
                    {
                        sum += YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei];
                    }
                }

                YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+inertIndexFluid[j]].boundaryField()[patchi][facei] = std::max(0.,1. - sum);
            }
        }
    }

    if ( energyFluid[j] == true )
    {
        T.write();
    }

    U.write();

    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
    }

    Info << fluidRegions[j].name() << nl << endl;
}


forAll(solidRegions, j)
{
    Info << "ASALI --> CFD for region: ";
    #include "setRegionSolidFields.H"

    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    const volVectorField& centers = mesh.C();
    unsigned int          NS      = centers.size();

    std::vector<double> dzVector(Nasali);
    double min;
    unsigned int counter = 1;
    forAll(T.internalField(), celli)
    {
        for (unsigned int i=0;i<Nasali;i++)
        {
            dzVector[i] = std::fabs(centers[celli][directionIndex] - Zasali[i]);
        }

        min = dzVector[0];
        for (unsigned int i=1;i<Nasali;i++)
        {
            min = std::min(dzVector[i],min);
        }

        for (unsigned int i=0;i<Nasali;i++)
        {
            if ( std::fabs(centers[celli][directionIndex] - Zasali[i]) == min)
            {
                if ( ResultsAsali == "mass" )
                {
                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if (fabs(omegaWALLasali[i][k]) < 1e-09)
                        {
                            YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = std::max(0.,omegaWALLasali[i][k]);
                        }
                    }
                }
                else if ( ResultsAsali == "mole" )
                {
                    double dummy = 0.;
                    double sum   = 0.;
                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if ( k != inertIndexSolid[j] )
                        {
                            if (fabs(omegaWALLasali[i][k]) < 1e-09)
                            {
                                moleFractions[k+1] = 0.;
                            }
                            else
                            {
                                moleFractions[k+1] = omegaWALLasali[i][k];
                                sum                = sum  + moleFractions[k+1];
                            }
                        }
                    }
                    
                    moleFractions[inertIndexSolid[j]+1] = 1. - sum;


                    thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if (fabs(massFractions[k+1]) < 1e-09)
                        {
                            YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = std::max(0.,massFractions[k+1]);
                        }
                    }
                }

                if ( coverageSolid[j] == true )
                {
                    for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
                    {
                        ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+k].internalField()[celli] = SITEasali[i][k];
                    }
                }

                if ( energySolid[j] == true )
                {
                    T.internalField()[celli] = TWALLasali[i];
                }

                break;
            }
        }
    }

    //- Estimation of inert composition
    {
        forAll(TSolid[j].internalField(), celli)
        {
            double sum = 0.;

            for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                if ( i != inertIndexSolid[j] )
                {
                    sum += YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
                }
            }

            YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+inertIndexSolid[j]].internalField()[celli] = std::max(0.,1. - sum);
        }

        forAll(TSolid[j].boundaryField(), patchi)
        {
        
            forAll(TSolid[j].boundaryField()[patchi], facei)
            {
                double sum = 0.;

                for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    if ( i != inertIndexSolid[j] )
                    {
                        sum += YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].boundaryField()[patchi][facei];
                    }
                }

                YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+inertIndexSolid[j]].boundaryField()[patchi][facei] = std::max(0.,1. - sum);
            }
        }
    }

    //- Estimation of catalyst active site composition
    {
        forAll(TSolid[j].internalField(), celli)
        {
            double sum = 0.;
            for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
            {
                if ( i != catalystIndexSolid[j] )
                {
                    sum += ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField()[celli];
                }
            }

            ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species() + catalystIndexSolid[j]].internalField()[celli] = std::max(0.,1.-sum);
        }
    }

    if ( energySolid[j] == true )
    {
        T.write();
    }

    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
    }

    if ( coverageSolid[j] == true )
    {
        for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
        {
            ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].write();
        }
    }

    Info << solidRegions[j].name() << nl << endl;
}
