//- Atomic balance
OpenSMOKE::OpenSMOKEVectorDouble ElementInlet(elementNames_.size());
OpenSMOKE::OpenSMOKEVectorDouble ElementOutlet(elementNames_.size());
for (int index=0;index<elementNames_.size();index++)
{
	scalar speciesMassFlowInlet  = 0.;
	scalar speciesMassFlowOutlet = 0.;
	for (int k=0;k<listInletBoundaries.size();k++)
	{
		label patchID = fluidRegions[j].boundaryMesh().findPatchID(listInletBoundaries[k]);

		forAll(element_[0].boundaryField()[patchID] , facei)
		{
			double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
			speciesMassFlowInlet += area*element_[index].boundaryField()[patchID][facei];
		}
	}
	for (int k=0;k<listOutletBoundaries.size();k++)
	{

		label patchID = fluidRegions[j].boundaryMesh().findPatchID(listOutletBoundaries[k]);
		forAll(element_[0].boundaryField()[patchID] , facei)
		{
			double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
			speciesMassFlowOutlet += area*element_[index].boundaryField()[patchID][facei];
		}
	}
	ElementInlet[index+1] = speciesMassFlowInlet;
	ElementOutlet[index+1] = speciesMassFlowOutlet;
}
