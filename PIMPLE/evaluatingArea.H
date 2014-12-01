// Evaluate Inlet patch area
scalar PatchInletArea  = 0.;
for (int j=0;j<listInletBoundaries.size();j++)
{
	PatchInletArea +=  PatchArea(mesh, listInletBoundaries[j]);
}
// Evaluate Oulet patch area
scalar PatchOutletArea  = 0.;
for (int j=0;j<listOutletBoundaries.size();j++)
{
	PatchOutletArea +=  PatchArea(mesh, listOutletBoundaries[j]);
}
// Evaluate catalyticArea patch area
scalar PatchCatArea  = 0.;
for (int j=0;j<listCatalyticBoundaries.size();j++)
{
	PatchCatArea +=  PatchArea(mesh, listCatalyticBoundaries[j]);
}
