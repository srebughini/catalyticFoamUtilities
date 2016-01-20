Info << endl;

forAll(fluidRegions, j)
{
	Info << "ASALI --> CFD for region: ";
	#include "setRegionFluidFields.H"

	OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

	const volVectorField& centers          = mesh.C();
	unsigned int          NS               = centers.size();
	         int          cataliticCounter = 0;

	std::vector<double> dzVector(Nasali);
	double min;
	forAll(T.internalField(), celli)
	{
		for (unsigned int i=0;i<Nasali;i++)
		{
			dzVector[i] = std::fabs(centers[celli][directionIndex] - Zasali[i]);
		}

		min = dzVector[0];
		for (unsigned int i=1;i<Nasali;i++)
		{
			min = std::min(dzVector[i],min);
		}

		for (unsigned int i=0;i<Nasali;i++)
		{
			if ( std::fabs(centers[celli][directionIndex] - Zasali[i]) == min)
			{
				if ( ResultsAsali == "mass" )
				{
					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = omegaBULKasali[i][k];
					}
				}
				else if ( ResultsAsali == "mole" )
				{
					double dummy;
					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						moleFractions[k+1] = omegaBULKasali[i][k];
					}

					thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = massFractions[k+1];
					}
				}

				if ( energyFluid[j] == true )
				{
					T.internalField()[celli] = TBULKasali[i];
				}

				for (unsigned int k=0;k<3;k++)
				{
					if ( k == directionIndex)
					{
						U.internalField()[celli][k] = Vasali[i];
					}
					else
					{
						U.internalField()[celli][k] = 0.;
					}
				}

				break;
			}
		}
	}

	if ( energyFluid[j] == true )
	{
		T.write();
	}

	U.write();

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	{
		YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
	}

	Info << fluidRegions[j].name() << nl << endl;
}


forAll(solidRegions, j)
{
	Info << "ASALI --> CFD for region: ";
	#include "setRegionSolidFields.H"

	OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
    OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

	const volVectorField& centers = mesh.C();
	unsigned int          NS      = centers.size();

	std::vector<double> dzVector(Nasali);
	double min;
	unsigned int counter = 1;
	forAll(T.internalField(), celli)
	{
		for (unsigned int i=0;i<Nasali;i++)
		{
			dzVector[i] = std::fabs(centers[celli][directionIndex] - Zasali[i]);
		}

		min = dzVector[0];
		for (unsigned int i=1;i<Nasali;i++)
		{
			min = std::min(dzVector[i],min);
		}

		for (unsigned int i=0;i<Nasali;i++)
		{
			if ( std::fabs(centers[celli][directionIndex] - Zasali[i]) == min)
			{
				if ( ResultsAsali == "mass" )
				{
					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = omegaWALLasali[i][k];
					}
				}
				else if ( ResultsAsali == "mole" )
				{
					double dummy;
					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						moleFractions[k+1] = omegaWALLasali[i][k];
					}

					thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

					for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
					{
						YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = massFractions[k+1];
					}
				}

				if ( coverageSolid[j] == true )
				{
					for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
					{
						ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+k].internalField()[celli] = SITEasali[i][k];
					}
				}

				if ( energySolid[j] == true )
				{
					T.internalField()[celli] = TWALLasali[i];
				}

				break;
			}
		}
	}

	if ( energySolid[j] == true )
	{
		T.write();
	}

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	{
		YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
	}

	if ( coverageSolid[j] == true )
	{
		for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
		{
			ZSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].write();
		}
	}

	Info << solidRegions[j].name() << nl << endl;
}
