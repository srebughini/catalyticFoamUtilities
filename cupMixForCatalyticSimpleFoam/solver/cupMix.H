volScalarField G
(
    IOobject
    (
        "G",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    rho*mag(U)
);

volScalarField cupT
(
    IOobject
    (
        "cup_T",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    G*T
);

PtrList<volScalarField>     cupMass(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField>     cupMole(thermodynamicsMapXML->NumberOfSpecies());
for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{
    cupMass.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "cup_mass_"+thermodynamicsMapXML->NamesOfSpecies()[i],
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Y[i]*G
        )
    );

    cupMole.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "cup_mole_"+thermodynamicsMapXML->NamesOfSpecies()[i],
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            X[i]*cTot
        )
    );
}
