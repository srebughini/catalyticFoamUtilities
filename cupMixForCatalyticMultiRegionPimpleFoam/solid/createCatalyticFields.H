PtrList<volScalarField> siteFraction(surfaceThermodynamicsMapXML->number_of_site_species()*solidRegions.size());

forAll(solidRegions, j)
{
	//- Reading adsorbed species
	Info << endl;	
	Info << " * Reading Surface Species for region "<< solidRegions[j].name() << nl << endl;

	for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
	{
		std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
		Info << "   Reading adsorbed specie: " << name << " > ";
	
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
			Info << name << " found at time " << runTime.timeName() << endl;
				
			siteFraction.set
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

			siteFraction.set
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
			
			if(i == 0)
			{
				forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField(), celli)
				{
					siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].internalField()[celli] = 1.;
				}
				forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField(), patchi)
			    	{	
					forAll(siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi], facei)
					{	
						siteFraction[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].boundaryField()[patchi][facei] = 1.;
					}
				}
				
				Info << "catalyst's material "<< name <<endl;
			}
			else
			{
				Info << "species not adsorbed" << endl;
			}
		}
	}
}
