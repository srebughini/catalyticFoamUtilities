//- Calculate mean rho value
scalar rhoMean  = 0.;
rhoMean = MeanInternalField(mesh, rho);
//- Calculate mean mu value
scalar muMean  = 0.;
muMean = MeanInternalField(mesh, mu);
//- Calculate mean Dmix value
OpenSMOKE::OpenSMOKEVectorDouble DmixMean(thermodynamicsMapXML->NumberOfSpecies());
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
    DmixMean[index+1]=MeanInternalField(mesh, Dmix[index]);
}
//- Calculate mean Pressure
scalar pMean  = 0.;
pMean = MeanInternalField(mesh, p);
//- Print inlet velocity
double Uinlet = 0.;
for (int j=0;j<listInletBoundaries.size();j++)
{
    label patchID = mesh.boundaryMesh().findPatchID(listInletBoundaries[j]);
    forAll(Y[0].boundaryField()[patchID] , facei)
    {
        Uinlet = mag(U.boundaryField()[patchID][facei]);
    }
}
//- Calculate mean velocity
scalar Umean  = 0.;
Umean = MeanInternalField(mesh, mag(U));
//- Calculate max velocity
scalar Umax  = 0.;
Umax = MaxInternalField(mesh, mag(U));
