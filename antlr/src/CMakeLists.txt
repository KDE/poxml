
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

set(antlr_LIB_SRCS
  ANTLRException.cpp
  ASTFactory.cpp
  ASTRefCount.cpp
  BaseAST.cpp
  BitSet.cpp
  CharBuffer.cpp
  CharScanner.cpp
  CommonAST.cpp
  CommonASTWithHiddenTokens.cpp
  CommonHiddenStreamToken.cpp
  CommonToken.cpp
  InputBuffer.cpp
  LLkParser.cpp
  LexerSharedInputState.cpp
  MismatchedCharException.cpp
  MismatchedTokenException.cpp
  NoViableAltException.cpp
  NoViableAltForCharException.cpp
  Parser.cpp
  ParserSharedInputState.cpp
  RecognitionException.cpp
  String.cpp
  Token.cpp
  TokenBuffer.cpp
  TokenStreamBasicFilter.cpp
  TokenStreamHiddenTokenFilter.cpp
  TokenStreamSelector.cpp
  TreeParser.cpp
  TreeParserSharedInputState.cpp
)

kde4_add_library(antlr SHARED ${antlr_LIB_SRCS})
set_target_properties(antlr PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS antlr ${INSTALL_TARGETS_DEFAULT_ARGS})
