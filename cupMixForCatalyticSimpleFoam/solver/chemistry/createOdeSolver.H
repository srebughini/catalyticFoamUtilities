// ODE Solver
typedef OdeSMOKE::KernelDense<OpenSMOKE::heterogeneousChemistry_ODE_OpenSMOKE> denseODE;
typedef OdeSMOKE::MethodGear<denseODE> methodGearODE;
OdeSMOKE::MultiValueSolver<methodGearODE> odeSolver;
odeSolver.SetReactor(&het);
