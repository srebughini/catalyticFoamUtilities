PtrList<volScalarField>     cupMassFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     cupMoleFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     gFluid(fluidRegions.size());
PtrList<volScalarField>     cTotFluid(fluidRegions.size());
PtrList<volScalarField>     cupTFluid(fluidRegions.size());
PtrList<volScalarField>     cupQFluid(fluidRegions.size());

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

    cTotFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "cTot",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(0, -2, -1, 0, 1), 0.0)
        )
    );

    cTotFluid[j] = rhoFluid[j]*mag(UFluid[j])/MWmixFluid[j];


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

    cupQFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "cup_Q",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(2, -3, -4, 0, 0), 0.0)
        )
    );

    cupQFluid[j] = gFluid[j]*QFluid[j];


    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        cupMassFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "cup_mass_"+thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, -2, -1, 0, 0), 0.0)
            )
        );

        cupMassFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i] = YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i]*gFluid[j];

        cupMoleFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "cup_mole_"+thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(0, -2, -1, 0, 1), 0.0)
            )
        );

        cupMoleFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i] = XFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i]*cTotFluid[j];

	}
}
