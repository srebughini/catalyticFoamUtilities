PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Dmix(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Sc(thermodynamicsMapXML->NumberOfSpecies());

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{

    Info << "Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

    IOobject header
    (
        thermodynamicsMapXML->NamesOfSpecies()[i],
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ
    );

    if (header.headerOk())
    {
        Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
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
    else
    {
        Info << "Ydefault" << endl;

        volScalarField Ydefault
        (
            IOobject
            (
                "Ydefault",
                mesh.time().timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh
        );

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
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                Ydefault
            )
        );
    }

    {
        Dmix.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Dmix_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionSet(0, 2, -1, 0, 0)
            )
        );
    }

    {
        Sc.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Sc_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionSet(0, 0, 0, 0, 0)
            )
        );
    }
 
}

mu = dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0), 1.8e-5);

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{
    Dmix[i] = dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0), 1.8e-5);
    Sc[i]   = dimensionedScalar("zero", dimensionSet(0, 0, 0, 0, 0),  0.7);
}

