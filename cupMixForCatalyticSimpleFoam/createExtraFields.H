Info<< "Creating field phix\n" << endl;
volScalarField phix
(
    IOobject
    (
        "gas::phix",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionSet(1,0,-1,0,0)
);

Info<< "Creating field phiy\n" << endl;
volScalarField phiy
(
    IOobject
    (
        "gas::phiy",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionSet(1,0,-1,0,0)
);

Info<< "Creating field phiz\n" << endl;
volScalarField phiz
(
    IOobject
    (
        "gas::phiz",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionSet(1,0,-1,0,0)
);

if (thermodynamicsMapXML->NumberOfSpecies() != Y.size())
	FatalError << "Kinetic schemes do not match..." << endl;

for (unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{
	if (Y[i].name() != thermodynamicsMapXML->NamesOfSpecies()[i])
	{
		FatalError << "Kinetic schemes do not match..." << endl;
	}
}


PtrList<volScalarField> x_;
PtrList<volScalarField> massPerPhi_;
x_.resize(Y.size());
massPerPhi_.resize(Y.size());

forAll(Y,i)
{
	x_.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"mole_"+ Y[i].name(),
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,
			dimensionedScalar("zero", dimensionSet(0, 0, 0, 0, 0), 0.)
		)
	);

	massPerPhi_.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"gas::"+ Y[i].name(),
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,
			dimensionedScalar("zero", dimensionSet(1,0,-1,0,0), 0.)
		)
	);
}
