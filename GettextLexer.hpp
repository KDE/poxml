#ifndef INC_GettextLexer_hpp_
#define INC_GettextLexer_hpp_

#line 2 "gettext.g"

#include <string>
using namespace std;
#include "parser.h"

#line 11 "GettextLexer.hpp"
#include "antlr/config.hpp"
/* $ANTLR 2.7.1: "gettext.g" -> "GettextLexer.hpp"$ */
#include "antlr/CommonToken.hpp"
#include "antlr/InputBuffer.hpp"
#include "antlr/BitSet.hpp"
#include "GettextParserTokenTypes.hpp"
#include "antlr/CharScanner.hpp"
class GettextLexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public GettextParserTokenTypes
 {
#line 1 "gettext.g"
#line 22 "GettextLexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const;
public:
	GettextLexer(ANTLR_USE_NAMESPACE(std)istream& in);
	GettextLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	GettextLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mWS(bool _createToken);
	public: void mL_BRACKET(bool _createToken);
	public: void mR_BRACKET(bool _createToken);
	public: void mT_INT(bool _createToken);
	public: void mT_COMMENT(bool _createToken);
	public: void mMSG_TAG(bool _createToken);
	public: void mT_STRING(bool _createToken);
	protected: void mESC(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
};

#endif /*INC_GettextLexer_hpp_*/
