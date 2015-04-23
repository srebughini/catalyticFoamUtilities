
{ 	
	Info << "  " << endl;

	if (maxAbsResidualT > absTolT) 
		Info << " Warning: maxAbsResidualT > absTolT" << endl;
	
	if (maxRelResidualT > relTolT) 
		Info << " Warning: maxRelResidualT > relTolT" << endl;        

	if (maxAbsResidualY > absTolY)
		Info << " Warning: maxAbsResidualY > absTolY" << endl;
		
	if (maxRelResidualY > relTolY)
		Info << " Warning: maxRelResidualT > relTolT" << endl;	
			     
	Info  << "-------------------------------Residual Check Transport-------------------------------" << endl; 
	
	if ((maxAbsResidualT < absTolT) && (maxAbsResidualY < absTolY) && (maxRelResidualT < relTolT) && (maxRelResidualY < relTolY))
	 {
		Info << " " << endl;
		Info << " Time = " << runTime.timeName() << endl;
		Info << " maxAbsResidualT = " << maxAbsResidualT << endl;
		Info << " maxRelResidualT = " << maxRelResidualT << endl;
		Info << " maxAbsResidualY = " << maxAbsResidualY << endl;
		Info << " maxRelResidualY = " << maxRelResidualY << endl;
		Info << " Convergerce at adjacent fluid interface reached at iteration " << pimpleCorr <<endl;
		finalTransportAdjacentFluid = true;
	 }
	 
	else
	 {
		if (pimpleCorr == maxPimpleCorrSerial)
		{
			Info << "Time = " << runTime.timeName() << ": Convergerce at adjacent fluid interface not reached in " << maxPimpleCorrSerial << " iterations" << endl;
			finalTransportAdjacentFluid= true;					
		}
		else
		{
			Info << " " << endl;
			Info << " Time = " << runTime.timeName() << endl;
			Info << " maxAbsResidualT = " << maxAbsResidualT << endl;
			Info << " maxRelResidualT = " << maxRelResidualT << endl;
			Info << " maxAbsResidualY = " << maxAbsResidualY << endl;
			Info << " maxRelResidualY = " << maxRelResidualY << endl;
			Info << " Convergerce at adjacent fluid interface not reached at iteration "<< pimpleCorr << endl << "Let's start again =P" << endl ;
			finalTransportAdjacentFluid = false;	
		}
	 }
	 Info  << "------------------------------------------------------------------------------------" << endl; 
}
