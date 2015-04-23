const wordList fluidsNames(rp["fluid"]);

PtrList<fvMesh> fluidRegions(fluidsNames.size());

forAll(fluidsNames, i)
{
	Info<< "Create solid mesh for region " << fluidsNames[i]
		<< " for time = " << runTime.timeName() << nl << endl;

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

