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


volScalarField MWmix
(
    IOobject
    (
        "MW",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(1, 0, 0, 0, -1)
);

volScalarField cTot
(
    IOobject
    (
        "cTot",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, -3, 0, 0, 1)
);


volScalarField psi
(
    IOobject
    (
        "psi",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, -2, 2, 0, 0)
);

volScalarField mu
(
    IOobject
    (
        "mu",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0), 1.8e-5)
);

volScalarField lambda
(
    IOobject
    (
        "K",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", dimensionSet(1, 1, -3, -1, 0), 1.8e-2)
);

volScalarField cp
(
    IOobject
    (
        "Cp",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, 2, -2, -1, 0)
);

volScalarField h
(
    IOobject
    (
        "h",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, 2, -2, 0, 0)
);

volScalarField heatReaction
(
    IOobject
    (
        "R::T",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(1, -1, -3, 0, 0)
);

volScalarField sourceImplicitTemp
(
    IOobject
    (
        "SI::T",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("SI::T", dimensionSet(1, -1, -3, -1, 0), 0.0)
);

volScalarField sourceExplicitTemp
(
    IOobject
    (
        "SE::T",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("SE::T", dimensionSet(1, -1, -3, 0, 0), 0.0)
);

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> X(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Dmix(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> rD(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> R(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> P(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> sourceImplicitSpecie(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> sourceExplicitSpecie(thermodynamicsMapXML->NumberOfSpecies());

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

    {
        Dmix.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Dmix::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0), 1.8e-5)
            )
        );
    }

    {
        X.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "X::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimensionSet(0, 0, 0, 0, 0), 0.0)
            )
        );
    }


    {
        rD.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "rD::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0),1.8e-06)
            )
        );
    }

    {
        R.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "R::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );
    }

    {
        P.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "P::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );
    }

    {
        sourceImplicitSpecie.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "SI::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("SI", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );
    }

    {
        sourceExplicitSpecie.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "SE::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("SE", dimensionSet(1, -3, -1, 0, 0), 0.0)
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
