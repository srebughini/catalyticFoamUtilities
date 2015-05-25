double Nasali;

std::vector<double> Zasali;
std::vector<double> TBULKasali;
std::vector<double> TWALLasali;
std::vector<double> Vasali;

std::vector< std::vector<double> > omegaBULKasali;
std::vector< std::vector<double> > omegaWALLasali;
std::vector< std::vector<double> > SITEasali;

std::string         ResultsAsali;

{
	std::string latestGridpath;
	std::string bulkSpeciespath;
	std::string adsorbedSpeciespath;
	std::string gridpath;
	std::string informationpath;
	std::string temperaturepath;
	std::string wallSpeciespath;
	std::string velocitypath;

	std::string latestGrid_;
	
	std::vector<double> omegaBulk;
	std::vector<double> omegaWall;
	std::vector<double> teta;

	for (unsigned int i=1;i<=1e05;i++)
	{
		std::string path = asaliFolder + "/" + to_string(i);

		if (boost::filesystem::exists(path))
		{
			if (boost::filesystem::is_directory(path))  
			{
				latestGrid_         = to_string(i);
				bulkSpeciespath     = path + "/bulkSpecies.txt";
				adsorbedSpeciespath = path + "/adsorbedSpecies.txt";
				gridpath            = path + "/grid.txt";
				informationpath     = path + "/information.txt";
				temperaturepath     = path + "/temperature.txt";
				wallSpeciespath     = path + "/wallSpecies.txt";
				velocitypath        = path + "/velocity.txt";
			}
		}
	}

	std::string dummyString;
	std::vector<double> dummyVector;
	double dummyDouble;

	if (boost::filesystem::exists(gridpath))
	{
		const char *pathGrid = gridpath.c_str();
		std::ifstream is(pathGrid);
		unsigned int k=0;
		while (getline(is,dummyString))
		{
			dummyVector.resize(k+1);
			dummyVector[k] = boost::lexical_cast<double>(dummyString);
			k++;
		}

		Nasali = dummyVector.size();
		Zasali.resize(Nasali);

		for (unsigned int k=0;k<Zasali.size();k++)
		{
			Zasali[k] = dummyVector[k];
		}
	}
	else
	{
		std::cout << "grid.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}

	if (boost::filesystem::exists(temperaturepath))
	{
		const char *pathTemperature = temperaturepath.c_str();
		std::ifstream is(pathTemperature);

		dummyVector.resize(Nasali*2);
		for (unsigned int k=0;k<dummyVector.size();k++)
			dummyVector[k] = 0.;

		unsigned int k=0;
		while (getline(is, dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyDouble)
			{
				dummyVector[k] = dummyDouble;
				k++;
			}
		}

		TBULKasali.resize(Nasali);
		TWALLasali.resize(Nasali);
		unsigned int counter=0;
		for (unsigned int k=0;k<Nasali;k++)
		{
			TBULKasali[k]  = std::max(0.,dummyVector[counter]);
			TWALLasali[k]  = std::max(0.,dummyVector[counter + 1]);
			counter = counter + 2;
		}
	}
	else
	{
		std::cout << "temperature.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}


	if (boost::filesystem::exists(bulkSpeciespath))
	{
		const char *pathBulk = bulkSpeciespath.c_str();
		std::ifstream is(pathBulk);

		unsigned int k=0;
		while (getline(is, dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyDouble)
			{
				omegaBulk.resize(k+1);
				omegaBulk[k] = std::max(0.,dummyDouble);
				k++;
			}
		}
	}
	else
	{
		std::cout << "bulkSpecies.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}

	if (boost::filesystem::exists(wallSpeciespath))
	{
		const char *pathWall = wallSpeciespath.c_str();
		std::ifstream is(pathWall);

		unsigned int k=0;
		while (getline(is, dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyDouble)
			{
				omegaWall.resize(k+1);
				omegaWall[k] = std::max(0.,dummyDouble);
				k++;
			}
		}
	}
	else
	{
		std::cout << "wallSpecies.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}


	if (boost::filesystem::exists(adsorbedSpeciespath))
	{
		const char *pathSite = adsorbedSpeciespath.c_str();
		std::ifstream is(pathSite);

		unsigned int k=0;
		while (getline(is, dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyDouble)
			{
				teta.resize(k+1);
				teta[k] = std::max(0.,dummyDouble);
				k++;
			}
		}
	}
	else
	{
		std::cout << "adsorbedSpecies.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}

	if (boost::filesystem::exists(velocitypath))
	{
		const char *pathVel = velocitypath.c_str(); 
		std::ifstream is(pathVel);

		unsigned int k=0;
		while (getline(is, dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyDouble)
			{
				Vasali.resize(k+1);
				Vasali[k] = std::max(0.,dummyDouble);
				k++;
			}
		}
	}
	else
	{
		std::cout << "velocity.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}

	if (boost::filesystem::exists(informationpath))
	{
		const char *pathInfo = informationpath.c_str();
		std::ifstream is(pathInfo);
		while (getline(is,dummyString))
		{
			std::istringstream iss(dummyString);
			while (iss >> dummyString)
			{
				ResultsAsali = dummyString;
				if ( ResultsAsali == "mole" || ResultsAsali == "mass")
					break;
			}
			if ( ResultsAsali == "mole" || ResultsAsali == "mass")
				break;
		}
	}
	else
	{
		std::cout << "information.txt file NOT exist" << std::endl;
		exit (EXIT_FAILURE);
	}


	{
		omegaBULKasali.resize(Nasali);
		omegaWALLasali.resize(Nasali);

		unsigned int counter = 0;
		for (unsigned int i=0;i<Nasali;i++)
		{
			omegaBULKasali[i].resize(thermodynamicsMapXML->NumberOfSpecies());
			omegaWALLasali[i].resize(thermodynamicsMapXML->NumberOfSpecies());
			for (unsigned int j=0;j<thermodynamicsMapXML->NumberOfSpecies();j++)
			{
				omegaBULKasali[i][j] = omegaBulk[counter];
				omegaWALLasali[i][j] = omegaWall[counter];
				counter++;
			}
		}
	}


	{
		SITEasali.resize(Nasali);
		unsigned int counter = 0;
		for (unsigned int i=0;i<Nasali;i++)
		{
			SITEasali[i].resize(surfaceThermodynamicsMapXML->number_of_site_species());
			for (unsigned int j=0;j<surfaceThermodynamicsMapXML->number_of_site_species();j++)
			{
				SITEasali[i][j] = teta[counter++];
			}
		}
	}
}
