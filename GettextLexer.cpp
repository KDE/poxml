/* $ANTLR 2.7.1: "gettext.g" -> "GettextLexer.cpp"$ */
#include "GettextLexer.hpp"
#include "antlr/CharBuffer.hpp"
#include "antlr/TokenStreamException.hpp"
#include "antlr/TokenStreamIOException.hpp"
#include "antlr/TokenStreamRecognitionException.hpp"
#include "antlr/CharStreamException.hpp"
#include "antlr/CharStreamIOException.hpp"
#include "antlr/NoViableAltForCharException.hpp"

#line 1 "gettext.g"

#line 14 "GettextLexer.cpp"
GettextLexer::GettextLexer(ANTLR_USE_NAMESPACE(std)istream& in)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in))
{
	setCaseSensitive(true);
	initLiterals();
}

GettextLexer::GettextLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(ib)
{
	setCaseSensitive(true);
	initLiterals();
}

GettextLexer::GettextLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(state)
{
	setCaseSensitive(true);
	initLiterals();
}

void GettextLexer::initLiterals()
{
}
bool GettextLexer::getCaseSensitiveLiterals() const
{
	return true;
}

ANTLR_USE_NAMESPACE(antlr)RefToken GettextLexer::nextToken()
{
	ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
	for (;;) {
		ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
		int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
		resetText();
		try {   // for char stream error handling
			try {   // for lexical error handling
				switch ( LA(1)) {
				case static_cast<unsigned char>('\t'):
				case static_cast<unsigned char>('\n'):
				case static_cast<unsigned char>('\r'):
				case static_cast<unsigned char>(' '):
				{
					mWS(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('['):
				{
					mL_BRACKET(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>(']'):
				{
					mR_BRACKET(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('0'):
				case static_cast<unsigned char>('1'):
				case static_cast<unsigned char>('2'):
				case static_cast<unsigned char>('3'):
				case static_cast<unsigned char>('4'):
				case static_cast<unsigned char>('5'):
				case static_cast<unsigned char>('6'):
				case static_cast<unsigned char>('7'):
				case static_cast<unsigned char>('8'):
				case static_cast<unsigned char>('9'):
				{
					mT_INT(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('#'):
				{
					mT_COMMENT(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('m'):
				{
					mMSG_TAG(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('"'):
				{
					mT_STRING(true);
					theRetToken=_returnToken;
					break;
				}
				default:
				{
					if (LA(1)==EOF_CHAR) {uponEOF(); _returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);}
				else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				}
				if ( !_returnToken ) goto tryAgain; // found SKIP token
				_ttype = _returnToken->getType();
				_returnToken->setType(_ttype);
				return _returnToken;
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
				throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamIOException& csie) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamIOException(csie.io);
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamException& cse) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamException(cse.getMessage());
		}
tryAgain:;
	}
}

void GettextLexer::mWS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WS;
	int _saveIndex;
	
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>(' '):
	{
		match(static_cast<unsigned char>(' '));
		break;
	}
	case static_cast<unsigned char>('\t'):
	{
		match(static_cast<unsigned char>('\t'));
		break;
	}
	case static_cast<unsigned char>('\n'):
	case static_cast<unsigned char>('\r'):
	{
		{
		switch ( LA(1)) {
		case static_cast<unsigned char>('\n'):
		{
			match(static_cast<unsigned char>('\n'));
			break;
		}
		case static_cast<unsigned char>('\r'):
		{
			match("\r\n");
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
		}
		}
		}
#line 110 "gettext.g"
		newline();
#line 173 "GettextLexer.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
#line 111 "gettext.g"
	_ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP;
#line 184 "GettextLexer.cpp"
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mL_BRACKET(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = L_BRACKET;
	int _saveIndex;
	
	match(static_cast<unsigned char>('['));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mR_BRACKET(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = R_BRACKET;
	int _saveIndex;
	
	match(static_cast<unsigned char>(']'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mT_INT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = T_INT;
	int _saveIndex;
	
	{
	int _cnt26=0;
	for (;;) {
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
		}
		else {
			if ( _cnt26>=1 ) { goto _loop26; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt26++;
	}
	_loop26:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mT_COMMENT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = T_COMMENT;
	int _saveIndex;
	
	match(static_cast<unsigned char>('#'));
	{
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			matchNot(static_cast<unsigned char>('\n'));
		}
		else {
			goto _loop29;
		}
		
	}
	_loop29:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mMSG_TAG(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = MSG_TAG;
	int _saveIndex;
	
	match("msg");
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('i'):
	{
		{
		match("id");
		}
		{
		if ((LA(1)==static_cast<unsigned char>('_'))) {
			match("_plural");
#line 126 "gettext.g"
			_ttype = T_MSGID_PLURAL;
#line 292 "GettextLexer.cpp"
		}
		else {
			match("");
#line 125 "gettext.g"
			_ttype = T_MSGID;
#line 298 "GettextLexer.cpp"
		}
		
		}
		break;
	}
	case static_cast<unsigned char>('s'):
	{
		match("str");
#line 128 "gettext.g"
		_ttype = T_MSGSTR;
#line 309 "GettextLexer.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mT_STRING(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = T_STRING;
	int _saveIndex;
	
	{
	int _cnt43=0;
	for (;;) {
		if ((LA(1)==static_cast<unsigned char>('"'))) {
			_saveIndex=text.length();
			match(static_cast<unsigned char>('"'));
			text.erase(_saveIndex);
			{
			for (;;) {
				if ((LA(1)==static_cast<unsigned char>('\\'))) {
					mESC(false);
				}
				else if ((_tokenSet_1.member(LA(1)))) {
					matchNot(static_cast<unsigned char>('"'));
				}
				else {
					goto _loop37;
				}
				
			}
			_loop37:;
			}
			{
			_saveIndex=text.length();
			match(static_cast<unsigned char>('"'));
			text.erase(_saveIndex);
			{
			for (;;) {
				switch ( LA(1)) {
				case static_cast<unsigned char>(' '):
				{
					match(static_cast<unsigned char>(' '));
					break;
				}
				case static_cast<unsigned char>('t'):
				{
					match(static_cast<unsigned char>('t'));
					break;
				}
				default:
				{
					goto _loop40;
				}
				}
			}
			_loop40:;
			}
			_saveIndex=text.length();
			match(static_cast<unsigned char>('\n'));
			text.erase(_saveIndex);
#line 133 "gettext.g"
			newline();
#line 383 "GettextLexer.cpp"
			{
			for (;;) {
				switch ( LA(1)) {
				case static_cast<unsigned char>(' '):
				{
					_saveIndex=text.length();
					match(static_cast<unsigned char>(' '));
					text.erase(_saveIndex);
					break;
				}
				case static_cast<unsigned char>('\t'):
				{
					_saveIndex=text.length();
					match(static_cast<unsigned char>('\t'));
					text.erase(_saveIndex);
					break;
				}
				default:
				{
					goto _loop42;
				}
				}
			}
			_loop42:;
			}
			}
		}
		else {
			if ( _cnt43>=1 ) { goto _loop43; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt43++;
	}
	_loop43:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void GettextLexer::mESC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = ESC;
	int _saveIndex;
	
	match(static_cast<unsigned char>('\\'));
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('n'):
	{
		match(static_cast<unsigned char>('n'));
		break;
	}
	case static_cast<unsigned char>('r'):
	{
		match(static_cast<unsigned char>('r'));
		break;
	}
	case static_cast<unsigned char>('t'):
	{
		match(static_cast<unsigned char>('t'));
		break;
	}
	case static_cast<unsigned char>('b'):
	{
		match(static_cast<unsigned char>('b'));
		break;
	}
	case static_cast<unsigned char>('f'):
	{
		match(static_cast<unsigned char>('f'));
		break;
	}
	case static_cast<unsigned char>('"'):
	{
		match(static_cast<unsigned char>('"'));
		break;
	}
	case static_cast<unsigned char>('\''):
	{
		match(static_cast<unsigned char>('\''));
		break;
	}
	case static_cast<unsigned char>('\\'):
	{
		match(static_cast<unsigned char>('\\'));
		break;
	}
	case static_cast<unsigned char>('0'):
	case static_cast<unsigned char>('1'):
	case static_cast<unsigned char>('2'):
	case static_cast<unsigned char>('3'):
	{
		{
		matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('3'));
		}
		{
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			{
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
			}
			{
			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
			}
			else if (((LA(1) >= static_cast<unsigned char>('\0') && LA(1) <= static_cast<unsigned char>('\377')))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
			}
			
			}
		}
		else if (((LA(1) >= static_cast<unsigned char>('\0') && LA(1) <= static_cast<unsigned char>('\377')))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
		}
		
		}
		break;
	}
	case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):
	case static_cast<unsigned char>('6'):
	case static_cast<unsigned char>('7'):
	{
		{
		matchRange(static_cast<unsigned char>('4'),static_cast<unsigned char>('7'));
		}
		{
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			{
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
			}
		}
		else if (((LA(1) >= static_cast<unsigned char>('\0') && LA(1) <= static_cast<unsigned char>('\377')))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
		}
		
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}


const unsigned long GettextLexer::_tokenSet_0_data_[] = { 4294966271UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextLexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long GettextLexer::_tokenSet_1_data_[] = { 4294967295UL, 4294967291UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet GettextLexer::_tokenSet_1(_tokenSet_1_data_,16);

