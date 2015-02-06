Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> x(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> massPerPhi(thermodynamicsMapXML->NumberOfSpecies());

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

	x.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"mole_"+thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,
			dimensionSet(0, 0, 0, 0, 0)
		)
	);

	massPerPhi.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"gas::"+thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,
			dimensionSet(1,0,-1,0,0)
		)
	);
}
