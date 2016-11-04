Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{

    Info << "Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;

    {
        Y.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh
            )
        );
    }
}

Info << endl;

PtrList<volScalarField> Z(surfaceThermodynamicsMapXML->number_of_site_species());

for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
{
    std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
    Info << "Reading species: " << name << " > ";

    IOobject header
    (
        name,
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ
    );

    if (header.headerOk())
    {
        Info << name << endl;
        Z.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    name,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh
            )
        );
    }
    else
    {
        Info << "thetaDefault" << endl;

        volScalarField Zdefault
        (
            IOobject
            (
                "thetaDefault",
                mesh.time().timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh
        );

        Z.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    name,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                Zdefault
            )
        );
    }
}

Info << endl;
