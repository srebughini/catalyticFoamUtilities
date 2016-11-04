template < typename T > std::string to_string( const T& v )
{
    std::ostringstream stm ;
    return ( stm << v ) ? stm.str() : "{*** error ***}" ;
}

void meshWarning()
{
    std::cout << "WARNING: this utility works ONLY if the mesh center line of the SOLID is (0,0,0) (0,0,L)" << std::endl;
}
