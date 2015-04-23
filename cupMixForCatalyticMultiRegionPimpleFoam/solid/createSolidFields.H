// Initialise solid field pointer lists
PtrList<volScalarField> TSolid(solidRegions.size());
    
// Populate solid field pointer lists
forAll(solidRegions, j)
{
	Info<< "*** Reading and allocating solid mesh thermophysical properties for region "
		<< solidRegions[j].name() << nl << endl;   

	Info<< "    Reading field T\n" << endl;
	TSolid.set   // temperature [K]
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
}

Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> YSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());

forAll (solidRegions,j)
{   
	Info<< "*** Reading and allocating composition and diffusion properties for region "
            << solidRegions[j].name() << nl << endl;

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	{
		Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

		IOobject header
		(
			thermodynamicsMapXML->NamesOfSpecies()[i],
			runTime.timeName(),
			solidRegions[j],
			IOobject::NO_READ
		);
		
		// check if field exists and can be read
		if (header.headerOk())
		{
			Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
			YSolid.set
			(
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						thermodynamicsMapXML->NamesOfSpecies()[i],
						runTime.timeName(),
						solidRegions[j],
						IOobject::MUST_READ,
						IOobject::AUTO_WRITE
					),
					solidRegions[j]
				)
			);
		}
		else
		{
			Info << "Species  " << thermodynamicsMapXML->NamesOfSpecies()[i] << 
			"not present at time  " << runTime.timeName() << endl;
			abort();
		}

	}
}
