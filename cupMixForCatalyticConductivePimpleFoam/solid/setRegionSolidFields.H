fvMesh& mesh = solidRegions[j];

tmp<volScalarField> trho    = rhoSolid[j];
volScalarField&      rho    = trho();

tmp<volScalarField> tp      = pSolid[j];
volScalarField&      p      = tp();

tmp<volScalarField> tT      = TSolid[j];
volScalarField&      T      = tT();

tmp<volScalarField> tcp     = cpSolid[j];
volScalarField&      cp     = tcp();

tmp<volScalarField> tlambda = lambdaSolid[j];
volScalarField&      lambda = tlambda();

