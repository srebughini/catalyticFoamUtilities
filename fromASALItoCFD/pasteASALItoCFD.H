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
				if ( specieFluid[j] == true )
				{
					if ( ResultsAsali == "mass" )
					{
						if (isNeighbourCell[celli] > 0)
						{
							for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
							{
								YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = omegaWALLasali[i][k];
							}
						}
						else
						{
							for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
							{
								YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = omegaBULKasali[i][k];
							}
						}
					}
					else if ( ResultsAsali == "mole" )
					{
						if (isNeighbourCell[celli] > 0)
						{
							double dummy;
							for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
							{
								moleFractions[k+1] = omegaWALLasali[i][k];
							}

							thermodynamicsMapXML->MassFractions_From_MoleFractions(massFractions,dummy,moleFractions);

							for(unsigned int k=0;k<thermodynamicsMapXML->NumberOfSpecies();k++)
							{
								YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+k].internalField()[celli] = massFractions[k+1];
							}
						}
						else
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
					}
				}

				if ( siteFluid[j] == true )
				{
					if (isNeighbourCell[celli] > 0)
					{
						const unsigned int iPatch = catalyticReactors[cataliticCounter].patchIndex[1];
						const unsigned int iFace  = catalyticReactors[cataliticCounter].faceIndex[1];
						for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
						{
							thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+k].boundaryField()[iPatch][iFace] = SITEasali[i][k];
						}
					}
				}

				if ( energyFluid[j] == true )
				{
					if (isNeighbourCell[celli] > 0)
					{
						T.internalField()[celli] = TWALLasali[i];
					}
					else
					{
						T.internalField()[celli] = TBULKasali[i];
					}
				}

				for (unsigned int k=0;k<3;k++)
				{
					if (isNeighbourCell[celli] > 0)
					{
						U.internalField()[celli][k] = 0.;
					}
					else
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
				}

				break;
			}
		}
		
		
		if (isNeighbourCell[celli] > 0)
		{
			cataliticCounter++;
		}

	}

	if ( energyFluid[j] == true )
	{
		T.write();
	}

	U.write();

	if ( specieFluid[j] == true )
	{
		for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		{
			YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
		}
	}

	if ( siteFluid[j] == true )
	{
		for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
		{
			thetaFluid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].write();
		}
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
				if ( specieSolid[j] == true )
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

					for(unsigned int k=0;k<surfaceThermodynamicsMapXML->number_of_site_species();k++)
					{
						thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+k].internalField()[celli] = SITEasali[i][k];
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

	if ( specieSolid[j] == true )
	{
		for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		{
			YSolid[j*thermodynamicsMapXML->NumberOfSpecies()+i].write();
		}

		for(unsigned int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
		{
			thetaSolid[j*surfaceThermodynamicsMapXML->number_of_site_species()+i].write();
		}
	}

	Info << solidRegions[j].name() << nl << endl;
}
