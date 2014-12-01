PtrList<surfaceScalarField> J(thermodynamicsMapXML->NumberOfSpecies());

forAll (Y,i)
{
	J.set
	(
		i,
		new surfaceScalarField
		(
			IOobject
			(
				"J::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			mesh,
			dimensionSet(1, 0, -1, 0, 0)
		)
	);
}

surfaceScalarField Jc
(
    IOobject
    (
	"fluxes::Jc",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
	IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("J::Jc", dimensionSet(1, 0, -1, 0, 0), 0.0)
);
