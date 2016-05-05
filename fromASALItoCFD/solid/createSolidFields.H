PtrList<volScalarField>     TSolid(solidRegions.size());

PtrList<volScalarField>     YSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());

PtrList<volScalarField>     ZSolid(surfaceThermodynamicsMapXML->number_of_site_species()*solidRegions.size());

forAll(solidRegions, j)
{
    TSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "T",
                runTime.timeName(),
                solidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[j]
        )
    );     

    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        IOobject header
        (
            thermodynamicsMapXML->NamesOfSpecies()[i],
            runTime.timeName(),
            solidRegions[j],
            IOobject::NO_READ
        );
        
        YSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[j]
            )
        );
    }


    for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
    {
        std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];

        IOobject header
        (
            name,
            runTime.timeName(),
            solidRegions[j],
            IOobject::NO_READ
        );

        if (header.headerOk())
        {
            ZSolid.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        solidRegions[j],
                        IOobject::MUST_READ,
                        IOobject::AUTO_WRITE
                    ),
                    solidRegions[j]
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
                    solidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j]
            );

            ZSolid.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        solidRegions[j],
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    thetaDefault
                )
            );
        }
    }
}
