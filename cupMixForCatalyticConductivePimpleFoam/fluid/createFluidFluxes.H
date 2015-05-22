PtrList<surfaceScalarField> J(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<surfaceScalarField> Jc(fluidRegions.size());

forAll (fluidRegions,j)
{
    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        J.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new surfaceScalarField
            (
                IOobject
                (
                    "J::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionSet(1, 0, -1, 0, 0)
            )
        );
    }

    Jc.set
    (
        j,
        new surfaceScalarField
        (
            IOobject
            (
            "fluxes::Jc",
            runTime.timeName(),
            fluidRegions[j],
            IOobject::NO_READ,
            IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("J::Jc", dimensionSet(1, 0, -1, 0, 0), 0.0)
        )
    );
}

