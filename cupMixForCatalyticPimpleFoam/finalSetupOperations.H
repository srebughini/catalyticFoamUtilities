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
	Rho(p,T)
);

#include "compressibleCreatePhi.H"

multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
forAll(Y, i)
{
	fields.add(Y[i]);
}

