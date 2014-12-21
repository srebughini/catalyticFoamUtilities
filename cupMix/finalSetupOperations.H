//- Creating density [kg/m3]
Info<< "Creating field rho\n" << endl;
volScalarField rho
(
	IOobject
	(
		"rho",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	Rho(p,psi)
);

#include "compressibleCreatePhi.H"

Info<< "Creating field DpDt\n" << endl;
volScalarField DpDt
(
	IOobject
	(
		"DpDt",
		runTime.timeName(),
		mesh
	),
	mesh,
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

