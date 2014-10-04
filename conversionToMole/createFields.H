PtrList<volScalarField> x(thermodynamicsMapXML->NumberOfSpecies());

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{
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
}
