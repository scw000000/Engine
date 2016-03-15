////////////////////////////////////////////////////////////////////////////////
// Filename: Initialization.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "Initialization.h"

bool CheckForJoystick(std::string hWnd)
   {
   return false;
   }

EngineOptions::EngineOptions()
   {
   // set all the options to decent default valu
	m_Renderer = "Direct3D 9";
	m_runFullSpeed = false;
	m_ScreenSize = Point(1024,768);

	m_pDoc = NULL;
   }

void EngineOptions::Init(const char* xmlFileName)
   {
	// read the XML file
	// if needed, override the XML file with options passed in on the command line.

	m_pDoc = new TiXmlDocument(xmlFileName);
	if (m_pDoc && m_pDoc->LoadFile())
	   {
      TiXmlElement *pRoot = m_pDoc->RootElement();
      if (!pRoot)
         {
         return;
         }
            
      // Loop through each child element and load the component
      TiXmlElement* pNode = NULL;
		pNode = pRoot->FirstChildElement("Graphics"); 
		if (pNode)
		   {
			std::string attribute;
			attribute = pNode->Attribute("renderer");
			if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
			   {
				ENG_ASSERT(0 && "Bad Renderer setting in Graphics options.");
			   }
		   else
			   {
				m_Renderer = attribute;
			   }

		   if (pNode->Attribute("width"))
			   {
			   m_ScreenSize.x = atoi(pNode->Attribute("width"));
			   if (m_ScreenSize.x < 800)
               {
               m_ScreenSize.x = 800;
               }
			   }  

			if (pNode->Attribute("height"))
			   {
				m_ScreenSize.y = atoi(pNode->Attribute("height"));
				   if (m_ScreenSize.y < 600) m_ScreenSize.y = 600;
			   }

			if (pNode->Attribute("runfullspeed"))
			   {
				attribute = pNode->Attribute("runfullspeed");
				m_runFullSpeed = (attribute == "yes") ? true : false;
			   }
		   }

	   }
   }


/*()
   {
   m_ScreenWidth = 800;
   m_ScreenHeight = 600;
   }*/