#ifndef INC_GettextParser_hpp_
#define INC_GettextParser_hpp_

#line 2 "gettext.g"

#include <string>
using namespace std;
#include "parser.h"

#line 11 "GettextParser.hpp"
#include "antlr/config.hpp"
/* $ANTLR 2.7.1: "gettext.g" -> "GettextParser.hpp"$ */
#include "antlr/TokenStream.hpp"
#include "antlr/TokenBuffer.hpp"
#include "GettextParserTokenTypes.hpp"
#include "antlr/LLkParser.hpp"

class GettextParser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public GettextParserTokenTypes
 {
#line 1 "gettext.g"
#line 22 "GettextParser.hpp"
protected:
	GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	GettextParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	public:  MsgList  file();
	public: string  comment();
	public:  MsgBlock  file_block();
	public: string  msgid();
	public: string  msgstr();
	public: string  msgid_plural();
	public: string  msgstr_plural();
private:
	static const char* _tokenNames[];
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
};

#endif /*INC_GettextParser_hpp_*/
