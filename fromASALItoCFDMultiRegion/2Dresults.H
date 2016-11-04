unsigned int Na = 0;
unsigned int Nr = 0;

std::vector<double> rasali;
std::vector<double> zasali;
std::vector< std::vector<double> > Tasali;
{
    std::string temperaturepath;
    std::string axialpath;
    std::string radialpath;
    std::string geometrypath;
    
    std::vector<double> zf;
    std::vector< std::vector<double> > Tf;
    double inertL   = 0.;

    if (boost::filesystem::exists(asaliFolder))
    {
        if (boost::filesystem::is_directory(asaliFolder))  
        {
            temperaturepath = asaliFolder + "/2D-temp.txt";
            axialpath       = asaliFolder + "/axial-coordinate.txt";
            radialpath      = asaliFolder + "/radial-coordinate.txt";
            geometrypath    = asaliFolder + "/geometry.txt";
        }
    }

    {
        std::string dummyString;
        std::vector<double> dummyVector;

        if (boost::filesystem::exists(axialpath))
        {
            const char *pathGrid = axialpath.c_str();
            std::ifstream is(pathGrid);
            unsigned int k=0;
            while (getline(is,dummyString))
            {
                dummyVector.resize(k+1);
                dummyVector[k] = boost::lexical_cast<double>(dummyString);
                k++;
            }

            Na = dummyVector.size();
            zf.resize(Na);

            for (unsigned int k=0;k<Na;k++)
            {
                zf[k] = dummyVector[k];
            }
        }
        else
        {
            std::cout << "axial-coordinate.txt file NOT exist" << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    {
        std::string dummyString;
        std::vector<double> dummyVector;

        if (boost::filesystem::exists(radialpath))
        {
            const char *pathGrid = radialpath.c_str();
            std::ifstream is(pathGrid);
            unsigned int k=0;
            while (getline(is,dummyString))
            {
                dummyVector.resize(k+1);
                dummyVector[k] = boost::lexical_cast<double>(dummyString);
                k++;
            }

            Nr = dummyVector.size();
            rasali.resize(Nr);

            for (unsigned int k=0;k<Nr;k++)
            {
                rasali[k] = dummyVector[k];
            }
        }
        else
        {
            std::cout << "radial-coordinate.txt file NOT exist" << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    {
        std::string dummyString;
        std::vector<double> dummyVector;
        double dummyDouble;

        if (boost::filesystem::exists(temperaturepath))
        {
            const char *pathGrid = temperaturepath.c_str();
            std::ifstream is(pathGrid);
            unsigned int k=0;
            while (getline(is,dummyString))
            {
                std::istringstream iss(dummyString);
                while (iss >> dummyDouble)
                {
                    dummyVector.resize(k+1);
                    dummyVector[k] = dummyDouble;
                    k++;
                }
            }

            Tf.resize(Na);
            unsigned int counter = 0;
            for (unsigned int k=0;k<Na;k++)
            {
                Tf[k].resize(Nr);
                for (unsigned int i=0;i<Nr;i++) 
                {
                    Tf[k][i] = dummyVector[counter++];
                }
            }
        }
        else
        {
            std::cout << "2D-temp.txt file NOT exist" << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    {
        std::string dummyString;
        std::vector<std::string> dummyVector;

        if (boost::filesystem::exists(geometrypath))
        {
            const char *pathGrid = geometrypath.c_str();
            std::ifstream is(pathGrid);
            unsigned int k=0;
            while (getline(is,dummyString))
            {
                dummyVector.resize(k+1);
                dummyVector[k] = dummyString;
                k++;
            }

            for (unsigned int k=0;k<dummyVector.size();k++)
            {
                if ( dummyVector[k] == "Inert" && dummyVector[k+1] == "lenght" )
                {
                    inertL = boost::lexical_cast<double>(dummyVector[k+2]);
                }
            }
        }
        else
        {
            std::cout << "geometry.txt file NOT exist" << std::endl;
            exit (EXIT_FAILURE);
        }
    }
    
    Na = 0;
    for (unsigned int i=0;i<zf.size();i++)
    {
        if ( zf[i] >= inertL )
        {
            Na++;
        }
    }

    Tasali.resize(Na);
    zasali.resize(Na);
    unsigned int counter = 0;
    for (unsigned int k=0;k<zf.size();k++)
    {
        if ( zf[k] >= inertL )
        {
            zasali[counter] = zf[k] - inertL;
            Tasali[counter].resize(Nr);
            for (unsigned int i=0;i<Nr;i++) 
            {
                Tasali[counter][i] = Tf[k][i];
            }
            counter++;
        }
    }
}
