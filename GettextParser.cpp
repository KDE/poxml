/* $ANTLR 2.7.1: "gettext.g" -> "GettextParser.cpp"$ */
#include "GettextParser.hpp"
#include "antlr/NoViableAltException.hpp"
#include "antlr/SemanticException.hpp"
#line 12 "gettext.g"

#include <iostream>
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
#line 43 "gettext.g"
	 MsgList ml ;
#line 64 "GettextParser.cpp"
#line 43 "gettext.g"
	
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
#line 49 "gettext.g"
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
#line 50 "gettext.g"
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
#line 76 "gettext.g"
	string s;
#line 148 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  c = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 76 "gettext.g"
	
	string r;
	
#line 154 "GettextParser.cpp"
	
	try {      // for error handling
		if ((LA(1)==T_COMMENT)) {
			{
			c = LT(1);
			match(T_COMMENT);
			r=comment();
			if ( inputState->guessing==0 ) {
#line 80 "gettext.g"
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
#line 53 "gettext.g"
	 MsgBlock mb ;
#line 191 "GettextParser.cpp"
#line 53 "gettext.g"
	
	string c, mi, mip, ms;
	
#line 196 "GettextParser.cpp"
	
	try {      // for error handling
		c=comment();
		mi=msgid();
		{
		if ((LA(1)==T_MSGSTR)) {
			{
			ms=msgstr();
			if ( inputState->guessing==0 ) {
#line 59 "gettext.g"
				
					mb.comment = QString::fromUtf8(c.c_str());
					mb.msgid = QString::fromUtf8(mi.c_str());
					mb.msgstr = QString::fromUtf8(ms.c_str());   
				
#line 212 "GettextParser.cpp"
			}
			}
		}
		else if ((LA(1)==T_MSGID_PLURAL)) {
			{
			mip=msgid_plural();
			ms=msgstr_plural();
			if ( inputState->guessing==0 ) {
#line 66 "gettext.g"
				
					mb.comment = QString::fromUtf8(c.c_str());
					mb.msgid = QString::fromUtf8(mi.c_str());
					mb.msgid_plural = QString::fromUtf8(mip.c_str());
					mb.msgstr = QString::fromUtf8(ms.c_str());   
				
#line 228 "GettextParser.cpp"
			}
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
			consumeUntil(_tokenSet_2);
		} else {
			throw ex;
		}
	}
	return mb ;
}

string  GettextParser::msgid() {
#line 84 "gettext.g"
	string s;
#line 253 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(T_MSGID);
		t = LT(1);
		match(T_STRING);
		if ( inputState->guessing==0 ) {
#line 85 "gettext.g"
			s = t->getText();
#line 263 "GettextParser.cpp"
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
#line 92 "gettext.g"
	string s;
#line 281 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(T_MSGSTR);
		t = LT(1);
		match(T_STRING);
		if ( inputState->guessing==0 ) {
#line 93 "gettext.g"
			s = t->getText();
#line 291 "GettextParser.cpp"
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

string  GettextParser::msgid_plural() {
#line 88 "gettext.g"
	string s;
#line 309 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(T_MSGID_PLURAL);
		t = LT(1);
		match(T_STRING);
		if ( inputState->guessing==0 ) {
#line 89 "gettext.g"
			s = t->getText();
#line 319 "GettextParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_4);
		} else {
			throw ex;
		}
	}
	return s;
}

string  GettextParser::msgstr_plural() {
#line 96 "gettext.g"
	string s;
#line 337 "GettextParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		{
		int _cnt18=0;
		for (;;) {
			if ((LA(1)==T_MSGSTR)) {
				match(T_MSGSTR);
				match(L_BRACKET);
				n = LT(1);
				match(T_INT);
				match(R_BRACKET);
				t = LT(1);
				match(T_STRING);
				if ( inputState->guessing==0 ) {
#line 98 "gettext.g"
					s = t->getText();
#line 356 "GettextParser.cpp"
				}
			}
			else {
				if ( _cnt18>=1 ) { goto _loop18; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt18++;
		}
		_loop18:;
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
	"T_MSGID_PLURAL",
	"T_MSGSTR",
	"L_BRACKET",
	"T_INT",
	"R_BRACKET",
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
const unsigned long GettextParser::_tokenSet_3_data_[] = { 384UL, 0UL, 0UL, 0UL };
// T_MSGID_PLURAL T_MSGSTR 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long GettextParser::_tokenSet_4_data_[] = { 256UL, 0UL, 0UL, 0UL };
// T_MSGSTR 
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextParser::_tokenSet_4(_tokenSet_4_data_,4);


