#ifndef INC_GettextParserTokenTypes_hpp_
#define INC_GettextParserTokenTypes_hpp_

/* $ANTLR 2.7.1: "gettext.g" -> "GettextParserTokenTypes.hpp"$ */
struct GettextParserTokenTypes {
	enum {
		EOF_ = 1,
		T_MSGID = 4,
		T_COMMENT = 5,
		T_STRING = 6,
		T_MSGSTR = 7,
		WS = 8,
		MSG_TAG = 9,
		ESC = 10,
		NULL_TREE_LOOKAHEAD = 3
	};
};
#endif /*INC_GettextParserTokenTypes_hpp_*/
