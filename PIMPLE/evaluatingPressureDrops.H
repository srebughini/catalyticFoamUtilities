//Evaluate Pressure drops
double InletPressure;
double OutletPressure;
double PressureDrops;
for (int j=0;j<listInletBoundaries.size();j++)
{
	label patchID = mesh.boundaryMesh().findPatchID(listInletBoundaries[j]);
	forAll(Y[0].boundaryField()[patchID] , facei)
	{
		double area = mesh.magSf().boundaryField()[patchID][facei];
		InletPressure += area*p.boundaryField()[patchID][facei];
	}
}
for (int j=0;j<listOutletBoundaries.size();j++)
{
	label patchID = mesh.boundaryMesh().findPatchID(listOutletBoundaries[j]);
	forAll(Y[0].boundaryField()[patchID] , facei)
	{
		double area = mesh.magSf().boundaryField()[patchID][facei];
		OutletPressure += area*p.boundaryField()[patchID][facei];
	}
}
InletPressure=InletPressure/PatchInletArea;
OutletPressure=OutletPressure/PatchOutletArea;
PressureDrops = InletPressure - OutletPressure;
