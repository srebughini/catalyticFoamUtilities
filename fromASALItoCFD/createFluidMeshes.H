const wordList fluidsNames(rp["fluid"]);

PtrList<fvMesh> fluidRegions(fluidsNames.size());

forAll(fluidsNames, i)
{
	Info<< "Create mesh for region " << fluidsNames[i] << nl << endl;

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

if ( energyFluid.size() != fluidRegions.size() )
{
	Info << nl << "ERROR in solverOptions/FromASALItoCFD/energyInFluid --> number of fluid regions: " << fluidRegions.size() << endl;
	abort();
}

if ( specieFluid.size() != fluidRegions.size() )
{
	Info << nl << "ERROR in solverOptions/FromASALItoCFD/specieInFluid --> number of fluid regions: " << fluidRegions.size() << endl;
	abort();
}
