PtrList<surfaceScalarField> phiFluid(fluidRegions.size());

//- Creating density [kg/m3]
forAll(fluidRegions, j)
{
	Info<< "Creating field rho\n" << endl;
	rhoFluid.set
	(   j,
		new volScalarField
		(
			IOobject
			(
				"rho",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			Rho(pFluid[j],TFluid[j],MWmixFluid[j])
		)
	);

	Info<< "Reading/calculating face flux field phi\n" << endl;
	phiFluid.set
	(   j,
		new surfaceScalarField
		(
			IOobject
			(
				"phi",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::READ_IF_PRESENT,
				IOobject::AUTO_WRITE
			),
		linearInterpolate(rhoFluid[j]*UFluid[j]) & fluidRegions[j].Sf()
		)
	);

}
		
multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
forAll(Y, j)
{
	fields.add(Y[j]);
}
