/* $ANTLR 2.7.1: "gettext.g" -> "GettextParser.cpp"$ */
#include "GettextParser.hpp"
#include "antlr/NoViableAltException.hpp"
#include "antlr/SemanticException.hpp"
#line 10 "gettext.g"

#include <iostream>
#include <string>
using namespace std;
#include "GettextLexer.hpp"
#include "GettextParser.hpp"
#include "antlr/AST.hpp"
#include "antlr/CommonAST.hpp"

/* 
int main()
{
	ANTLR_USING_NAMESPACE(std)
	ANTLR_USING_NAMESPACE(antlr)
	try {
		GettextLexer lexer(cin);
		GettextParser parser(lexer);
		parser.file();

	} catch(exception& e) {
		cerr << "exception: " << e.what() << endl;
	}
}
*/

#line 30 "GettextParser.cpp"
GettextParser::GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
	setTokenNames(_tokenNames);
}

GettextParser::GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
	setTokenNames(_tokenNames);
}

GettextParser::GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
	setTokenNames(_tokenNames);
}

GettextParser::GettextParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
	setTokenNames(_tokenNames);
}

GettextParser::GettextParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
	setTokenNames(_tokenNames);
}

 MsgList  GettextParser::file() {
#line 41 "gettext.g"
	 MsgList ml ;
#line 64 "GettextParser.cpp"
#line 41 "gettext.g"
	
	string c, mi, ms;
	MsgBlock mb;
	MsgList ml2;
	
#line 71 "GettextParser.cpp"
	
	try {      // for error handling
		bool synPredMatched3 = false;
		if (((LA(1)==T_MSGID||LA(1)==T_COMMENT))) {
			int _m3 = mark();
			synPredMatched3 = true;
			inputState->guessing++;
			try {
				{
				comment();
				match(T_MSGID);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched3 = false;
			}
			rewind(_m3);
			inputState->guessing--;
		}
		if ( synPredMatched3 ) {
			{
			mb=file_block();
			ml2=file();
			if ( inputState->guessing==0 ) {
#line 47 "gettext.g"
				ml = ml2; ml.append(mb);
#line 98 "GettextParser.cpp"
			}
			}
		}
		else {
			bool synPredMatched6 = false;
			if (((LA(1)==ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE||LA(1)==T_COMMENT))) {
				int _m6 = mark();
				synPredMatched6 = true;
				inputState->guessing++;
				try {
					{
					comment();
					match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched6 = false;
				}
				rewind(_m6);
				inputState->guessing--;
			}
			if ( synPredMatched6 ) {
				c=comment();
				if ( inputState->guessing==0 ) {
#line 48 "gettext.g"
					(void)c;
#line 125 "GettextParser.cpp"
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_0);
		} else {
			throw ex;
		}
	}
	return ml ;
}

string  GettextParser::comment() {
#line 62 "gettext.g"
	string s;
#line 148 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  c = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 62 "gettext.g"
	
	string r;
	
#line 154 "GettextParser.cpp"
	
	try {      // for error handling
		if ((LA(1)==T_COMMENT)) {
			{
			c = LT(1);
			match(T_COMMENT);
			r=comment();
			if ( inputState->guessing==0 ) {
#line 66 "gettext.g"
				s = c->getText() + r;
#line 165 "GettextParser.cpp"
			}
			}
		}
		else if ((LA(1)==ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE||LA(1)==T_MSGID)) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw ex;
		}
	}
	return s;
}

 MsgBlock  GettextParser::file_block() {
#line 51 "gettext.g"
	 MsgBlock mb ;
#line 191 "GettextParser.cpp"
#line 51 "gettext.g"
	
	string c, mi, ms;
	
#line 196 "GettextParser.cpp"
	
	try {      // for error handling
		c=comment();
		mi=msgid();
		ms=msgstr();
		if ( inputState->guessing==0 ) {
#line 55 "gettext.g"
			
				(void)c;
				mb.msgid = QString::fromUtf8(mi.c_str());
				mb.msgstr = QString::fromUtf8(ms.c_str());   
			
#line 209 "GettextParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw ex;
		}
	}
	return mb ;
}

string  GettextParser::msgid() {
#line 70 "gettext.g"
	string s;
#line 227 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(T_MSGID);
		t = LT(1);
		match(T_STRING);
		if ( inputState->guessing==0 ) {
#line 71 "gettext.g"
			s = t->getText();
#line 237 "GettextParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_3);
		} else {
			throw ex;
		}
	}
	return s;
}

string  GettextParser::msgstr() {
#line 74 "gettext.g"
	string s;
#line 255 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(T_MSGSTR);
		t = LT(1);
		match(T_STRING);
		if ( inputState->guessing==0 ) {
#line 75 "gettext.g"
			s = t->getText();
#line 265 "GettextParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw ex;
		}
	}
	return s;
}

const char* GettextParser::_tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"T_MSGID",
	"T_COMMENT",
	"T_STRING",
	"T_MSGSTR",
	"WS",
	"MSG_TAG",
	"ESC",
	0
};

const unsigned long GettextParser::_tokenSet_0_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long GettextParser::_tokenSet_1_data_[] = { 18UL, 0UL, 0UL, 0UL };
// EOF T_MSGID 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long GettextParser::_tokenSet_2_data_[] = { 50UL, 0UL, 0UL, 0UL };
// EOF T_MSGID T_COMMENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long GettextParser::_tokenSet_3_data_[] = { 128UL, 0UL, 0UL, 0UL };
// T_MSGSTR 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_3(_tokenSet_3_data_,4);


