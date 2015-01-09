{
	const scalarField& TCells = T.internalField();
	const scalarField& magUcells = magU.internalField();
	const scalarField& cpCells = cp.internalField();
	const scalarField& hCells = h.internalField();
	
	scalarField& cuphCells = cuph.internalField();
	scalarField& cuph_over_Tcells = cuph_over_T.internalField();
	
	forAll(TCells, celli)
	{
		cuphCells[celli] = hCells[celli]*magUcells[celli];
		cuph_over_Tcells[celli] = hCells[celli]*magUcells[celli]/TCells[celli];
	}

	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pT = T.boundaryField()[patchi];
		fvPatchScalarField& pmagU = magU.boundaryField()[patchi];
		fvPatchScalarField& pcp = cp.boundaryField()[patchi];
		fvPatchScalarField& ph = h.boundaryField()[patchi];
		
		fvPatchScalarField& pcuph = cuph.boundaryField()[patchi];
		fvPatchScalarField& pcuph_over_T = cuph_over_T.boundaryField()[patchi];

		forAll(pT, facei)
		{
			pcuph[facei] = ph[facei]*pmagU[facei];
			pcuph_over_T[facei] = ph[facei]*pmagU[facei]/pT[facei];
		}
	}
}
