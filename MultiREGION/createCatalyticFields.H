//- Reading adsorbed species
Info << endl;	
Info << " * Reading Surface Species:"<<endl;
PtrList<volScalarField> siteFraction(surfaceThermodynamicsMapXML->number_of_site_species());

for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
{
	std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
	Info << "   Reading adsorbed specie: " << name << " > ";

	IOobject header
	(
		name,
		fluidRegions[j].time().timeName(),
		fluidRegions[j],
		IOobject::NO_READ
	);

	// check if field exists and can be read
	if (header.headerOk())
	{
		Info << name << " found at time " << fluidRegions[j].time().timeName() << endl;
			
		siteFraction.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					name,
					fluidRegions[j].time().timeName(),
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
		volScalarField thetaDefault
		(
			IOobject
			(
				"thetaDefault",
				fluidRegions[j].time().timeName(),
				fluidRegions[j],
				IOobject::MUST_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j]
		);

		siteFraction.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					name,
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				),
				thetaDefault
			)
		);

		if(i == 0)
		{
			forAll(siteFraction[i].boundaryField(), patchi)
			{
				if (Y[i].boundaryField()[patchi].type() == "catalyticWall")
				{
					forAll(siteFraction[i].boundaryField()[patchi], facei)
						siteFraction[i].boundaryField()[patchi][facei] = 1.;
				}
				else
				{
					forAll(siteFraction[i].boundaryField()[patchi], facei)
						siteFraction[i].boundaryField()[patchi][facei] = 0.;
				}
			}

			Info << "Catalyst's material " <<endl;
		}
		else
		{
			Info << "specie not adsorbed" << endl;
		}
	}
}
