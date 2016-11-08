//- Internal Field
Info << "ASALI --> CFD ";
{
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
                            Y[k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            Y[k].internalField()[celli] = std::max(0.,omegaBULKasali[i][k]);
                        }
                    }
                }
                else if ( ResultsAsali == "mole" )
                {
                    double dummy = 0.;
                    double sum   = 0.;
                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if ( k != inertIndex )
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

                    moleFractions[inertIndex+1] = std::max(0.,1. - sum);
                    
                    thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

                    for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                    {
                        if (fabs(massFractions[k+1]) < 1e-09)
                        {
                            Y[k].internalField()[celli] = 0.;
                        }
                        else
                        {
                            Y[k].internalField()[celli] = std::max(0.,massFractions[k+1]);
                        }
                    }
                }

                {
                    for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
                    {
                        Z[k].internalField()[celli] = SITEasali[i][k];
                    }
                }

                if ( energy == true )
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
        forAll(T.internalField(), celli)
        {
            double sum = 0.;

            for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                if ( i != inertIndex )
                {
                    sum += Y[i].internalField()[celli];
                }
            }

            Y[inertIndex].internalField()[celli] = std::max(0.,1. - sum);
        }
    }

    //- Estimation of catalyst active site composition
    {
        forAll(T.internalField(), celli)
        {
            double sum = 0.;
            for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
            {
                if ( i != catalystIndex )
                {
                    sum += Z[i].internalField()[celli];
                }
            }

            Z[catalystIndex].internalField()[celli] = std::max(0.,1.-sum);
        }
    }
}
Info << "internal fields" << nl << endl;

Info << "ASALI --> CFD ";
{
    OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

    forAll(T.boundaryField(), patchi)
    {
        if ( Y[0].boundaryField()[patchi].type() == "compressible::catalyticWall" )
        {
            const fvsPatchVectorField& centers          = mesh.Cf().boundaryField()[patchi];
            unsigned int              NS               = centers.size();
                     int              cataliticCounter = 0;

            std::vector<double> dzVector(Nasali);
            double min;

            forAll(T.boundaryField()[patchi], facei)
            {
                for (unsigned int i=0;i<Nasali;i++)
                {
                    dzVector[i] = std::fabs(centers[facei][directionIndex] - Zasali[i]);
                }

                min = dzVector[0];
                for (unsigned int i=1;i<Nasali;i++)
                {
                    min = std::min(dzVector[i],min);
                }

                for (unsigned int i=0;i<Nasali;i++)
                {
                    if ( std::fabs(centers[facei][directionIndex] - Zasali[i]) == min)
                    {
                        if ( ResultsAsali == "mass" )
                        {
                            for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                            {
                                if (fabs(omegaBULKasali[i][k]) < 1e-09)
                                {
                                    Y[k].boundaryField()[patchi][facei] = 0.;
                                }
                                else
                                {
                                    Y[k].boundaryField()[patchi][facei] = std::max(0.,omegaWALLasali[i][k]);
                                }
                            }
                        }
                        else if ( ResultsAsali == "mole" )
                        {
                            double dummy = 0.;
                            double sum   = 0.;
                            for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                            {
                                if ( k != inertIndex )
                                {
                                    if (fabs(omegaBULKasali[i][k]) < 1e-09)
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

                            moleFractions[inertIndex+1] = std::max(0.,1. - sum);
                            
                            thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

                            for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
                            {
                                if (fabs(massFractions[k+1]) < 1e-09)
                                {
                                    Y[k].boundaryField()[patchi][facei] = 0.;
                                }
                                else
                                {
                                    Y[k].boundaryField()[patchi][facei] = std::max(0.,massFractions[k+1]);
                                }
                            }
                        }

                        {
                            for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
                            {
                                Z[k].boundaryField()[patchi][facei] = SITEasali[i][k];
                            }
                        }

                        if ( energy == true )
                        {
                            T.boundaryField()[patchi][facei] = TWALLasali[i];
                        }

                        break;
                    }
                }
            }
        }
    }

    //- Inert composition
    {
        forAll(T.boundaryField(), patchi)
        {
            forAll(T.boundaryField()[patchi], facei)
            {
                double sum = 0.;

                for(label i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
                {
                    if ( i != inertIndex )
                    {
                        sum += Y[i].boundaryField()[patchi][facei];
                    }
                }

                Y[inertIndex].boundaryField()[patchi][facei] = std::max(0.,1. - sum);
            }
        }
    }

    //- Estimation of catalyst active site composition
    {
        forAll(T.boundaryField(), patchi)
        {
            forAll(T.boundaryField()[patchi], facei)
            {
                double sum = 0.;

                for(label i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
                {
                    if ( i != catalystIndex )
                    {
                        sum += Z[i].boundaryField()[patchi][facei];
                    }
                }

                Z[catalystIndex].boundaryField()[patchi][facei] = std::max(0.,1. - sum);
            }
        }
    }


}
Info << "boundary fields" << nl << endl;

//- Write
{
    if ( energy == true )
    {
        T.write();
    }

    U.write();

    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        Y[i].write();
    }

    for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
    {
        Z[i].write();
    }
}
