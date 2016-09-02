PtrList<volScalarField>     cupFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     gFluid(fluidRegions.size());
PtrList<volScalarField>     cupTFluid(fluidRegions.size());

forAll(fluidRegions, j)
{

    gFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "G",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(1, -2, -1, 0, 0), 0.0)
        )
    );

    gFluid[j] = rhoFluid[j]*mag(UFluid[j]);

    cupTFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "cup_T",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(1, -2, -1, 1, 0), 0.0)
        )
    );

    cupTFluid[j] = gFluid[j]*TFluid[j];


    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        cupFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "cup_"+thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, -2, -1, 0, 0), 0.0)
            )
        );

        cupFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i] = YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i]*gFluid[j];
	}
}
