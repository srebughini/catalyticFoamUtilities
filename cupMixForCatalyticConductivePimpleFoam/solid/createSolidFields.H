// Initialise solid field pointer lists
PtrList<volScalarField> TSolid(solidRegions.size());
PtrList<volScalarField> lambdaSolid(solidRegions.size());
PtrList<volScalarField> cpSolid(solidRegions.size());
PtrList<volScalarField> rhoSolid(solidRegions.size());
PtrList<volScalarField> pSolid(solidRegions.size());

// Populate solid field pointer lists
forAll(solidRegions, j)
{
    Info<< "*** Reading region " << solidRegions[j].name() << "\n" << endl;   

    Info<< "Creating field rho\n" << endl;
    rhoSolid.set
    (   
        j,
        new volScalarField
        (
            IOobject
            (
                "rhoMaterial",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
        solidRegions[j],
        dimensionedScalar("rhoMaterial", dimensionSet(1, -3, 0, 0, 0),rhoS[j])
        )
    ); 

    Info<< "Reading field T\n" << endl;
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

    Info<< "Reading field p\n" << endl;
    pSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "p",
                runTime.timeName(),
                solidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[j]
        )
    );     

    Info<< "Creating field lambda\n" << endl;
    lambdaSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "K",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("K", dimensionSet(1, 1, -3, -1, 0),lambdaS[j])
        )
    );

    Info<< "Creating field cp\n" << endl;
    cpSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "specificHeats",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("specificHeats", dimensionSet(0, 2, -2, -1, 0),cpS[j])
        )
    );
}
