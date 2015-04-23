{
	ChangeDimensions (totalNumberOfSolidInterfaceFacesAdjacent,&TAbsResidual,true);
	ChangeDimensions (totalNumberOfSolidInterfaceFacesAdjacent,&TRelResidual,true);
	ChangeDimensions (totalNumberOfSolidInterfaceFacesAdjacent,thermodynamicsMapXML->NumberOfSpecies(),&YAbsResidual, true);
	ChangeDimensions (totalNumberOfSolidInterfaceFacesAdjacent,thermodynamicsMapXML->NumberOfSpecies(),&YRelResidual, true);
	
	TAbsResidual = 0.;
	TRelResidual = 0.;
	YAbsResidual = 0.;
	YRelResidual = 0.;
	
	if (energyEquation == true)
	{		
		for (int i=1;i<=totalNumberOfSolidInterfaceFacesAdjacent;i++)
		{
			TAbsResidual[i] = abs(Ts1_Adjacent[i]-Ts2_Adjacent[i]); 
			TRelResidual[i] = abs(Ts1_Adjacent[i]-Ts2_Adjacent[i])/Ts1_Adjacent[i];
		}
		maxAbsResidualT = TAbsResidual.Max();
		maxRelResidualT = TRelResidual.Max();
	}
	

	// TODO 
	if (solidSpeciesEquations[0] == true && solidSpeciesEquations[1]== true)
	{
		for (int i = 1; i<=totalNumberOfSolidInterfaceFacesAdjacent; i++)
		{	
			for (unsigned int z=0; z<thermodynamicsMapXML->NumberOfSpecies(); z++)
			{
				YAbsResidual[i][z+1] = abs(Ys1_Adjacent[i][z+1] - Ys2_Adjacent[i][z+1]);
				if (Ys1_Adjacent[i][z+1] != 0 && (abs(Ys1_Adjacent[i][z+1]+0.5*(Ys2_Adjacent[i][z+1]-Ys1_Adjacent[i][z+1])) > 1e-8))
				{
					YRelResidual[i][z+1] = abs(Ys1_Adjacent[i][z+1]-Ys2_Adjacent[i][z+1])/Ys1_Adjacent[i][z+1];
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
	
