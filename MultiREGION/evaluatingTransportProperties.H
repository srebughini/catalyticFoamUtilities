//- Calculate mean rho value
scalar rhoMean  = 0.;
rhoMean = MeanInternalField(fluidRegions[j], rho);
//- Calculate mean mu value
scalar muMean  = 0.;
muMean = MeanInternalField(fluidRegions[j], mu);
//- Calculate mean Dmix value
OpenSMOKE::OpenSMOKEVectorDouble DmixMean(thermodynamicsMapXML->NumberOfSpecies());
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	DmixMean[index+1]=MeanInternalField(fluidRegions[j], Dmix[index]);
}
//- Calculate mean Pressure
scalar pMean  = 0.;
pMean = MeanInternalField(fluidRegions[j], p);
//- Print inlet velocity
double Uinlet = 0.;
for (int j=0;j<listInletBoundaries.size();j++)
{
	label patchID = fluidRegions[j].boundaryMesh().findPatchID(listInletBoundaries[j]);
	forAll(Y[0].boundaryField()[patchID] , facei)
	{
		Uinlet = mag(U.boundaryField()[patchID][facei]);
	}
}
//- Calculate mean velocity
scalar Umean  = 0.;
Umean = MeanInternalField(fluidRegions[j], mag(U));
//- Calculate max velocity
scalar Umax  = 0.;
Umax = MaxInternalField(fluidRegions[j], mag(U));
