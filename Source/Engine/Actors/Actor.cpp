////////////////////////////////////////////////////////////////////////////////
// Filename: Actor.cpp
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "EngineStd.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "ActorFactory.h"

Actor::Actor(ActorId id)
   {
   m_id = id;
   m_type = "Unknown";

	// [mrmike] added post press - this is an editor helper
	m_resource = "Unknown";
   }

// The actor should call Destroy() before calling destructor
Actor::~Actor(void)
   {
   ENG_LOG("Actor", std::string("Destroying Actor ") + ToStr(m_id));
   ENG_ASSERT(m_components.empty());  // [rez] if this assert fires, the actor was destroyed without calling Actor::Destroy()
   }

bool Actor::Init(TiXmlElement* pData)
   {
   ENG_LOG("Actor", std::string("Initializing Actor ") + ToStr( m_id ) );

	m_type = pData->Attribute("type");
	m_resource = pData->Attribute("resource");
   return true;
   }

void Actor::PostInit(void)
   {
   for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
      {
      it->second->VPostInit();
      }
   }

// Once these pointers are deleted from map, it will delete itself automatically
void Actor::Destroy(void)
   {
   // Call map::clear
   m_components.clear();
   }

void Actor::Update(int deltaMs)
   {
   for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
      {
      it->second->VUpdate(deltaMs);
      }
   }

/* 
Not used yet ( for editor )
std::string Actor::ToXML()
{
    TiXmlDocument outDoc;

    // Actor element
    TiXmlElement* pActorElement = GCC_NEW TiXmlElement("Actor");
    pActorElement->SetAttribute("type", m_type.c_str());
	pActorElement->SetAttribute("resource", m_resource.c_str());

    // components
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        StrongActorComponentPtr pComponent = it->second;
        TiXmlElement* pComponentElement = pComponent->VGenerateXml();
        pActorElement->LinkEndChild(pComponentElement);
    }

    outDoc.LinkEndChild(pActorElement);
	TiXmlPrinter printer;
	outDoc.Accept(&printer);

	return printer.CStr();
}*/


void Actor::AddComponent(StrongActorComponentPtr pComponent)
   {
   std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair( pComponent->VGetId(), pComponent) );
   ENG_ASSERT(success.second);
   }