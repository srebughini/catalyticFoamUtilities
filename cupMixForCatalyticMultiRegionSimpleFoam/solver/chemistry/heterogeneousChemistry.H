#ifndef heterogeneousChemistry_H
#define heterogeneousChemistry_H

class heterogeneousChemistry
{

public:

    heterogeneousChemistry(OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&          thermodynamicsMap, 
                           OpenSMOKE::KineticsMap_CHEMKIN<double>&                kineticsMap,
                           OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&  thermodynamicsSurfaceMap, 
                           OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>& kineticsSurfaceMap);

    void setPressure(const double P);

    void setEnergyEquation(const bool energy);
 
    void setCatalyticLoad(const double alfaCat);

    void setComposition(const OpenSMOKE::OpenSMOKEVectorDouble omega);

    void setCoverage(const OpenSMOKE::OpenSMOKEVectorDouble Z);

    void setTemperature(const double T);

    void reactionSourceTerms(const OpenSMOKE::OpenSMOKEVectorDouble& y,OpenSMOKE::OpenSMOKEVectorDouble &dy);

    void reactionJacobian   (const OpenSMOKE::OpenSMOKEVectorDouble& y,Eigen::VectorXd &J);

    void reactionJacobian   (const OpenSMOKE::OpenSMOKEVectorDouble& y,Eigen::MatrixXd &J);
    
    unsigned int NumberOfEquations();

    virtual int Equations(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y, OpenSMOKE::OpenSMOKEVectorDouble& dy);

    virtual int Print(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y);
    
private:

    OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&            thermodynamicsMap_;             //!< thermodynamic map
    OpenSMOKE::KineticsMap_CHEMKIN<double>&                  kineticsMap_;                   //!< kinetic map
    OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&    thermodynamicsSurfaceMap_;      //!< thermodynamic map
    OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>&   kineticsSurfaceMap_;            //!< kinetic map

    unsigned int NC_;
    unsigned int NE_;
    unsigned int SURF_NC_;
    unsigned int SURF_NP_;

    double P_;
    double T_;
    double Q_;
    double MW_;
    double alfaCat_;
    double rhoGas_;
    double cpGas_;

    bool energy_;

    OpenSMOKE::OpenSMOKEVectorDouble omega_;
    OpenSMOKE::OpenSMOKEVectorDouble x_;
    OpenSMOKE::OpenSMOKEVectorDouble c_;
    OpenSMOKE::OpenSMOKEVectorDouble RfromGas_;
    OpenSMOKE::OpenSMOKEVectorDouble RfromSurface_;
    OpenSMOKE::OpenSMOKEVectorDouble Z_;
    OpenSMOKE::OpenSMOKEVectorDouble Gamma_;
    OpenSMOKE::OpenSMOKEVectorDouble Rsurface_;
    OpenSMOKE::OpenSMOKEVectorDouble RsurfacePhases_;
    OpenSMOKE::OpenSMOKEVectorDouble dummy;

    OpenSMOKE::OpenSMOKEVectorDouble y_plus_;
    OpenSMOKE::OpenSMOKEVectorDouble dy_plus_;
    OpenSMOKE::OpenSMOKEVectorDouble dy_original_;

};

heterogeneousChemistry::heterogeneousChemistry( OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&          thermodynamicsMap,
                                                OpenSMOKE::KineticsMap_CHEMKIN<double>&                kineticsMap,
                                                OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&  thermodynamicsSurfaceMap, 
                                                OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>& kineticsSurfaceMap):
    thermodynamicsMap_(thermodynamicsMap), 
    kineticsMap_(kineticsMap),
    thermodynamicsSurfaceMap_(thermodynamicsSurfaceMap), 
    kineticsSurfaceMap_(kineticsSurfaceMap)
    {
        NC_        = thermodynamicsMap_.NumberOfSpecies();
        SURF_NC_   = thermodynamicsSurfaceMap_.number_of_site_species();
        SURF_NP_   = thermodynamicsSurfaceMap_.number_of_site_phases(0);
        NE_        = NC_ + 1;

        P_             = 0.;
        T_             = 0.;
        Q_             = 0.;
        MW_            = 0.;
        alfaCat_       = 0.;
        cpGas_         = 0.;

        energy_        = false;

        ChangeDimensions(NC_, &x_,            true);
        ChangeDimensions(NC_, &omega_,        true);
        ChangeDimensions(NC_, &c_,            true);
        ChangeDimensions(NC_, &RfromSurface_, true);

        ChangeDimensions(SURF_NC_, &Z_,              true);
        ChangeDimensions(SURF_NP_, &Gamma_,          true);
        ChangeDimensions(SURF_NC_, &Rsurface_,       true);
        ChangeDimensions(SURF_NP_, &RsurfacePhases_, true);

        ChangeDimensions(NE_, &y_plus_,      true);
        ChangeDimensions(NE_, &dy_plus_,     true);
        ChangeDimensions(NE_, &dy_original_, true);
    }

unsigned int heterogeneousChemistry::NumberOfEquations()
{
    return SURF_NC_;
}

void heterogeneousChemistry::setPressure(const double P)
{
    P_ = P;
}

void heterogeneousChemistry::setEnergyEquation(const bool energy)
{
    energy_ = energy;
}

void heterogeneousChemistry::setCatalyticLoad(const double alfaCat)
{
    alfaCat_ = alfaCat;
}

void heterogeneousChemistry::setComposition(const OpenSMOKE::OpenSMOKEVectorDouble omega)
{
    for (unsigned int i=1;i<=NC_;i++)
    {
        omega_[i] = omega[i];
    }
}

void heterogeneousChemistry::setCoverage(const OpenSMOKE::OpenSMOKEVectorDouble Z)
{
    for (unsigned int i=1;i<=SURF_NC_;i++)
    {
        Z_[i] = Z[i];
    }
}

void heterogeneousChemistry::setTemperature(const double T)
{
    T_ = T;
}

void heterogeneousChemistry::reactionSourceTerms(const OpenSMOKE::OpenSMOKEVectorDouble& y, OpenSMOKE::OpenSMOKEVectorDouble &dy)
{
    {
        unsigned int k=1;
        for(unsigned int i=1;i<=NC_;i++)
        {
            omega_[i] = std::max(y[k++],0.);
        }

        {
            T_ = std::max(y[k++],0.);
        }
    }

    // Calculates thermodynamic properties
    thermodynamicsMap_.SetTemperature(T_);
    thermodynamicsMap_.SetPressure(P_);
    thermodynamicsSurfaceMap_.SetPressure(P_);
    thermodynamicsSurfaceMap_.SetTemperature(T_);
    kineticsSurfaceMap_.SetPressure(P_);
    kineticsSurfaceMap_.SetTemperature(T_);

    // Calculates the pressure and the concentrations of species
    thermodynamicsMap_.MoleFractions_From_MassFractions(x_, MW_, omega_);
    const double cTot_ = P_/(PhysicalConstants::R_J_kmol * T_);
    Product(cTot_, x_, &c_);

    for(unsigned int i=1;i<=SURF_NP_;i++)
    {
        Gamma_[i] = thermodynamicsSurfaceMap_.matrix_densities_site_phases()[0][i-1];
    }

    kineticsSurfaceMap_.ReactionEnthalpiesAndEntropies();
    kineticsSurfaceMap_.ArrheniusKineticConstants();
    kineticsSurfaceMap_.ReactionRates(c_, Z_, dummy, Gamma_);
    kineticsSurfaceMap_.FormationRates(&RfromSurface_, &Rsurface_, &dummy, &RsurfacePhases_);

    if ( energy_ == true )
    {
        Q_ = kineticsSurfaceMap_.HeatRelease(RfromSurface_, Rsurface_, dummy);
    }
    else
    {
        Q_     = 0.;
    }

    // Recovering residuals
    {
        unsigned int k=1;

        // Gas phase species
        for (unsigned int i=1;i<=NC_;++i)
        {
            dy[k++] = alfaCat_*RfromSurface_[i]*thermodynamicsMap_.MW()[i];
        }

        // Energy
        {
            dy[k++] = alfaCat_*Q_;
        }
    }
}

void heterogeneousChemistry::reactionJacobian(const OpenSMOKE::OpenSMOKEVectorDouble& y,Eigen::VectorXd &J) 
{
    // Calculated as suggested by Buzzi (private communication)
     const double ZERO_DER = std::sqrt(OPENSMOKE_TINY_FLOAT);
     const double ETA2 = std::sqrt(OpenSMOKE::OPENSMOKE_MACH_EPS_DOUBLE);
     const double BETA = 1.e3 * OpenSMOKE::OPENSMOKE_MACH_EPS_DOUBLE;
     const double TOLR = 1.e-7;
     const double TOLA = 1.e-12;

    for(unsigned int i=1;i<=NE_;i++)
    {
        y_plus_[i] = y[i];
    }

    // Call equations
    reactionSourceTerms(y,dy_original_);     

    // Derivatives with respect to y[kd]
    for(int kd=1;kd<=NE_;kd++)
    {
        double hf = 1.e0;
        double error_weight = 1./(TOLA+TOLR*fabs(y[kd]));
        double hJ = ETA2 * fabs(std::max(y[kd], 1./error_weight));
        double hJf = hf/error_weight;
        hJ = std::max(hJ, hJf);
        hJ = std::max(hJ, ZERO_DER);

        // This is what is done by Buzzi
        double dy = std::min(hJ, 1.e-3 + 1e-3*fabs(y[kd]));
        double udy = 1. / dy;
        y_plus_[kd] += dy;

        reactionSourceTerms(y_plus_,dy_plus_);

        J(kd-1) = (dy_plus_[kd]-dy_original_[kd]) * udy;

        y_plus_[kd] = y[kd];
    }
}

void heterogeneousChemistry::reactionJacobian(const OpenSMOKE::OpenSMOKEVectorDouble& y,Eigen::MatrixXd &J) 
{
    // Calculated as suggested by Buzzi (private communication)
    const double ZERO_DER = std::sqrt(OPENSMOKE_TINY_FLOAT);
    const double ETA2 = std::sqrt(OpenSMOKE::OPENSMOKE_MACH_EPS_DOUBLE);
    const double BETA = 1.e3 * OpenSMOKE::OPENSMOKE_MACH_EPS_DOUBLE;
    const double TOLR = 1.e-7;
    const double TOLA = 1.e-12;

    for(unsigned int i=1;i<=NE_;i++)
    {
        y_plus_[i] = y[i];
    }

    // Call equations
    reactionSourceTerms(y,dy_original_);

     // Derivatives with respect to y[kd]
     for(int kd=1;kd<=NE_;kd++)
     {
         double hf = 1.e0;
         double error_weight = 1./(TOLA+TOLR*fabs(y[kd]));
         double hJ = ETA2 * fabs(std::max(y[kd], 1./error_weight));
         double hJf = hf/error_weight;
         hJ = std::max(hJ, hJf);
         hJ = std::max(hJ, ZERO_DER);

         // This is what is done by Buzzi
         double dy = std::min(hJ, 1.e-3 + 1e-3*fabs(y[kd]));
         double udy = 1. / dy;
         y_plus_[kd] += dy;

         reactionSourceTerms(y_plus_,dy_plus_);

         for(int j=1;j<=NE_;j++)
         {
             J(j-1,kd-1) = (dy_plus_[j]-dy_original_[j]) * udy;
         }

         y_plus_[kd] = y[kd];
     }
}

int heterogeneousChemistry::Equations(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y, OpenSMOKE::OpenSMOKEVectorDouble& dy)
{
    {
        for (unsigned int i=1;i<=SURF_NC_;i++)
        {
            Z_[i] = std::max(y[i],0.);
        }
    }

    // Calculates thermodynamic properties
    thermodynamicsMap_.SetTemperature(T_);
    thermodynamicsMap_.SetPressure(P_);
    thermodynamicsSurfaceMap_.SetPressure(P_);
    thermodynamicsSurfaceMap_.SetTemperature(T_);
    kineticsSurfaceMap_.SetPressure(P_);
    kineticsSurfaceMap_.SetTemperature(T_);

    // Calculates the pressure and the concentrations of species
    thermodynamicsMap_.MoleFractions_From_MassFractions(x_, MW_, omega_);
    const double cTot_ = P_/(PhysicalConstants::R_J_kmol * T_);
    Product(cTot_, x_, &c_);

    for(unsigned int i=1;i<=SURF_NP_;i++)
    {
        Gamma_[i] = thermodynamicsSurfaceMap_.matrix_densities_site_phases()[0][i-1];
    }

    kineticsSurfaceMap_.ReactionEnthalpiesAndEntropies();
    kineticsSurfaceMap_.ArrheniusKineticConstants();
    kineticsSurfaceMap_.ReactionRates(c_, Z_, dummy, Gamma_);
    kineticsSurfaceMap_.FormationRates(&RfromSurface_, &Rsurface_, &dummy, &RsurfacePhases_);

    // Recovering residuals
    {
        // Site species
        for (unsigned int i=1;i<=SURF_NC_;++i)
        {
            const unsigned int index_phase = thermodynamicsSurfaceMap_.vector_site_phases_belonging()[i-1]+1;
            dy[i] = (Rsurface_[i] - Z_[i]*Rsurface_.SumElements());
        }
    }

    return 0;
}

int heterogeneousChemistry::Print(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y)
{
}
#endif // heterogeneousChemistry_H
