//Write on terminal
std::cout << "---------------------------------------------------" << std::endl;
cout.setf(std::ios::scientific);
std::cout << "Inlet area 	= " <<  PatchInletArea << "	[m2]" << std::endl;
std::cout << "Oulet area 	= " <<  PatchOutletArea << "	[m2]" << std::endl;
std::cout << "Catalytic area 	= " <<  PatchCatArea << "	[m2]" << std::endl;
std::cout << "---------------------------------------------------" << std::endl;
std::cout << "Inlet velocity 	= " << Uinlet << "	[m/s]" << std::endl;
std::cout << "Mean velocity 	= " << Umean << "	[m/s]" << std::endl;
std::cout << "Max velocity 	= " << Umax << "	[m/s]" << std::endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "	IN mass fraction [-]" << endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
		std::cout << Y[index].name() << "\t" << MassFractionInlet[index+1] << std::endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "	OUT mass fraction [-]" << endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
		std::cout << Y[index].name() << "\t" << MassFractionOutlet[index+1] << std::endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "	Conversion [-]" << endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	if (ConversionVector[index+1] > 0.)
		std::cout << Y[index].name() << "\t" << ConversionVector[index+1] << std::endl;
}
std::cout << "---------------------------------------------------" << std::endl;
Info << "IN pressure 	= " << InletPressure << "	[Pa]" << endl;
Info << "OUT pressure 	= " << OutletPressure << "	[Pa]" << endl;
Info << "Pressure drops 	= " << PressureDrops/1.e05 << "	[bar]" << endl;
Info << "Mean pressure 	= " << pMean/1.e05 << "	[bar]" << endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "Mean density 	= " << rhoMean << "	[kg/m3]" << endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "Mean viscosity 	= " << muMean << "	[Pas]" << endl;
std::cout << "---------------------------------------------------" << std::endl;
Info << "	Mean diffusivity [m2/s]" << endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	std::cout << Y[index].name() << "\t" << DmixMean[index+1] << std::endl;
}
std::cout << "---------------------------------------------------" << std::endl;
Info << "	Atomic balance error [-]" << endl;
for (int index=0;index<elementNames_.size();index++)
{
	std::cout << elementNames_[index] << "\t" << (ElementInlet[index+1] - ElementOutlet[index+1])*100/ElementInlet[index+1]  << "\t[%]" << std::endl;
}
std::cout << "---------------------------------------------------" << std::endl;

// Write on file
std::ofstream fmassFlux;
std::string fileName = "postProcessorSIMPLE_"+runTime.timeName()+".txt";
const char *pathFILE = fileName.c_str();
fmassFlux.open(pathFILE, std::ios::out);
fmassFlux.setf(std::ios::scientific);
fmassFlux.precision(12);
fmassFlux << "Inlet area 	= " <<  PatchInletArea << "	[m2]" << std::endl;
fmassFlux << "Oulet area 	= " <<  PatchOutletArea << "	[m2]" << std::endl;
fmassFlux << "Catalytic area 	= " <<  PatchCatArea << "	[m2]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Inlet velocity 	= " << Uinlet << "	[m/s]" << std::endl;
fmassFlux << "Mean velocity 	= " << Umean << "	[m/s]" << std::endl;
fmassFlux << "Max velocity 	= " << Umax << "	[m/s]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "	IN mass fraction [-]" << std::endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
		fmassFlux << Y[index].name() << "\t" << MassFractionInlet[index+1] << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "	OUT mass fraction [-]" << std::endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
		fmassFlux << Y[index].name() << "\t" << MassFractionOutlet[index+1] << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "	Conversion [-]" << std::endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	if (ConversionVector[index+1] > 0.)
		fmassFlux << Y[index].name() << "\t" << ConversionVector[index+1] << std::endl;
}
fmassFlux << " " << std::endl;
fmassFlux << "Pressure drops 	= " <<  PressureDrops/1.e05 << "	[bar]" << std::endl;
fmassFlux << "IN pressure 	= " << InletPressure << "	[Pa]" << std::endl;
fmassFlux << "OUT pressure 	= " << OutletPressure << "	[Pa]" << std::endl;
fmassFlux << "Mean pressure 	= " <<  pMean/1.e05 << "	[bar]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean density 	= " << rhoMean << "	[kg/m3]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean viscosity 	= " << muMean << "	[Pas]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "	Mean diffusivity [m2/s]" << std::endl;
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	fmassFlux << Y[index].name() << "\t" << DmixMean[index+1] << std::endl;
}
fmassFlux << " " << std::endl;
fmassFlux << "	Atomic balance error [-]" << std::endl;
for (int index=0;index<elementNames_.size();index++)
{
	fmassFlux << elementNames_[index] << "\t" << (ElementInlet[index+1] - ElementOutlet[index+1])*100/ElementInlet[index+1]  << "\t[%]" << std::endl;
}
fmassFlux.close();
