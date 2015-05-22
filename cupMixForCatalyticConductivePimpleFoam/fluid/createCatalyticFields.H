PtrList<volScalarField> siteFraction(surfaceThermodynamicsMapXML->number_of_site_species()*fluidRegions.size());

forAll(fluidRegions, j)
{
    Info<< "*** Reading region " << fluidRegions[j].name() << "\n" << endl;

    //- Reading adsorbed species
    for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
    {
        std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
        Info << "Reading adsorbed species: " << name << " > ";
        IOobject header
        (
            name,
            runTime.timeName(),
            fluidRegions[j],
            IOobject::NO_READ
        );

        // check if field exists and can be read
        if (header.headerOk())
        {
            Info << name << endl;
            siteFraction.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        fluidRegions[j],
                        IOobject::MUST_READ,
                        IOobject::AUTO_WRITE
                    ),
                    fluidRegions[j]
                )
            );
        }
        else
        {
            volScalarField thetaDefault
            (
                IOobject
                (
                    "thetaDefault",
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j]
            );

            siteFraction.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        fluidRegions[j],
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    thetaDefault
                )
            );
            
            if( i == catalystIndex)
            {
                forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField(), celli)
                {
                    siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField()[celli] = 1.;
                }

                forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField(), patchi)
                {
                    forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi], facei)
                    {    
                        siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi][facei] = 1.;
                    }
                }
                Info << "catalyst's material" << endl;
            }
            else
            {
                Info << "thetaDefault" << endl;
            }
        }
    }
    
    Info << endl;
}
