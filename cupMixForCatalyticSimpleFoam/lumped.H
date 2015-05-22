namespace OpenSMOKE
{
    template<typename map>
    class KineticsMap_Surface_CHEMKIN_Lumped : public KineticsMap_Surface_CHEMKIN<map>
    {
        public:

        KineticsMap_Surface_CHEMKIN_Lumped(ThermodynamicsMap_Surface_CHEMKIN<map>& thermo, rapidxml::xml_document<>& doc)
        : KineticsMap_Surface_CHEMKIN<map>(thermo, doc)
        {
            iO2   = this->thermodynamics_.IndexOfSpecies("O2");

            for (unsigned int j = 0; j < this->number_of_lumped_reactions_; j++)
            {
                
                if (this->names_of_lumped_functions_[j] == "Diretta")
                    iDiretta = this->indices_of_lumped_reactions_[j+1];
                else if (this->names_of_lumped_functions_[j] == "Inversa")
                    iInversa = this->indices_of_lumped_reactions_[j+1];
                else
                {
                    std::cout << "ERROR: " << this->names_of_lumped_functions_[j+1] << " NOT recognized LUMPED name!" << std::endl;
                    exit(-1);
                }
            }
        }


        virtual void UserDefinedReactionRates(const OpenSMOKEVectorDouble& c, 
                                                const OpenSMOKEVectorDouble& z, 
                                                const OpenSMOKEVectorDouble& a, 
                                                const OpenSMOKEVectorDouble& gamma)
        {


            this->netReactionRates_[iDiretta]  = 1.e06*c[iO2];

            this->netReactionRates_[iInversa]  = 0.;


        }

        private:

        unsigned int    iO2;
        unsigned int    iDiretta;
        unsigned int    iInversa;

    };

}
