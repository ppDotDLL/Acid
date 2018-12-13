#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCapsule::ColliderCapsule(const float &radius, const float &height, const Transform &localTransform) :
		Collider(localTransform),
		m_shape(std::make_unique<btCapsuleShape>(radius, height)),
		m_radius(radius),
		m_height(height)
	{
		auto gizmoTypeSquare = GizmoType::Resource(Model::Resource("Gizmos/Capsule.obj"), 3.0f);
		m_gizmo = Gizmos::Get()->AddGizmo(new Gizmo(gizmoTypeSquare, localTransform, Colour::GREEN));
	}

	ColliderCapsule::~ColliderCapsule()
	{
		Gizmos::Get()->RemoveGizmo(m_gizmo);
	}

	void ColliderCapsule::Start()
	{
	}

	void ColliderCapsule::Update()
	{
		m_gizmo->SetTransform(GetGameObject()->GetTransform()); //  * m_localTransform
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height, m_radius)); // TODO
	}

	void ColliderCapsule::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_radius = metadata.GetChild<float>("Radius");
		m_height = metadata.GetChild<float>("Height");
	}

	void ColliderCapsule::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCapsule::GetCollisionShape() const
	{
		return m_shape.get();
	}
}
