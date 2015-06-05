//Evaluate Pressure drops
double InletPressure;
double OutletPressure;
double PressureDrops;
for (int k=0;k<listInletBoundaries.size();k++)
{
	label patchID = fluidRegions[j].boundaryMesh().findPatchID(listInletBoundaries[k]);
	forAll(Y[0].boundaryField()[patchID] , facei)
	{
		double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
		InletPressure += area*p.boundaryField()[patchID][facei];
	}
}
for (int k=0;k<listOutletBoundaries.size();k++)
{
	label patchID = fluidRegions[j].boundaryMesh().findPatchID(listOutletBoundaries[k]);
	forAll(Y[0].boundaryField()[patchID] , facei)
	{
		double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
		OutletPressure += area*p.boundaryField()[patchID][facei];
	}
}
InletPressure=InletPressure/PatchInletArea;
OutletPressure=OutletPressure/PatchOutletArea;
PressureDrops = InletPressure - OutletPressure;
