tmp<volScalarField> Rho(volScalarField& p, volScalarField& T)
{
	return p/(8314*T);
}

void normalizeMassFractions(Eigen::VectorXd& omega_plus_temperature, const label celli, const double massFractionsTol)
{
    unsigned int failures = 0;

    double sumFractions = 0.;
    for(int i=0; i < omega_plus_temperature.size()-1; i++)
            sumFractions += omega_plus_temperature(i);

    if (sumFractions > 1.+massFractionsTol || sumFractions < 1.-massFractionsTol)
    {
        failures++;
      
        if (time == 0)
        {
		Info << "\nFATAL ERROR: sum of mass fractions is not 1" << endl;
		abort();
        }    
    }
	
    for(int i=0; i < omega_plus_temperature.size()-1; i++)
	omega_plus_temperature(i) /= sumFractions;

    if (failures != 0)
    	Info << "WARNING: error in sum of mass fractions in " << failures << " homogeneous cell(s)!" << endl;
}

void normalizeMassAndSiteFractions(	Eigen::VectorXd& yf, const label celli, 
					const unsigned int NC, const unsigned int SURF_NP, const unsigned int SURF_NC, 
					const double massFractionsTol, const double siteFractionsTol)
{
	unsigned int failures = 0;

	// Gas phase
	{
		double sum = 0.;
		for(unsigned int i=0;i<NC;i++)
			sum += yf(i);

		if (sum > (1.+massFractionsTol) || sum < (1.-massFractionsTol) )
		{
			failures++;
			
			if (time == 0)
			{
				Info << "\nFATAL ERROR: sum of mass fractions is not 1" << endl;
				abort();
			}    
		}
		
		for(unsigned int i=0;i<NC;i++)
			yf(i) /= sum;
	}
	
	// Surface site species
	{
		const unsigned int count = NC+1+SURF_NP;
		
		double sum = 0.;
		for(unsigned int i=0;i<SURF_NC;i++)
			sum += yf(count+i);

		if (sum > (1.+siteFractionsTol) || sum < (1.-siteFractionsTol) )
		{
			failures++;
			
			if (time == 0)
			{
				Info << "\nFATAL ERROR: sum of site fractions is not 1" << endl;
				abort();
			}    
		}
		
		for(unsigned int i=0;i<SURF_NC;i++)
			yf(count+i) /= sum;
	}

	if (failures != 0)
    		Info << "WARNING: error in sum of mass fractions in " << failures << " heterogeneous cell(s)!" << endl;
}

void normalizeMassAndSiteFractions(	OpenSMOKE::OpenSMOKEVectorDouble& yf, const label celli, 
					const unsigned int NC, const unsigned int SURF_NP, const unsigned int SURF_NC, 
					const double massFractionsTol, const double siteFractionsTol)
{
	unsigned int failures = 0;

	// Gas phase
	{
		double sum = 0.;
		for(unsigned int i=1;i<=NC;i++)
			sum += yf[i];

		if (sum > (1.+massFractionsTol) || sum < (1.-massFractionsTol) )
		{
			failures++;
			
			if (time == 0)
			{
				Info << "\nFATAL ERROR: sum of mass fractions is not 1" << endl;
				Info << "Check internal field on cell: " << celli << endl;
				abort();
			}    
		}
		
		for(unsigned int i=1;i<=NC;i++)
			yf[i] /= sum;
	}
	
	// Surface site species
	{
		const unsigned int count = NC+1+SURF_NP;
		
		double sum = 0.;
		for(unsigned int i=1;i<=SURF_NC;i++)
			sum += yf[count+i];

		if (sum > (1.+siteFractionsTol) || sum < (1.-siteFractionsTol) )
		{
			failures++;
			
			if (time == 0)
			{
				Info << "\nFATAL ERROR: sum of site fractions is not 1" << endl;
				Info << "Check internal field on cell: " << celli << endl;
				abort();
			}    
		}
		
		for(unsigned int i=1;i<=SURF_NC;i++)
			yf[count+i] /= sum;
	}

	if (failures != 0)
    		Info << "WARNING: error in sum of mass fractions in " << failures << " heterogeneous cell(s)!" << endl;
}

void normalizeMassFractions(OpenSMOKE::OpenSMOKEVectorDouble& omega_plus_temperature, const label celli, const double massFractionsTol)
{
    unsigned int failures = 0;

    double sumFractions = 0.;
    for(int i=1; i <= omega_plus_temperature.Size()-1; i++)
            sumFractions += omega_plus_temperature[i];

    if (sumFractions > 1.+massFractionsTol || sumFractions < 1.-massFractionsTol)
    {
        failures++;
        
        if (time == 0)
        {
				Info << "\nFATAL ERROR: sum of mass fractions is not 1" << endl;
				Info << "Check internal field on cell: " << celli << endl;
				abort();
        }    
    }
	
	 for(int i=1; i <= omega_plus_temperature.Size()-1; i++)
            omega_plus_temperature[i] /= sumFractions;

    if (failures != 0)
    	Info << "WARNING: error in sum of mass fractions in " << failures << " homogeneous cell(s)!" << endl;
}
