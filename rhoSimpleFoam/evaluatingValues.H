//- Calculate mean rho value
scalar rhoMean  = 0.;
rhoMean = MeanInternalField(mesh, rho);

//- Calculate mean mu value
scalar muMean  = 0.;
muMean = MeanInternalField(mesh, mu);

//- Calculate mean cp value
scalar cpMean  = 0.;
cpMean = MeanInternalField(mesh, cp);

//- Calculate mean k value
scalar kMean  = 0.;
kMean = MeanInternalField(mesh, k);

//- Calculate mean Pressure
scalar pMean  = 0.;
pMean = MeanInternalField(mesh, p);

//- Calculate mean velocity
scalar Umean  = 0.;
Umean = MeanInternalField(mesh, mag(U));

//- Calculate max velocity
scalar Umax  = 0.;
Umax = MaxInternalField(mesh, mag(U));
