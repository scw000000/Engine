#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: SceneNodes.h
////////////////////////////////////////////////////////////////////////////////

//typedef unsigned int ActorId;

// TODO: implement rest graphic class such as material & alpha
class SceneNodeProperties
   {
   friend class SceneNode;
   public:
      ActorId GetActorId( void ) const { return m_ActorId; } // Is the return type appropriate?
      const Mat4x4& GetToWorld( void ) const { return m_ToWorld; }
      const Mat4x4& GetFromWorld( void ) const { return m_FromWorld; }
      void Transform( Mat4x4* const toWorld, Mat4x4* const fromWorld ) const;

      const char* Name( void ) const { return m_Name.c_str(); }

      //bool HasAlpha( void ) const {};
      // virtual float Alpha( void ) const {}

      //RenderPass Renderpass( void ) const {}
      float GetRadius( void ) const { return m_Radius; }
      
      //const Material& GetMaterial( void ) const {}

   protected:
      ActorId m_ActorId;
      std::string m_Name;
      Mat4x4 m_ToWorld;
      Mat4x4 m_FromWorld;
      float m_Radius;
      //RenderPass m_RemderPass;
      //Material m_Material;
      //AlphaType m_AlphaType;

   private:

   };

class SceneNode : public ISceneNode
{
	friend class Scene;

protected:
	SceneNodeList			m_Children;
	SceneNode				*m_pParent;
	SceneNodeProperties		m_Props;
	WeakBaseRenderComponentPtr	m_RenderComponent;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Mat4x4 *to, const Mat4x4 *from=NULL);

	virtual ~SceneNode();

	virtual const SceneNodeProperties* const VGet() const { return &m_Props; }

	virtual void VSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld=NULL);

	virtual HRESULT VOnRestore(Scene *pScene);
	virtual HRESULT VOnUpdate(Scene *, DWORD const elapsedMs);

	virtual HRESULT VPreRender(Scene *pScene);
	virtual bool VIsVisible(Scene *pScene) const;
	virtual HRESULT VRender(Scene *pScene) { return S_OK; } 
	virtual HRESULT VRenderChildren(Scene *pScene);
	virtual HRESULT VPostRender(Scene *pScene);

	virtual bool VAddChild(shared_ptr<ISceneNode> kid);
	virtual bool VRemoveChild(ActorId id);
	virtual HRESULT VOnLostDevice(Scene *pScene);
	virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast);

	void SetAlpha(float alpha);
	float GetAlpha() const { return m_Props.Alpha(); }

	Vec3 GetPosition() const { return m_Props.m_ToWorld.GetPosition(); }
	void SetPosition(const Vec3 &pos) { m_Props.m_ToWorld.SetPosition(pos); }

	const Vec3 GetWorldPosition() const;					// [mrmike] added post-press to respect ancestor's position 

	Vec3 GetDirection() const { return m_Props.m_ToWorld.GetDirection(); }

	void SetRadius(const float radius) { m_Props.m_Radius = radius; }
	void SetMaterial(const Material &mat) { m_Props.m_Material = mat; }
};
