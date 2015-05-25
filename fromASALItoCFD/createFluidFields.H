PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<volScalarField> TFluid(fluidRegions.size());

forAll(fluidRegions, j)
{
	UFluid.set
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

	if ( energyFluid[j] == true )
	{
		TFluid.set
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
	}

}

PtrList<volScalarField> YFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> thetaFluid(surfaceThermodynamicsMapXML->number_of_site_species()*fluidRegions.size());

forAll (fluidRegions,j)
{
	if ( specieFluid[j] == true )
	{
		for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		{
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
				YFluid.set
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
				volScalarField Ydefault
				(
					IOobject
					(
						"Ydefault",
						runTime.timeName(),
						fluidRegions[j],
						IOobject::MUST_READ,
						IOobject::NO_WRITE
					),
					fluidRegions[j]
				);

				YFluid.set
				(
					j*thermodynamicsMapXML->NumberOfSpecies()+i,
					new volScalarField
					(
						IOobject
						(
							thermodynamicsMapXML->NamesOfSpecies()[i],
							runTime.timeName(),
							fluidRegions[j],
							IOobject::NO_READ,
							IOobject::AUTO_WRITE
						),
						Ydefault
					)
				);
			}
		}

		//- Reading adsorbed species
		for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
		{
			std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
			IOobject header
			(
				name,
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ
			);

			// check if field exists and can be read
			if (header.headerOk())
			{
				thetaFluid.set
				(
					j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
					new volScalarField
					(
						IOobject
						(
							name,
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
				volScalarField thetaDefault
				(
					IOobject
					(
						"thetaDefault",
						runTime.timeName(),
						fluidRegions[j],
						IOobject::MUST_READ,
						IOobject::NO_WRITE
					),
					fluidRegions[j]
				);

				thetaFluid.set
				(
					j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
					new volScalarField
					(
						IOobject
						(
							name,
							runTime.timeName(),
							fluidRegions[j],
							IOobject::NO_READ,
							IOobject::AUTO_WRITE
						),
						thetaDefault
					)
				);
				
				if( i == catalystIndex)
				{
					forAll(thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField(), celli)
					{
						thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField()[celli] = 1.;
					}

					forAll(thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField(), patchi)
					{
						forAll(thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi], facei)
						{    
							thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi][facei] = 1.;
						}
					}
				}

			}
		}
	}
}
