/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: SCMPrint.cpp 903149 2010-01-26 09:58:40Z borisk $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include <map>
#include <list>
#include "members_gen.h"
#include "struct_def.h"

XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
static void usage();
class cfg_bin_struct_generator;
void processElements(XSNamedMap<XSObject> *xsElements);
void processTypeDefinitions(XSNamedMap<XSObject> *xsTypeDefs);
void printBasic(XSObject *xsObject, const char *type);
void printCompositorTypeConnector(XSModelGroup::COMPOSITOR_TYPE type);
void processSimpleTypeDefinition(XSSimpleTypeDefinition * xsSimpleTypeDef);
void processComplexTypeDefinition(XSComplexTypeDefinition *xsComplexTypeDef,XSElementDeclaration*xsElement=0);
void processParticle(XSParticle *xsParticle,cfg_bin_struct_generator *pcfg_bin_generator=0,ostream &stream=XERCES_STD_QUALIFIER cout);

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
{
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
}

~StrX()
{
    XMLString::release(&fLocalForm);
}


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
const char* localForm() const
{
    return fLocalForm;
}

private :
// -----------------------------------------------------------------------
//  Private data members
//
//  fLocalForm
//      This is the local code page form of the string.
// -----------------------------------------------------------------------
char*   fLocalForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

class SCMPrintHandler : public DefaultHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SCMPrintHandler();
    ~SCMPrintHandler();

    bool getSawErrors() const
    {
        return fSawErrors;
    }

	void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);
    void resetErrors();


private:
    bool            fSawErrors;
};

SCMPrintHandler::SCMPrintHandler() :
    fSawErrors(false)
{
}

SCMPrintHandler::~SCMPrintHandler()
{
}

// ---------------------------------------------------------------------------
//  SCMPrintHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SCMPrintHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SCMPrintHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SCMPrintHandler::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SCMPrintHandler::resetErrors()
{
    fSawErrors = false;
}

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    XERCES_STD_QUALIFIER cout << "\nUsage:\n"
    "    SCMPrint [options] <XSD file | List file>\n\n"
    "This program parses XML Schema file(s), to show how one can\n"
    "access the Schema Content Model information.\n\n"
    "Options:\n"
	"    -f     Enable full schema constraint checking processing. Defaults to off.\n"
    "    -l     Indicate the input file is a List File that has a list of XSD files.\n"
    "           Default to off (Input file is a XSD file).\n"
	"    -?     Show this help.\n\n"
    << XERCES_STD_QUALIFIER endl;
}

class cfg_bin_struct_generator : public struct_def_gen_class,public members_gen_gen_class
{
    private:
        typedef std::map<string,cfg_bin_struct_generator> structdef_map;
        typedef std::pair<string,cfg_bin_struct_generator> structdef_pair;
        typedef std::list<cfg_bin_struct_generator> structdef_list;

        XSComplexTypeDefinition *xsComplexTypeDef;
        XSElementDeclaration *xsElement;
        unsigned int indent;
        static structdef_map structDefUnresolved,structDefDefinedType;
        static structdef_list structDefResolved;
            
    public:
        cfg_bin_struct_generator(XSComplexTypeDefinition *xsComplexTypeDef,XSElementDeclaration *xsElement=0,char * typeName=0)
        {
            XSObject * obj=xsElement==0?(XSObject*)xsComplexTypeDef:(XSObject*)xsElement;
            this->xsComplexTypeDef=xsComplexTypeDef;
            this->xsElement=xsElement;
            this->indent=0;
            if( obj != 0)
                this->set_struc_name(StrX(obj->getName()).localForm());
            else
                this->set_struc_name(typeName);
            this->set_serialize_para_type("archive*");
            this->set_serialize_para_name("ar");
            this->set_deserialize_para_type("dearchive*");
            this->set_deserialize_para_name("dr");
        }
        ~cfg_bin_struct_generator(){}
        void generate_members(ostream &stream, unsigned int indent)
        {
            this->indent=indent;
            processParticle(this->xsComplexTypeDef->getParticle(),this,stream);
        }
        void generate_serialize(ostream &stream, unsigned int indent)
        {
            this->indent=indent;
        }
        void generate_deserialize(ostream &stream, unsigned int indent)
        {
            this->indent=indent;
        }
        void generate_member(const char *member_type,const char *member_name,ostream &stream)
        {
            this->set_member_type(member_type);
            this->set_member_name(member_name);
            this->generate_members_gen(stream,indent);
        }
        bool is_structdef_resolved(structdef_map &defined,XSParticle *xsParticle=0)
        {
            XSParticle * Particle = xsParticle==0?this->xsComplexTypeDef->getParticle():xsParticle;
            XSParticle::TERM_TYPE termType =Particle->getTermType();
            if(XSParticle::TERM_ELEMENT == termType)
            {
                string typeName(StrX(Particle->getElementTerm()->getTypeDefinition()->getName()).localForm());
                return is_this_struct_defined(typeName,defined);
            }else if(termType == XSParticle::TERM_MODELGROUP){
                XSModelGroup *xsModelGroup = Particle->getModelGroupTerm();
                XSModelGroup::COMPOSITOR_TYPE compositorType = xsModelGroup->getCompositor();
                XSParticleList *xsParticleList = xsModelGroup->getParticles();
                if(compositorType != XSModelGroup::COMPOSITOR_SEQUENCE){
                    XERCES_STD_QUALIFIER cout << __func__<< " member not sequence"<< XERCES_STD_QUALIFIER endl;
                    return false;
                }
                for (unsigned i = 0; i < xsParticleList->size(); i++) {
                    if(false == is_structdef_resolved(defined,xsParticleList->elementAt(i)))
                        return false;
                }
                return true;
            } else if (termType == XSParticle::TERM_WILDCARD) {
                XERCES_STD_QUALIFIER cout << __func__<< " whildcard not expected"<< XERCES_STD_QUALIFIER endl;
            }
            XERCES_STD_QUALIFIER cout << __func__<< " unknown term type"<< XERCES_STD_QUALIFIER endl;
            return false;
        } 
        static void init_cfg_bin_struct_generate_class(char *baseType[])
        {
            structDefResolved.clear();
            structDefUnresolved.clear();
            structDefDefinedType.clear();
            for(int i =0;baseType[i] != 0;i++)
            {
                structDefDefinedType.insert(structdef_pair(baseType[i],cfg_bin_struct_generator(0,0,baseType[i])));
            }
        }
        static void add_to_type_defines(cfg_bin_struct_generator *pcfg)
        {
            structdef_map toRemove;
            structDefUnresolved.insert(structdef_pair(pcfg->struc_name,*pcfg));
            for(structdef_map::iterator it=structDefUnresolved.begin();it != structDefUnresolved.end(); ++it)
            {
                if(it->second.is_structdef_resolved(structDefDefinedType)==true)
                {
                    XERCES_STD_QUALIFIER cout << "adding structure "<< it->first << XERCES_STD_QUALIFIER endl;
                    toRemove.insert(structdef_pair(it->first,it->second));
                    structDefResolved.push_back(it->second);
                    structDefDefinedType.insert(structdef_pair(it->first,it->second));
                }
            }
            for(structdef_map::iterator it=toRemove.begin();it != toRemove.end(); ++it)
            {
                structDefUnresolved.erase(it->first);
            }
        }
        static void cfg_bin_output(ostream &stream)
        {
            stream << "structures not resolved \n";

            for(structdef_map::iterator it=structDefUnresolved.begin();it != structDefUnresolved.end(); ++it)
            {
                stream << it->first << "  ";
            }
            stream << "\n+++++++++++++++++++++++++++++++++++++++++++\n";

            for(structdef_list::iterator it=structDefResolved.begin();it != structDefResolved.end(); ++it)
            {
                it->generate_struct_def(stream);
            }
        }
    private:
        bool is_this_struct_defined(string &name,structdef_map &defined)
        {
            for(structdef_map::iterator it=defined.begin();it != defined.end(); ++it)
            {
                if(it->first == name)
                    return true;
            }
            return false;
        }
};
cfg_bin_struct_generator::structdef_map cfg_bin_struct_generator::structDefUnresolved,cfg_bin_struct_generator::structDefDefinedType;
cfg_bin_struct_generator::structdef_list cfg_bin_struct_generator::structDefResolved;

// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        return 1;
    }
    const char * baseType[]={"string","integer",0};
    cfg_bin_struct_generator::init_cfg_bin_struct_generate_class((char**)baseType);

    // cannot return out of catch-blocks lest exception-destruction
    // result in calls to destroyed memory handler!
    int errorCode = 0;
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER cerr   << "Error during initialization! Message:\n"
        << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
        errorCode = 2;
    }
    if(errorCode) {
        XMLPlatformUtils::Terminate();
        return errorCode;
    }

    bool							doList				= false;
    bool							schemaFullChecking	= false;
    const char*                     xsdFile             = 0;
    int argInd;

    for (argInd = 1; argInd < argC; argInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[argInd][0] != '-')
            break;

        // Watch for special case help request
        if (!strcmp(argV[argInd], "-?"))
        {
            usage();
            return 1;
        }
        else if (!strcmp(argV[argInd], "-l")
              ||  !strcmp(argV[argInd], "-L"))
        {
            doList = true;
        }
        else if (!strcmp(argV[argInd], "-f")
              ||  !strcmp(argV[argInd], "-F"))
        {
            schemaFullChecking = true;
        }
        else
        {
            XERCES_STD_QUALIFIER cerr << "Unknown option '" << argV[argInd]
                << "', ignoring it\n" << XERCES_STD_QUALIFIER endl;
        }
    }

    //
    //  There should be only one and only one parameter left, and that
    //  should be the file name.
    //
    if (argInd != argC - 1)
    {
        usage();
        return 1;
    }

    XMLGrammarPool *grammarPool = 0;
    SAX2XMLReader* parser = 0;
    try
    {
        grammarPool = new XMLGrammarPoolImpl(XMLPlatformUtils::fgMemoryManager);

        parser = XMLReaderFactory::createXMLReader(XMLPlatformUtils::fgMemoryManager, grammarPool);
        parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
        parser->setFeature(XMLUni::fgXercesSchema, true);
        parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
        parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
        parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, true);
        parser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);

        SCMPrintHandler handler;
        parser->setErrorHandler(&handler);

        bool more = true;
        bool parsedOneSchemaOkay = false;
        XERCES_STD_QUALIFIER ifstream fin;

        // the input is a list file
        if (doList)
            fin.open(argV[argInd]);

        if (fin.fail()) {
            XERCES_STD_QUALIFIER cerr <<"Cannot open the list file: " << argV[argInd] << XERCES_STD_QUALIFIER endl;
            return 3;
        }

        while (more)
        {
            char fURI[1000];
            //initialize the array to zeros
            memset(fURI,0,sizeof(fURI));

            if (doList) {
                if (! fin.eof() ) {
                    fin.getline (fURI, sizeof(fURI));
                    if (!*fURI)
                        continue;
                    else {
                        xsdFile = fURI;
                        XERCES_STD_QUALIFIER cerr << "==Parsing== " << xsdFile << XERCES_STD_QUALIFIER endl;
                    }
                }
                else
                    break;
            }
            else {
                xsdFile = argV[argInd];
                more = false;
            }

            parser->loadGrammar(xsdFile, Grammar::SchemaGrammarType, true);
            if (handler.getSawErrors())
            {
                handler.resetErrors();
            }
            else
            {
                parsedOneSchemaOkay = true;
            }
        }

        if (parsedOneSchemaOkay)
        {
            XERCES_STD_QUALIFIER cout << "********** Printing out information from Schema **********" << "\n\n";
            bool updatedXSModel;
            XSModel *xsModel = grammarPool->getXSModel(updatedXSModel);
            if (xsModel)
            {
                StringList *namespaces = xsModel->getNamespaces();
                for (unsigned i = 0; i < namespaces->size(); i++) {

                    const XMLCh *nameSpace = namespaces->elementAt(i);
                    if(strcmp(StrX(nameSpace).localForm(), "http://www.w3.org/2001/XMLSchema") == 0) continue;
                    XERCES_STD_QUALIFIER cout << "Processing Namespace:   ";
                    if (nameSpace && *nameSpace)
                        XERCES_STD_QUALIFIER cout << StrX(nameSpace);
                    XERCES_STD_QUALIFIER cout << "\n============================================" << XERCES_STD_QUALIFIER endl << XERCES_STD_QUALIFIER endl;

                    processElements(xsModel->getComponentsByNamespace(XSConstants::ELEMENT_DECLARATION,
                                                                  nameSpace));
                    processTypeDefinitions(xsModel->getComponentsByNamespace(XSConstants::TYPE_DEFINITION,
                                                                         nameSpace));
                }
            }
            else
            {
                XERCES_STD_QUALIFIER cout << "No XSModel to print" << "\n\n";
            }
        }
        else
        {
            XERCES_STD_QUALIFIER cout << "Did not parse a schema document cleanly so not printing Schema for Schema XSModel information";
        }

        XERCES_STD_QUALIFIER cout << XERCES_STD_QUALIFIER endl;
    }
    catch (const OutOfMemoryException&)
    {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException during parsing: '" << xsdFile << "'\n" << XERCES_STD_QUALIFIER endl;
        errorCode = 6;
    }
    catch (const XMLException& e)
    {
        XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << xsdFile << "'\n"
        << "Exception message is:  \n"
        << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        errorCode = 4;
    }
    catch (...)
    {
        XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << xsdFile << "'\n" << XERCES_STD_QUALIFIER endl;
        errorCode = 5;
    }
    cfg_bin_struct_generator::cfg_bin_output(XERCES_STD_QUALIFIER cout);

    delete parser;
    delete grammarPool;
    XMLPlatformUtils::Terminate();

    return errorCode;
}

void printBasic(XSObject *xsObject, const char *type)
{
    XERCES_STD_QUALIFIER cout << "Name:\t\t\t";
    const XMLCh *nameSpace = xsObject->getNamespace();
    if (nameSpace && *nameSpace) {
        XERCES_STD_QUALIFIER cout << StrX(nameSpace) << ", ";
    }
    XERCES_STD_QUALIFIER cout << StrX(xsObject->getName()) << "\n";
    XERCES_STD_QUALIFIER cout << "Component Type:\t" << type << XERCES_STD_QUALIFIER endl;
}

void processElements(XSNamedMap<XSObject> *xsElements)
{
    if (!xsElements || xsElements->getLength() == 0) {
        XERCES_STD_QUALIFIER cout << "no elements\n\n"  << XERCES_STD_QUALIFIER endl;
        return;
    }
    for (XMLSize_t i=0; i < xsElements->getLength(); i++) {
        XSElementDeclaration *xsElement = (XSElementDeclaration *)xsElements->item(i);
        printBasic(xsElement, "Element");

        // Content Model
        XSTypeDefinition *xsTypeDef = xsElement->getTypeDefinition();
        XERCES_STD_QUALIFIER cout << "Content Model" << "\n";
        XERCES_STD_QUALIFIER cout << "\tType:\t";
        if (xsTypeDef->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
            XERCES_STD_QUALIFIER cout << "Simple\n";
        } else {
            XERCES_STD_QUALIFIER cout << "Complex\n";
        }
        if(xsTypeDef->getAnonymous() == true) {
            processComplexTypeDefinition((XSComplexTypeDefinition*)xsTypeDef,xsElement);
        } else {
        XERCES_STD_QUALIFIER cout << "\tName:\t"
            << StrX(xsTypeDef->getName()) << "\n";
        }

        XERCES_STD_QUALIFIER cout << "\n--------------------------------------------" << XERCES_STD_QUALIFIER endl;
    }
}

void processSimpleTypeDefinition(XSSimpleTypeDefinition * xsSimpleTypeDef)
{
    XSTypeDefinition *xsBaseTypeDef = xsSimpleTypeDef->getBaseType();
    XERCES_STD_QUALIFIER cout << "Base:\t\t\t";
    XERCES_STD_QUALIFIER cout << StrX(xsBaseTypeDef->getName()) << XERCES_STD_QUALIFIER endl;

    int facets = xsSimpleTypeDef->getDefinedFacets();
    if (facets) {
        XERCES_STD_QUALIFIER cout << "Facets:\n";

        if (facets & XSSimpleTypeDefinition::FACET_LENGTH)
                XERCES_STD_QUALIFIER cout << "\tLength:\t\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_LENGTH)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MINLENGTH)
                XERCES_STD_QUALIFIER cout << "\tMinLength:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MINLENGTH)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXLENGTH)
                XERCES_STD_QUALIFIER cout << "\tMaxLength:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXLENGTH)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_PATTERN) {
            StringList *lexicalPatterns = xsSimpleTypeDef->getLexicalPattern();
            if (lexicalPatterns && lexicalPatterns->size()) {
                XERCES_STD_QUALIFIER cout << "\tPattern:\t\t";
                for (unsigned i = 0; i < lexicalPatterns->size(); i++) {
                    XERCES_STD_QUALIFIER cout << StrX(lexicalPatterns->elementAt(i));
                }
                XERCES_STD_QUALIFIER cout << XERCES_STD_QUALIFIER endl;
            }
        }
        if (facets & XSSimpleTypeDefinition::FACET_WHITESPACE)
                XERCES_STD_QUALIFIER cout << "\tWhitespace:\t\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_WHITESPACE)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXINCLUSIVE)
                XERCES_STD_QUALIFIER cout << "\tMaxInclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXINCLUSIVE)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXEXCLUSIVE)
                XERCES_STD_QUALIFIER cout << "\tMaxExclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXEXCLUSIVE)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MINEXCLUSIVE)
                XERCES_STD_QUALIFIER cout << "\tMinExclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MINEXCLUSIVE)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_MININCLUSIVE)
                XERCES_STD_QUALIFIER cout << "\tMinInclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MININCLUSIVE)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_TOTALDIGITS)
                XERCES_STD_QUALIFIER cout << "\tTotalDigits:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_TOTALDIGITS)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_FRACTIONDIGITS)
                XERCES_STD_QUALIFIER cout << "\tFractionDigits:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_FRACTIONDIGITS)) << XERCES_STD_QUALIFIER endl;
        if (facets & XSSimpleTypeDefinition::FACET_ENUMERATION) {
            StringList *lexicalEnums = xsSimpleTypeDef->getLexicalEnumeration();
            if (lexicalEnums && lexicalEnums->size()) {
                XERCES_STD_QUALIFIER cout << "\tEnumeration:\n";
                for (unsigned i = 0; i < lexicalEnums->size(); i++) {
                    XERCES_STD_QUALIFIER cout << "\t\t\t" << StrX(lexicalEnums->elementAt(i)) << "\n";
                }
                XERCES_STD_QUALIFIER cout << XERCES_STD_QUALIFIER endl;
            }
        }
    }
}

void printCompositorTypeConnector(XSModelGroup::COMPOSITOR_TYPE type)
{
    switch (type) {
        case XSModelGroup::COMPOSITOR_SEQUENCE :
            XERCES_STD_QUALIFIER cout << ",";
            break;
        case XSModelGroup::COMPOSITOR_CHOICE :
            XERCES_STD_QUALIFIER cout << "|";
            break;
        case XSModelGroup::COMPOSITOR_ALL :
            XERCES_STD_QUALIFIER cout << "*";
            break;
    }
}

void processParticle(XSParticle *xsParticle,cfg_bin_struct_generator *pcfg_bin_generator,ostream &stream)
{
    if (!xsParticle) {
        XERCES_STD_QUALIFIER cout << "xsParticle is NULL";
        return;
    }
    XSParticle::TERM_TYPE termType = xsParticle->getTermType();
    if (termType == XSParticle::TERM_ELEMENT) {
        XSElementDeclaration *xsElement = xsParticle->getElementTerm();
        if(pcfg_bin_generator == 0){
            XERCES_STD_QUALIFIER cout << StrX(xsElement->getName()) << "--" << StrX(xsElement->getTypeDefinition()->getName()) ;
        }else{
            pcfg_bin_generator->generate_member(StrX(xsElement->getTypeDefinition()->getName()).localForm(),StrX(xsElement->getName()).localForm(),stream);
        }
    } else if (termType == XSParticle::TERM_MODELGROUP) {
        XSModelGroup *xsModelGroup = xsParticle->getModelGroupTerm();
        XSModelGroup::COMPOSITOR_TYPE compositorType = xsModelGroup->getCompositor();
        XSParticleList *xsParticleList = xsModelGroup->getParticles();
        if(pcfg_bin_generator == 0){
            XERCES_STD_QUALIFIER cout << "(";

            for (unsigned i = 0; i < xsParticleList->size()-1; i++) {
                processParticle(xsParticleList->elementAt(i));
                printCompositorTypeConnector(compositorType);
            }
            processParticle(xsParticleList->elementAt(xsParticleList->size()-1));
            XERCES_STD_QUALIFIER cout << ")";
        }else{
            if(compositorType != XSModelGroup::COMPOSITOR_SEQUENCE){
                XERCES_STD_QUALIFIER cout << "generate structure member not sequence"<< XERCES_STD_QUALIFIER endl;
                return;
            }
            for (unsigned i = 0; i < xsParticleList->size(); i++) {
                processParticle(xsParticleList->elementAt(i),pcfg_bin_generator,stream);
            }
        }
    } else if (termType == XSParticle::TERM_WILDCARD) {
        XERCES_STD_QUALIFIER cout << "* (wildcard)";
    }
}

void processComplexTypeDefinition(XSComplexTypeDefinition *xsComplexTypeDef,XSElementDeclaration*xsElement)
{
    XSTypeDefinition *xsBaseTypeDef = xsComplexTypeDef->getBaseType();
    if (xsBaseTypeDef) {
        XERCES_STD_QUALIFIER cout << "Base:\t\t\t";
        XERCES_STD_QUALIFIER cout << StrX(xsBaseTypeDef->getName()) << "\n";
    }

    XERCES_STD_QUALIFIER cout << "Content Model:\t";
    XSComplexTypeDefinition::CONTENT_TYPE contentType = xsComplexTypeDef->getContentType();
    if (contentType == XSComplexTypeDefinition::CONTENTTYPE_ELEMENT ||
        contentType == XSComplexTypeDefinition::CONTENTTYPE_MIXED) {
        processParticle(xsComplexTypeDef->getParticle());
        XERCES_STD_QUALIFIER cout << XERCES_STD_QUALIFIER endl;
    }
    cfg_bin_struct_generator cfgBinStructGen(xsComplexTypeDef,xsElement);
    cfg_bin_struct_generator::add_to_type_defines(&cfgBinStructGen);
}

void processTypeDefinitions(XSNamedMap<XSObject> *xsTypeDefs)
{
    if (!xsTypeDefs) return;

    for (XMLSize_t i=0; i < xsTypeDefs->getLength(); i++) {
        XSTypeDefinition *xsTypeDef = (XSTypeDefinition *)xsTypeDefs->item(i);

        printBasic(xsTypeDef, "Type Definition");

        // Content Model
        XERCES_STD_QUALIFIER cout << "Category:\t";
        if (xsTypeDef->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
            XERCES_STD_QUALIFIER cout << "\tSimple\n";
            processSimpleTypeDefinition((XSSimpleTypeDefinition *)xsTypeDef);
        } else {
            XERCES_STD_QUALIFIER cout << "\tComplex\n";
            processComplexTypeDefinition((XSComplexTypeDefinition *)xsTypeDef);
        }

        XERCES_STD_QUALIFIER cout << "\n--------------------------------------------" << XERCES_STD_QUALIFIER endl;
    }
}
