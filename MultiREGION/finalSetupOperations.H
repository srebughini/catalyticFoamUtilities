//- Creating density [kg/m3]
Info<< "Creating field rho\n" << endl;
volScalarField rho
(
	IOobject
	(
		"rho",
		runTime.timeName(),
		fluidRegions[j],
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	Rho(p,psi)
);

Info<< "Reading/calculating face flux field phi\n" << endl;
surfaceScalarField phi
(
	IOobject
	(
		"phi",
		runTime.timeName(),
		fluidRegions[j],
		IOobject::READ_IF_PRESENT,
		IOobject::AUTO_WRITE
	),
	linearInterpolate(rho*U) & fluidRegions[j].Sf()
);

Info<< "Creating field DpDt\n" << endl;
volScalarField DpDt
(
	IOobject
	(
		"DpDt",
		runTime.timeName(),
		fluidRegions[j]
	),
	fluidRegions[j],
	dimensionedScalar("DpDt", p.dimensions()/dimTime, 0)
);

// Kinetic energy
Info<< "Creating field kinetic energy K\n" << endl;
volScalarField K("K", 0.5*magSqr(U));

multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
forAll(Y, i)
{
	fields.add(Y[i]);
}

