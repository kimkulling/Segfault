#pragma once

#include "core/segfault.h"

#include <cppcore/Container/TArray.h>

namespace segfault::core {

    //-------------------------------------------------------------------------------------------------
    ///	@ingroup    Runtime
    ///
    ///	@brief	This class implements a simple string tokenizer.
    ///
    ///	You can use it to split an arbitrary strings by self-defined separators. You can use it to
    ///	parse a string from any source.
    ///	Here is a simple example:
    ///	@code
    ///	String tst = "/usr/bin/ls";
    ///	StringArray token;
    ///	Tokenizer::tokenize( tst, token, "/" );
    ///	@endcode
    ///	This will return the strings usr, bin and ls.
    //-------------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT Tokenizer {
    public:
        ///	@brief	The class default constructor.
        Tokenizer() = default;
        
        ///	@brief	The class destructor.
        ~Tokenizer() = default;

        ///	@brief	Returns the number of stored token.
        ///	@return	The number of stored token.
        size_t getNumOfToken() const;
        
        ///	@brief	Returns the token at the given index.
        ///	@param	idx		[in] The given index, must be between ( 0 ... numToken-1 ).
        ///	@return	The token at the given index. If the index is out of range an empty string will be 
        ///			returned.
        const std::string &getTokenAt(uint32_t idx) const;
        
        ///	@brief	Returns true, if the stored list of token is empty.
        ///	@return	true, if no token are store, false if tokens are stored.
        bool isEmpty() const;

        ///	@brief	The internal token list will be cleared.
        void clear();
        
        ///	@brief	Will tokenize a given string by a given delimiter ( static version ).
        ///	@param	str			[in] The string to tokenize.
        ///	@param	tokens		[in] The result token list.
        ///	@param	delimiters	[in] The delimiter.
        ///	@return	The number of token.
        static size_t tokenize(const std::string &str, StringArray &tokens, const std::string &delimiters);

        ///	@brief	Will tokenize a given string by a given delimiter.
        ///	@param	str			[in] The string to tokenize.
        ///	@param	delimiters	[in] The delimiter.
        ///	@return	The number of token.
        size_t tokenize( const std::string& str, const std::string& delimiters );

        static bool isLineBreak( char c );

        Tokenizer(const Tokenizer&) = delete;
        Tokenizer& operator = (const Tokenizer &) = delete;

    private:
        StringArray m_Token;
    };

} // Namespace segfault::core
