scalar MaxInternalField(const Foam::fvMesh &mesh, const volScalarField &rho)
{
	scalar Value = 0.;
	scalar MaxValue = 0.;
	
	const scalarField& rhoCells = rho.internalField();
	
	forAll(rhoCells, celli)
	{
		Value = rho[celli];
		MaxValue = max(Value,MaxValue);
	}
	
	return MaxValue;
}
