/*
 * tester.c : a small tester program for parsing using the SAX API.
 *
 * See Copyright for the status of this software.
 *
 * Daniel.Veillard@w3.org
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <libxml/parser.h>
#include <libxml/parserInternals.h> /* only for xmlNewInputFromFile() */
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <libxml/xmlmemory.h>
#include <libxml/xml-error.h>

static int debug = 0;
static int copy = 0;
static int recovery = 0;
static int push = 0;
static int speed = 0;

xmlSAXHandler emptySAXHandlerStruct = {
    NULL, /* internalSubset */
    NULL, /* isStandalone */
    NULL, /* hasInternalSubset */
    NULL, /* hasExternalSubset */
    NULL, /* resolveEntity */
    NULL, /* getEntity */
    NULL, /* entityDecl */
    NULL, /* notationDecl */
    NULL, /* attributeDecl */
    NULL, /* elementDecl */
    NULL, /* unparsedEntityDecl */
    NULL, /* setDocumentLocator */
    NULL, /* startDocument */
    NULL, /* endDocument */
    NULL, /* startElement */
    NULL, /* endElement */
    NULL, /* reference */
    NULL, /* characters */
    NULL, /* ignorableWhitespace */
    NULL, /* processingInstruction */
    NULL, /* comment */
    NULL, /* xmlParserWarning */
    NULL, /* xmlParserError */
    NULL, /* xmlParserError */
    NULL, /* getParameterEntity */
    NULL, /* cdataBlock; */
    NULL
};

xmlSAXHandlerPtr emptySAXHandler = &emptySAXHandlerStruct;
extern xmlSAXHandlerPtr debugSAXHandler;

/************************************************************************
 *									*
 *				Debug Handlers				*
 *									*
 ************************************************************************/

/**
 * isStandaloneDebug:
 * @ctxt:  An XML parser context
 *
 * Is this document tagged standalone ?
 *
 * Returns 1 if true
 */
int
isStandaloneDebug(void *c)
{
    return(0);
}

/**
 * hasInternalSubsetDebug:
 * @ctxt:  An XML parser context
 *
 * Does this document has an internal subset
 *
 * Returns 1 if true
 */
int
hasInternalSubsetDebug(void *ctx)
{
    return(0);
}

/**
 * hasExternalSubsetDebug:
 * @ctxt:  An XML parser context
 *
 * Does this document has an external subset
 *
 * Returns 1 if true
 */
int
hasExternalSubsetDebug(void *ctx)
{
    return(0);
}

/**
 * hasInternalSubsetDebug:
 * @ctxt:  An XML parser context
 *
 * Does this document has an internal subset
 */
void
internalSubsetDebug(void *ctx, const xmlChar *name,
	       const xmlChar *ExternalID, const xmlChar *SystemID)
{
    /* xmlDtdPtr externalSubset; */

/***********
    if ((ExternalID != NULL) || (SystemID != NULL)) {
        externalSubset = xmlParseDTD(ExternalID, SystemID);
	if (externalSubset != NULL) {
	    xmlFreeDtd(externalSubset);
	}
    }
 ***********/
}

/**
 * resolveEntityDebug:
 * @ctxt:  An XML parser context
 * @publicId: The public ID of the entity
 * @systemId: The system ID of the entity
 *
 * Special entity resolver, better left to the parser, it has
 * more context than the application layer.
 * The default behaviour is to NOT resolve the entities, in that case
 * the ENTITY_REF nodes are built in the structure (and the parameter
 * values).
 *
 * Returns the xmlParserInputPtr if inlined or NULL for DOM behaviour.
 */
xmlParserInputPtr
resolveEntityDebug(void *ctx, const xmlChar *publicId, const xmlChar *systemId)
{
    /* xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx; */

/*********
    if (systemId != NULL) {
        return(xmlNewInputFromFile(ctxt, (char *) systemId));
    }
 *********/
    return(NULL);
}

/**
 * getEntityDebug:
 * @ctxt:  An XML parser context
 * @name: The entity name
 *
 * Get an entity by name
 *
 * Returns the xmlParserInputPtr if inlined or NULL for DOM behaviour.
 */
xmlEntityPtr
getEntityDebug(void *ctx, const xmlChar *name)
{
    return xmlGetPredefinedEntity("amp");
}

/**
 * getParameterEntityDebug:
 * @ctxt:  An XML parser context
 * @name: The entity name
 *
 * Get a parameter entity by name
 *
 * Returns the xmlParserInputPtr
 */
xmlEntityPtr
getParameterEntityDebug(void *ctx, const xmlChar *name)
{
    return(NULL);
}


/**
 * entityDeclDebug:
 * @ctxt:  An XML parser context
 * @name:  the entity name 
 * @type:  the entity type 
 * @publicId: The public ID of the entity
 * @systemId: The system ID of the entity
 * @content: the entity value (without processing).
 *
 * An entity definition has been parsed
 */
void
entityDeclDebug(void *ctx, const xmlChar *name, int type,
          const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
}

/**
 * attributeDeclDebug:
 * @ctxt:  An XML parser context
 * @name:  the attribute name 
 * @type:  the attribute type 
 *
 * An attribute definition has been parsed
 */
void
attributeDeclDebug(void *ctx, const xmlChar *elem, const xmlChar *name,
              int type, int def, const xmlChar *defaultValue,
	      xmlEnumerationPtr tree)
{
}

/**
 * elementDeclDebug:
 * @ctxt:  An XML parser context
 * @name:  the element name 
 * @type:  the element type 
 * @content: the element value (without processing).
 *
 * An element definition has been parsed
 */
void
elementDeclDebug(void *ctx, const xmlChar *name, int type,
	    xmlElementContentPtr content)
{
}

/**
 * notationDeclDebug:
 * @ctxt:  An XML parser context
 * @name: The name of the notation
 * @publicId: The public ID of the entity
 * @systemId: The system ID of the entity
 *
 * What to do when a notation declaration has been parsed.
 */
void
notationDeclDebug(void *ctx, const xmlChar *name,
	     const xmlChar *publicId, const xmlChar *systemId)
{
}

/**
 * unparsedEntityDeclDebug:
 * @ctxt:  An XML parser context
 * @name: The name of the entity
 * @publicId: The public ID of the entity
 * @systemId: The system ID of the entity
 * @notationName: the name of the notation
 *
 * What to do when an unparsed entity declaration is parsed
 */
void
unparsedEntityDeclDebug(void *ctx, const xmlChar *name,
		   const xmlChar *publicId, const xmlChar *systemId,
		   const xmlChar *notationName)
{
}

/**
 * setDocumentLocatorDebug:
 * @ctxt:  An XML parser context
 * @loc: A SAX Locator
 *
 * Receive the document locator at startup, actually xmlDefaultSAXLocator
 * Everything is available on the context, so this is useless in our case.
 */
void
setDocumentLocatorDebug(void *ctx, xmlSAXLocatorPtr loc)
{
}

/**
 * startDocumentDebug:
 * @ctxt:  An XML parser context
 *
 * called when the document start being processed.
 */
void
startDocumentDebug(void *ctx)
{
}

/**
 * endDocumentDebug:
 * @ctxt:  An XML parser context
 *
 * called when the document end has been detected.
 */
void
endDocumentDebug(void *ctx)
{
}

/**
 * startElementDebug:
 * @ctxt:  An XML parser context
 * @name:  The element name
 *
 * called when an opening tag has been processed.
 */
void
startElementDebug(void *ctx, const xmlChar *name, const xmlChar **atts)
{
}

/**
 * endElementDebug:
 * @ctxt:  An XML parser context
 * @name:  The element name
 *
 * called when the end of an element has been detected.
 */
void
endElementDebug(void *ctx, const xmlChar *name)
{
}

/**
 * charactersDebug:
 * @ctxt:  An XML parser context
 * @ch:  a xmlChar string
 * @len: the number of xmlChar
 *
 * receiving some chars from the parser.
 * Question: how much at a time ???
 */
void
charactersDebug(void *ctx, const xmlChar *ch, int len)
{
}

/**
 * referenceDebug:
 * @ctxt:  An XML parser context
 * @name:  The entity name
 *
 * called when an entity reference is detected. 
 */
void
referenceDebug(void *ctx, const xmlChar *name)
{
}

/**
 * ignorableWhitespaceDebug:
 * @ctxt:  An XML parser context
 * @ch:  a xmlChar string
 * @start: the first char in the string
 * @len: the number of xmlChar
 *
 * receiving some ignorable whitespaces from the parser.
 * Question: how much at a time ???
 */
void
ignorableWhitespaceDebug(void *ctx, const xmlChar *ch, int len)
{
}

/**
 * processingInstructionDebug:
 * @ctxt:  An XML parser context
 * @target:  the target name
 * @data: the PI data's
 * @len: the number of xmlChar
 *
 * A processing instruction has been parsed.
 */
void
processingInstructionDebug(void *ctx, const xmlChar *target,
                      const xmlChar *data)
{
}

/**
 * cdataBlockDebug:
 * @ctx: the user data (XML parser context)
 * @value:  The pcdata content
 * @len:  the block length
 *
 * called when a pcdata block has been parsed
 */
void
cdataBlockDebug(void *ctx, const xmlChar *value, int len)
{
}

/**
 * commentDebug:
 * @ctxt:  An XML parser context
 * @value:  the comment content
 *
 * A comment has been parsed.
 */
void
commentDebug(void *a, const xmlChar *b)
{
}

/**
 * warningDebug:
 * @ctxt:  An XML parser context
 * @msg:  the message to display/transmit
 * @...:  extra parameters for the message display
 *
 * Display and format a warning messages, gives file, line, position and
 * extra parameters.
 */
void
warningDebug(void *ctx, const char *msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stdout, "SAX.warning: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}

/**
 * errorDebug:
 * @ctxt:  An XML parser context
 * @msg:  the message to display/transmit
 * @...:  extra parameters for the message display
 *
 * Display and format a error messages, gives file, line, position and
 * extra parameters.
 */
void
errorDebug(void *ctx, const char *msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stdout, "SAX.error: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}

/**
 * fatalErrorDebug:
 * @ctxt:  An XML parser context
 * @msg:  the message to display/transmit
 * @...:  extra parameters for the message display
 *
 * Display and format a fatalError messages, gives file, line, position and
 * extra parameters.
 */
void
fatalErrorDebug(void *ctx, const char *msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stdout, "SAX.fatalError: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}

xmlSAXHandler debugSAXHandlerStruct = {
    internalSubsetDebug,
    isStandaloneDebug,
    hasInternalSubsetDebug,
    hasExternalSubsetDebug,
    resolveEntityDebug,
    getEntityDebug,
    entityDeclDebug,
    notationDeclDebug,
    attributeDeclDebug,
    elementDeclDebug,
    unparsedEntityDeclDebug,
    setDocumentLocatorDebug,
    startDocumentDebug,
    endDocumentDebug,
    startElementDebug,
    endElementDebug,
    referenceDebug,
    charactersDebug,
    ignorableWhitespaceDebug,
    processingInstructionDebug,
    commentDebug,
    warningDebug,
    xmlParserError,
    xmlParserError,
    getParameterEntityDebug,
    cdataBlockDebug
};

xmlSAXHandlerPtr debugSAXHandler = &debugSAXHandlerStruct;

/************************************************************************
 *									*
 *				Debug					*
 *									*
 ************************************************************************/

void parseAndPrintFile(char *filename) {
    FILE *f;
    char cmd_buffer[300];

    if (!push) {
	/*
	 * Empty callbacks for checking
	 */
	f = fopen(filename, "r");
    } else {
	sprintf(cmd_buffer, "xmlizer %s", filename);
	f = popen(cmd_buffer, "r");
    }

    if (f != NULL) {
	int res;
	char chars[10];
	xmlParserCtxtPtr ctxt;

	res = fread(chars, 1, 4, f);
	if (res > 0) {
	    ctxt = xmlCreatePushParserCtxt(emptySAXHandler, NULL,
					   chars, res, filename);
	    while ((res = fread(chars, 1, 3, f)) > 0) {
		xmlParseChunk(ctxt, chars, res, 0);
	    }
	    xmlParseChunk(ctxt, chars, 0, 1);
	    xmlFreeParserCtxt(ctxt);
	}
	if (push)
	    pclose(f);
	else
	    fclose(f);
    } else {
	fprintf(stderr, "Cannot read file %s\n", filename);
    }
/*	
 * Debug callback
 */
    if (!push) {
        /*
         * Empty callbacks for checking
         */
        f = fopen(filename, "r");
    } else
        f = popen(cmd_buffer, "r");
    
if (f != NULL) {
	    int res;
	    char chars[10];
	    xmlParserCtxtPtr ctxt;

	    res = fread(chars, 1, 4, f);
	    if (res > 0) {
		ctxt = xmlCreatePushParserCtxt(debugSAXHandler, NULL,
			    chars, res, filename);
		while ((res = fread(chars, 1, 3, f)) > 0) {
		    xmlParseChunk(ctxt, chars, res, 0);
		}
		res = xmlParseChunk(ctxt, chars, 0, 1);
		xmlFreeParserCtxt(ctxt);
	    }
            if (push)
                pclose(f);
            else
                fclose(f);

	}
}


int main(int argc, char **argv) {
    int i;
    int files = 0;

    for (i = 1; i < argc ; i++) {
	if ((!strcmp(argv[i], "-debug")) || (!strcmp(argv[i], "--debug")))
	    debug++;
	else if ((!strcmp(argv[i], "-copy")) || (!strcmp(argv[i], "--copy")))
	    copy++;
	else if ((!strcmp(argv[i], "-recover")) ||
	         (!strcmp(argv[i], "--recover")))
	    recovery++;
	else if ((!strcmp(argv[i], "-push")) ||
	         (!strcmp(argv[i], "--push")))
	    push++;
	else if ((!strcmp(argv[i], "-speed")) ||
	         (!strcmp(argv[i], "--speed")))
	    speed++;
    }
    for (i = 1; i < argc ; i++) {
	if (argv[i][0] != '-') {
	    parseAndPrintFile(argv[i]);
	    files ++;
	}
    }
    xmlCleanupParser();
    xmlMemoryDump();

    return(0);
}
