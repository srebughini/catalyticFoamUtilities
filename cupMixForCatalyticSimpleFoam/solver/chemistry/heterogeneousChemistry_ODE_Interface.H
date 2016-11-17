#ifndef  heterogeneousChemistry_ODE_Interface_H
#define  heterogeneousChemistry_ODE_Interface_H

#include "math/OpenSMOKEVector.h"
#include "ode/OpenSMOKE_OdeSystemObject.h"

#include "ode/OpenSMOKE_OdeSystemObject.h"
#include "ODESystemVirtualClassWithOpenSMOKEVectors.H"
#include "heterogeneousChemistry.H"

namespace OpenSMOKE
{
    class heterogeneousChemistry_ODE_OpenSMOKE : public ODESystemVirtualClassWithOpenSMOKEVectors
    {
        public:

            heterogeneousChemistry_ODE_OpenSMOKE() {};

            void SetReactor(heterogeneousChemistry* reactor) { reactor_ = reactor; }

            virtual void GetEquations(const OpenSMOKE::OpenSMOKEVectorDouble& y, const double t, OpenSMOKE::OpenSMOKEVectorDouble& dy)
            {
                reactor_->Equations(t, y, dy);
            }
            virtual void PrintResults(const OpenSMOKE::OpenSMOKEVectorDouble& y, const double t)
            {
                reactor_->Print(t, y);
            }

        private:

            heterogeneousChemistry* reactor_;
    };
}

#endif    // heterogeneousChemistry_ODE_Interface_H

