const wordList fluidsNames(rp["fluid"]);

if ( fluidsNames.size() != 1 )
{
    Info << "Wrong number of fluid regions: catalyticConductivePimpleFoam has been developed for only ONE fluid regions." << endl;
    Info << "Do you need more fluid regions? move to catalyticMultiRegionPimpleFoam." << endl;
    abort();
}

PtrList<fvMesh> fluidRegions(fluidsNames.size());

forAll(fluidsNames, i)
{
    Info<< "Create mesh for region " << fluidsNames[i] << " for time = " << runTime.timeName() << nl << endl;

    fluidRegions.set
    (
        i,
        new fvMesh
        (
            IOobject
            (
                fluidsNames[i],
                runTime.timeName(),
                runTime,
                IOobject::MUST_READ
            )
        )
    );
}

