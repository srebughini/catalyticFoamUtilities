const wordList solidsNames(rp["solid"]);

PtrList<fvMesh> solidRegions(solidsNames.size());

forAll(solidsNames, i)
{
	Info<< "Create mesh for region " << solidsNames[i] << nl << endl;
	solidRegions.set
	(
		i,
		new fvMesh
		(
			IOobject
			(
				solidsNames[i],
				runTime.timeName(),
				runTime,
				IOobject::MUST_READ
			)
		)
	);
}

if ( energySolid.size() != solidRegions.size() )
{
	Info << nl << "ERROR in solverOptions/FromASALItoCFD/energyInSolid --> number of solid regions: " << solidRegions.size() << endl;
	abort();
}

if ( specieSolid.size() != solidRegions.size() )
{
	Info << nl << "ERROR in solverOptions/FromASALItoCFD/specieInSolid --> number of solid regions: " << solidRegions.size() << endl;
	abort();
}
