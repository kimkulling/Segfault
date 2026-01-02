#include "core/tokenizer.h"

namespace segfault::core {

    using namespace ::cppcore;

    static const std::string EmptyToken = "";

    size_t Tokenizer::getNumOfToken() const {
        return m_Token.size();
    }

    const std::string &Tokenizer::getTokenAt( uint32_t index ) const {
        if (m_Token.isEmpty() || index >= m_Token.size()) {
            return EmptyToken;
        }

        return m_Token[ index ];
    }

    bool Tokenizer::isEmpty() const {
        return m_Token.isEmpty();
    }

    void Tokenizer::clear() {
        m_Token.clear();
    }

    size_t Tokenizer::tokenize(const std::string &str, StringArray &tokens, const std::string &delimiters) {
        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of( delimiters, 0 );
        
        // find first "non-delimiter".
        std::string::size_type pos = str.find_first_of( delimiters, lastPos );
        while( std::string::npos != pos || std::string::npos != lastPos ) {
            // Found a token, add it to the vector.
            std::string tmp = str.substr( lastPos, pos - lastPos );
            if ( !tmp.empty() && ' ' != tmp[ 0 ] ) {
                tokens.add( tmp );
            }
            
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of( delimiters, pos );
            
            // Find next "non-delimiter"
            pos = str.find_first_of( delimiters, lastPos );
        }

        return static_cast<uint32_t>( tokens.size() );
    }

    size_t Tokenizer::tokenize( const std::string& str, const std::string& delimiters ) {
        if( !m_Token.isEmpty() ) {
            m_Token.resize( 0 );
        }
        
        tokenize( str, m_Token, delimiters );
        
        return m_Token.size();
    }

    bool Tokenizer::isLineBreak(char c) {
        if ( c == '\n' ) {
            return true;
        } 
        
        return false;
    }

} // namespace segfault::core
