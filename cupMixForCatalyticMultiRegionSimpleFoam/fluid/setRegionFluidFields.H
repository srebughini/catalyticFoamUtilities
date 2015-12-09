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

tmp<volScalarField> tMWmix  = MWmixFluid[j];
volScalarField&      MWmix  = tMWmix();

tmp<volScalarField> tmu     = muFluid[j];
volScalarField&      mu     = tmu();

tmp<volScalarField> tpsi    = psiFluid[j];
volScalarField&      psi    = tpsi();

surfaceScalarField& phi     = phiFluid[j];

const dimensionedScalar initialMass
(
    "initialMass",
    dimMass,
    initialMassFluid[j]
);
