volScalarField T
(
	IOobject
	(
		"gas::T",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	thermo.T()
);

volScalarField h
(
	IOobject
	(
		"gas::h",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	thermo.he()
);

volScalarField cp
(
	IOobject
	(
		"gas::cp",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	thermo.he()/(thermo.rho()*thermo.T())
);

volScalarField cuph
(
	IOobject
	(
		"cup::h",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	mesh,
	dimensionSet(0, 3, -3, 0, 0)
);

volScalarField cuph_over_T
(
	IOobject
	(
		"cup::h_over_T",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	mesh,
	dimensionSet(0, 3, -3, 0, -1)
);

volScalarField cupT
(
	IOobject
	(
		"cup::T",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	mesh,
	dimensionSet(0, 1, -1, 0, -1)
);

volScalarField magU
(
	IOobject
	(
		"magU",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	mesh
);

volScalarField K
(
	IOobject
	(
		"gas::k",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	0.5*magSqr(U)
);
