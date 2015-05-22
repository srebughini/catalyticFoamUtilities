fvMesh& mesh = fluidRegions[j];

tmp<volVectorField> tU      = UFluid[j];
volVectorField&      U      = tU();

tmp<volScalarField> trho    = rhoFluid[j];
volScalarField&      rho    = trho();

tmp<volScalarField> tp      = pFluid[j];
volScalarField&      p      = tp();

tmp<volScalarField> tT      = TFluid[j];
volScalarField&      T      = tT();

tmp<volScalarField> tcp     = cpFluid[j];
volScalarField&      cp     = tcp();

tmp<volScalarField> tlambda = lambdaFluid[j];
volScalarField&      lambda = tlambda();

tmp<volScalarField> tcTot   = cTotFluid[j];
volScalarField&      cTot   = tcTot();

tmp<volScalarField> tMWmix  = MWmixFluid[j];
volScalarField&      MWmix  = tMWmix();

tmp<volScalarField> tDpDt   = DpDtFluid[j];
volScalarField&      DpDt   = tDpDt();

tmp<volScalarField> tmu     = muFluid[j];
volScalarField&      mu     = tmu();

tmp<volScalarField> tpsi    = psiFluid[j];
volScalarField&      psi    = tpsi();

fv::IOoptionList& fvOptions = fluidFvOptions[j];

surfaceScalarField& phi     = phiFluid[j];
