#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT VertexModelData
	{
	public:
		VertexModelData(const uint32_t &index, const Vector3 &position);

		const uint32_t &GetIndex() const { return m_index; }

		const std::optional<uint32_t> &GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const std::optional<uint32_t> &uvIndex) { m_uvIndex = uvIndex; }

		const std::optional<uint32_t> &GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const std::optional<uint32_t> &normalIndex) { m_normalIndex = normalIndex; }

		VertexModelData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexModelData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		const Vector3 &GetPosition() const { return m_position; }

		const float &GetLength() const { return m_length; }

		void AddTangent(const Vector3 &tangent);

		void AverageTangents();

		const Vector3 &GetAverageTangent() const { return m_averagedTangent; }

		bool IsSet() const;

		bool HasSameUvAndNormal(const std::optional<uint32_t> &uvIndexOther, const std::optional<uint32_t> &normalIndexOther) const;
	private:
		uint32_t m_index;
		std::optional<uint32_t> m_uvIndex;
		std::optional<uint32_t> m_normalIndex;
		VertexModelData *m_duplicateVertex;
		Vector3 m_position;
		float m_length;
		std::vector<Vector3> m_tangents;
		Vector3 m_averagedTangent;
	};
}
