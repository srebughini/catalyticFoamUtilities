PtrList<volVectorField>     UFluid(fluidRegions.size());
PtrList<volScalarField>     TFluid(fluidRegions.size());

PtrList<volScalarField>     YFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());


forAll(fluidRegions, j)
{
    UFluid.set
    (
        j,
        new volVectorField
        (
            IOobject
            (
                "U",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[j]
        )
    );
        
    TFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "T",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[j]
        )
    );    

    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        IOobject header
        (
            thermodynamicsMapXML->NamesOfSpecies()[i],
            runTime.timeName(),
            fluidRegions[j],
            IOobject::NO_READ
        );
        
        YFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[j]
            )
        );
	}
}
