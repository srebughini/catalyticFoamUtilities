PtrList<volScalarField> TSolid(solidRegions.size());

forAll(solidRegions, j)
{
	if ( energySolid[j] == true )
	{
		TSolid.set
		(
			j,
			new volScalarField
			(
				IOobject
				(
					"T",
					runTime.timeName(),
					solidRegions[j],
					IOobject::MUST_READ,
					IOobject::AUTO_WRITE
				),
				solidRegions[j]
			)
		);
	}
}

PtrList<volScalarField> YSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField> thetaSolid(surfaceThermodynamicsMapXML->number_of_site_species()*solidRegions.size());
forAll (solidRegions,j)
{
	if ( specieSolid[j] == true )
	{
		for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		{
			IOobject header
			(
				thermodynamicsMapXML->NamesOfSpecies()[i],
				runTime.timeName(),
				solidRegions[j],
				IOobject::NO_READ
			);
			
			// check if field exists and can be read
			if (header.headerOk())
			{
				YSolid.set
				(
					j*thermodynamicsMapXML->NumberOfSpecies()+i,
					new volScalarField
					(
						IOobject
						(
							thermodynamicsMapXML->NamesOfSpecies()[i],
							runTime.timeName(),
							solidRegions[j],
							IOobject::MUST_READ,
							IOobject::AUTO_WRITE
						),
						solidRegions[j]
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
						solidRegions[j],
						IOobject::MUST_READ,
						IOobject::NO_WRITE
					),
					solidRegions[j]
				);

				YSolid.set
				(
					j*thermodynamicsMapXML->NumberOfSpecies()+i,
					new volScalarField
					(
						IOobject
						(
							thermodynamicsMapXML->NamesOfSpecies()[i],
							runTime.timeName(),
							solidRegions[j],
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
				solidRegions[j],
				IOobject::NO_READ
			);

			// check if field exists and can be read
			if (header.headerOk())
			{
				thetaSolid.set
				(
					j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
					new volScalarField
					(
						IOobject
						(
							name,
							runTime.timeName(),
							solidRegions[j],
							IOobject::MUST_READ,
							IOobject::AUTO_WRITE
						),
						solidRegions[j]
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
						solidRegions[j],
						IOobject::MUST_READ,
						IOobject::NO_WRITE
					),
					solidRegions[j]
				);

				thetaSolid.set
				(
					j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
					new volScalarField
					(
						IOobject
						(
							name,
							runTime.timeName(),
							solidRegions[j],
							IOobject::NO_READ,
							IOobject::AUTO_WRITE
						),
						thetaDefault
					)
				);
				
				if( i == catalystIndex)
				{
					forAll(thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField(), celli)
					{
						thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField()[celli] = 1.;
					}

					forAll(thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField(), patchi)
					{
						forAll(thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi], facei)
						{    
							thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi][facei] = 1.;
						}
					}
				}
			}
		}
	}
}
