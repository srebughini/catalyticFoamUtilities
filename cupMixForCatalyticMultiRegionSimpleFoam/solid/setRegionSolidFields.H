fvMesh& mesh = solidRegions[j];

tmp<volScalarField> trho         = rhoSolid[j];
volScalarField&      rho         = trho();

tmp<volScalarField> tp           = pSolid[j];
volScalarField&      p           = tp();

tmp<volScalarField> tcp          = cpSolid[j];
volScalarField&      cp          = tcp();

tmp<volScalarField> tT           = TSolid[j];
volScalarField&      T           = tT();

tmp<volScalarField> tlambda      = lambdaMaterial[j];
volScalarField&      lambda      = tlambda();

tmp<volScalarField> tMWmix       = MWmixSolid[j];
volScalarField&      MWmix       = tMWmix();

tmp<volScalarField> tbin         = binSolid[j];
volScalarField&      bin         = tbin();
