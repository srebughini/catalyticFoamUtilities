template < typename T > std::string to_string( const T& v )
{
    std::ostringstream stm ;
    return ( stm << v ) ? stm.str() : "{*** error ***}" ;
}
