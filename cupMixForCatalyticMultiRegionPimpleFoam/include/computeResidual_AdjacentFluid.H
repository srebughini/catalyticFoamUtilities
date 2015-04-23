{
	
	
	ChangeDimensions (totalNumberOfFluidInterfaceFacesAdjacent,&TAbsResidual,true);
	ChangeDimensions (totalNumberOfFluidInterfaceFacesAdjacent,&TRelResidual,true);
	ChangeDimensions (totalNumberOfFluidInterfaceFacesAdjacent,thermodynamicsMapXML->NumberOfSpecies(),&YAbsResidual, true);
	ChangeDimensions (totalNumberOfFluidInterfaceFacesAdjacent,thermodynamicsMapXML->NumberOfSpecies(),&YRelResidual, true);
	
	TAbsResidual = 0.;
	TRelResidual = 0.;
	YAbsResidual = 0.;
	YRelResidual = 0.;
	
	
	if (energyEquation == true)
	{	
		
		for (int i=1;i<=totalNumberOfFluidInterfaceFacesAdjacent;i++)
		{
			TAbsResidual[i] = abs(Tf1_Adjacent[i]-Tf2_Adjacent[i]); 
			TRelResidual[i] = abs(Tf1_Adjacent[i]-Tf2_Adjacent[i])/Tf1_Adjacent[i];
		}
		
		maxAbsResidualT = TAbsResidual.Max();
		maxRelResidualT = TRelResidual.Max();
		
	}
	
	// TODO
	if (fluidSpeciesEquations[0] == true && fluidSpeciesEquations[1]== true) 
	{
		for (int i = 1; i<=totalNumberOfFluidInterfaceFacesAdjacent; i++)
		{	
			for (unsigned int z=0; z<thermodynamicsMapXML->NumberOfSpecies(); z++)
			{
				YAbsResidual[i][z+1] = abs(Yf1_Adjacent[i][z+1] - Yf2_Adjacent[i][z+1]);
				if (Yf1_Adjacent[i][z+1] != 0 && (abs(Yf1_Adjacent[i][z+1]+0.5*(Yf2_Adjacent[i][z+1]-Yf1_Adjacent[i][z+1])) > 1e-8))
				{
					YRelResidual[i][z+1] = abs(Yf1_Adjacent[i][z+1]-Yf2_Adjacent[i][z+1])/Yf1_Adjacent[i][z+1];
				}
				else
				{
					YRelResidual[i][z+1] = 0;
				}
			}
		}
		maxAbsResidualY = MaxValue(YAbsResidual);
		maxRelResidualY = MaxValue(YRelResidual);
	}			
}
	
