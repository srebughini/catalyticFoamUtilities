Info<< "Reading field U\n" << endl;
volVectorField U
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
);

//- Reading pressure [Pa]
Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j]
);

//- Reading temperature [K]
Info<< "Reading field T\n" << endl;
volScalarField T
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
);

//- Allocating memory for mean molecular weight [kg/mol]
Info<< "Creating field MWmix\n" << endl;
volScalarField MWmix
(
    IOobject
    (
        "gas::MW",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(1, 0, 0, 0, -1)
);

//- Allocating memory for total concentration [kmol/m3]
Info<< "Creating field cTot\n" << endl;
volScalarField cTot
(
    IOobject
    (
        "gas::cTot",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(0, -3, 0, 0, 1)
);


//- Allocating memory for p/rho [s2/m2]
Info<< "Creating field psi\n" << endl;
volScalarField psi
(
    IOobject
    (
        "psi",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(0, -2, 2, 0, 0)
);

//- Allocating memory for dynamic viscosity [kg/m/s]
Info<< "Creating field mu\n" << endl;
volScalarField mu
(
    IOobject
    (
        "gas::mu",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(1, -1, -1, 0, 0)
);

//- Allocating memory for thermal conductivity [W/m/K]
Info<< "Creating field lambda\n" << endl;
volScalarField lambda
(
    IOobject
    (
        "gas::lambda",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(1, 1, -3, -1, 0)
);

//- Allocating memory for mean specific heat [J/kg/K]
Info<< "Creating field Cp\n" << endl;
volScalarField cp
(
    IOobject
    (
        "gas::Cp",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(0, 2, -2, -1, 0)
);

//- Allocating memory for mean specific heat [J/kg/K]
Info<< "Creating field Cv\n" << endl;
volScalarField cv
(
    IOobject
    (
        "gas::Cv",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(0, 2, -2, -1, 0)
);

//- Allocating memory for total enthalpy [m2/s2]
Info<< "Creating field enthalpy\n" << endl;
volScalarField h
(
    IOobject
    (
        "gas::h",
        runTime.timeName(),
        fluidRegions[j],
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fluidRegions[j],
    dimensionSet(0, 2, -2, 0, 0)
);
