//Write on terminal
cout.setf(std::ios::scientific);
Info << "-------------------------------------------------------------" << endl;
Info << "Mean velocity                = " << Umean << " [m/s]" << endl;
Info << "Max velocity                 = " << Umax << " [m/s]" << endl;
Info << "-------------------------------------------------------------" << endl;
Info << "Mean pressure                = " << pMean/1.e05 << " [bar]" << endl;
Info << "-------------------------------------------------------------" << endl;
Info << "Mean density                 = " << rhoMean << " [Kg/m3]" << endl;
Info << "-------------------------------------------------------------" << endl;
Info << "Mean viscosity               = " << muMean << " [Pas]" << endl;
Info << "-------------------------------------------------------------" << endl;
Info << "Mean specific heat           = " << cpMean << " [J/Kg/K]" << endl;
Info << "-------------------------------------------------------------" << endl;
Info << "Mean thermal conductivity    = " << kMean << " [W/m/K]" << endl;
Info << "-------------------------------------------------------------" << endl;

// Write on file
std::ofstream fmassFlux;
std::string fileName = "postProcessorRhoSimpleFoam.txt";
const char *pathFILE = fileName.c_str();
fmassFlux.open(pathFILE, std::ios::out);
fmassFlux.setf(std::ios::scientific);
fmassFlux.precision(6);
fmassFlux << " " << std::endl;
fmassFlux << "Mean velocity                = " << Umean << " [m/s]" << std::endl;
fmassFlux << "Max velocity                 = " << Umax << " [m/s]" << std::endl;
fmassFlux << " " <<  std::endl;
fmassFlux << "Mean pressure                = " << pMean/1.e05 << " [bar]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean density                 = " << rhoMean << " [Kg/m3]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean viscosity               = " << muMean << " [Pas]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean specific heat           = " << cpMean << " [J/Kg/K]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux << "Mean thermal conductivity    = " << kMean << " [W/m/K]" << std::endl;
fmassFlux << " " << std::endl;
fmassFlux.close();
