// Initialise fluid field pointer lists
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<volScalarField> pFluid(fluidRegions.size());
PtrList<volScalarField> TFluid(fluidRegions.size());
PtrList<volScalarField> cTotFluid(fluidRegions.size());
PtrList<volScalarField> MWmixFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());
PtrList<fv::IOoptionList> fluidFvOptions(fluidRegions.size());

// Populate fluid field pointer lists
forAll(fluidRegions, j)
{
	Info<< "*** Reading and allocating fluid mesh thermophysical properties for region "
		<< fluidRegions[j].name() << nl << endl;

	Info<< "    Reading field U\n" << endl;
	UFluid.set   // speed [m/s]
	(
		j,
		new volVectorField
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
		)
	);
        
	Info<< "    Reading field p\n" << endl;
	pFluid.set   // pressure [Pa]
	(
		j,
		new volScalarField
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
		)
	);
        
	Info<< "    Reading field T\n" << endl;
	TFluid.set   // temperature [K]
	(
		j,
		new volScalarField
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
		)
	); 

	Info<< "    Creating field cTot\n" << endl;
	cTotFluid.set   // total concentration [mol/m3]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"cTot",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, -3, 0, 0, 1)
		)
	);    

	Info<< "    Creating field MWmix\n" << endl;
	MWmixFluid.set   // mean molecular weight [kg/mol]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"meanMolecularWeight",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, 0, 0, 0, -1)
		)
	);

	Info<< "    Adding fvOptions\n" << endl;
	fluidFvOptions.set
	(
		j,
		new fv::IOoptionList(fluidRegions[j])
	);
}

Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());

forAll (fluidRegions,j)
{   
	Info<< "*** Reading and allocating composition and diffusion properties for region "
            << fluidRegions[j].name() << nl << endl;

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	{

		Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

		IOobject header
		(
			thermodynamicsMapXML->NamesOfSpecies()[i],
			runTime.timeName(),
			fluidRegions[j],
			IOobject::NO_READ
		);
		
		// check if field exists and can be read
		if (header.headerOk())
		{
			Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
			Y.set
			(
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						thermodynamicsMapXML->NamesOfSpecies()[i],
						runTime.timeName(),
						fluidRegions[j],
						IOobject::MUST_READ,
						IOobject::AUTO_WRITE
					),
					fluidRegions[j]
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

