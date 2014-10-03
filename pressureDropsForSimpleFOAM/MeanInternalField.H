scalar MeanInternalField(const Foam::fvMesh &mesh, const volScalarField &rho)
{
	scalar SumValue = 0.;
	scalar MeanValue = 0.;
	scalar Volume = 0.;
	
	const scalarField& rhoCells = rho.internalField();
	
	forAll(rhoCells, celli)
	{
		SumValue += rho[celli]*mesh.V()[celli];
		Volume += mesh.V()[celli];
	}
	MeanValue = SumValue/Volume;

	return MeanValue;
}
