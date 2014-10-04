Info<< "\nReading field T\n" << endl;
volScalarField T
(
	IOobject
	(		
		"T",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	),
mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
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
        IOobject::NO_WRITE
    ),
    mesh
);

Info<< "Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{

	Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

	IOobject header
	(
		thermodynamicsMapXML->NamesOfSpecies()[i],
		mesh.time().timeName(),
		mesh,
		IOobject::NO_READ
	);

	// check if field exists and can be read
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
					IOobject::NO_WRITE
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
					IOobject::NO_WRITE
				),
				Ydefault
			)
		);

	}
}
Info << endl;
