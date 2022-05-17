#include "MiniginPCH.h"
#include "PhysicsDebugDraw.h"

#include "Renderer.h"

PhysicsDebugDraw::PhysicsDebugDraw()
	: m_pRenderer{&dae::Renderer::GetInstance()}
{
}

void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	m_pRenderer->RenderPolygon(vertices, vertexCount, glm::vec4{ color.r, color.g, color.b, color.a });
}

void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	m_pRenderer->RenderPolygon(vertices, vertexCount, glm::vec4{ color.r, color.g, color.b, color.a });
}

void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	m_pRenderer->RenderCircle(center, radius, glm::vec4{ color.r, color.g, color.b, color.a });
}

void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& /*axis*/, const b2Color& color)
{
	m_pRenderer->RenderCircle(center, radius, glm::vec4{ color.r, color.g, color.b, color.a });
}

void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_pRenderer->RenderLine(p1, p2, glm::vec4{ color.r, color.g, color.b, color.a });
}

void PhysicsDebugDraw::DrawTransform(const b2Transform& /*xf*/)
{
}

void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	m_pRenderer->RenderCircle(p, size, glm::vec4{ color.r, color.g, color.b, color.a });
}