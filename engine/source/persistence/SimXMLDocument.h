//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Console implementation of STL map.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XMLDOC_H_
#define _XMLDOC_H_

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

#ifndef _VECTOR_H_
#include "collection/vector.h"
#endif // _VECTOR_H_

#ifndef TINYXML_INCLUDED
#include "persistence/tinyXML/tinyxml.h"
#endif

class SimXMLDocument: public SimObject
{
   // This typedef is required for tie ins with the script language.
   // --------------------------------------------------------------------------
    protected:
      typedef SimObject Parent;
   // --------------------------------------------------------------------------

   public:
      SimXMLDocument();
      ~SimXMLDocument();

      // These are overloaded functions from SimObject that we handle for
      // tie in to the script language.  The .cc file has more in depth
      // comments on these.
      //-----------------------------------------------------------------------
      bool processArguments(S32 argc, const char** argv);
      bool onAdd();
      void onRemove();
      static void initPersistFields();
      
      // Set this to default state at construction.
      void reset(void);
      
      // Read / write / parse XML.
      S32 loadFile(const char* rFileName);
      S32 saveFile(const char* rFileName);
      S32 parse(const char* rText);
      
      // Clear XML document.
      void clear(void);
      
      // Get error description if it exists.
      const char* getErrorDesc(void) const;
      // Clear previously set error.
      void clearError(void);

      // Push first/last child element onto element stack.
      bool pushFirstChildElement(const char* rName);
      // Convert top stack element into sibling with given name.
      bool nextSiblingElement(const char* rName);
      // push child element at index onto stack.
      bool pushChildElement(S32 index);
      // Get element value
      const char* elementValue();
      
      // Pop last element off of stack.
      void popElement(void);

      // Get attribute from top element on element stack.
      const char* attribute(const char* rAttribute);

      // Does the attribute exist in the current element
      bool attributeExists(const char* rAttribute);

      // Obtain the name of the current element's first or last attribute
      const char* firstAttribute();
      const char* lastAttribute();

      // Move through the current element's attributes to obtain their names
      const char* nextAttribute();
      const char* prevAttribute();

      // Set attribute of top element on element stack.
      void setAttribute(const char* rAttribute, const char* rVal);
      // Set attributes of a simObject on top element of the stack.
      void setObjectAttributes(const char* objectID);
      
      // Remove attribute with given name from top element on stack.
      void removeAttribute(const char* rAttribute);
      
      // Create a new element and push it onto stack as a new level.
      void pushNewElement(const char* rName);
      // Create a new element and push it onto stack on current level.
      void addNewElement(const char* rName);
      // Write XML declaration to current level.
      void addHeader(void);
      // Write a XML comment to the current level.
      void addComment(const char* comment);
      // Read a comment from the current level at the specified index.
      const char* readComment( S32 index );
      // Write text to the current level.
      void addText(const char* text);
      // Retrieve text from the current level.
      const char* getText();
      // Remove Text
      void removeText();
      // Write data to the current level.
      void addData(const char* text);
      // Retrieve data from the current level.
      const char* getData();
      
   private:
      // Document.
      TiXmlDocument* m_qDocument;
      // Stack of nodes.
      Vector<TiXmlElement*> m_paNode;
      // The current attribute
      TiXmlAttribute* m_CurrentAttribute;

   public:
      DECLARE_CONOBJECT(SimXMLDocument);
};

#endif // _XMLDOC_H_
////EOF/////////////////////////////////////////////////////////////////////////
