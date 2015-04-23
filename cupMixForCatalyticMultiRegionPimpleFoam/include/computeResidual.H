{
	

	ChangeDimensions (totalNumberOfInterfaceFaces,&TAbsResidual,true);
	ChangeDimensions (totalNumberOfInterfaceFaces,&TRelResidual,true);
	ChangeDimensions (totalNumberOfInterfaceFaces,thermodynamicsMapXML->NumberOfSpecies(),&YAbsResidual, true);
	ChangeDimensions (totalNumberOfInterfaceFaces,thermodynamicsMapXML->NumberOfSpecies(),&YRelResidual, true);
	
	TAbsResidual = 0.;
	TRelResidual = 0.;
	YAbsResidual = 0.;
	YRelResidual = 0.;

	
	if (energyEquation == true)
	{
		for (int i=1;i<=totalNumberOfInterfaceFaces;i++)
		{
			TAbsResidual[i] = abs(Tf[i]-Ts[i]); 
			TRelResidual[i] = abs(Tf[i]-Ts[i])/Tf[i];
		}
		maxAbsResidualT = TAbsResidual.Max();
		maxRelResidualT = TRelResidual.Max();
	
	}
			
	for (int i = 1; i<=totalNumberOfInterfaceFaces; i++)
	{	
		for (unsigned int z=1; z<=thermodynamicsMapXML->NumberOfSpecies(); z++)
		{
			YAbsResidual[i][z] = abs(Yf[i][z] - Ys[i][z]);
			if (Yf[i][z] != 0 && (abs(Yf[i][z]+0.5*(Ys[i][z]-Yf[i][z])) > 1e-8))
			{
				YRelResidual[i][z] = abs(Yf[i][z]-Ys[i][z])/Yf[i][z];
			}
			else
			{
				YRelResidual[i][z] = 0;
			}
		}
	}
	maxAbsResidualY = MaxValue(YAbsResidual);
	maxRelResidualY = MaxValue(YRelResidual);			
	

	
}
