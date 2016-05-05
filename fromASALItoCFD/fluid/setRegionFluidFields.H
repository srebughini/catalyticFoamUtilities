fvMesh& mesh = fluidRegions[j];

tmp<volVectorField> tU      = UFluid[j];
volVectorField&      U      = tU();

tmp<volScalarField> tT      = TFluid[j];
volScalarField&      T      = tT();
