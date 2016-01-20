fvMesh& mesh = solidRegions[j];

tmp<volScalarField> tT      = TSolid[j];
volScalarField&      T      = tT();

