﻿#pragma once

#include <string>
#include "Maths/Colour.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Resources/IResource.hpp"
#include "Textures/Texture.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	struct ParticleData
	{
		Matrix4 mvp;
		Colour colourOffset;
		Vector4 offsets;
		Vector3 blend;
	};

	/// <summary>
	/// A definition for what a particle should act and look like.
	/// </summary>
	class ACID_EXPORT ParticleType :
		public IResource
	{
	private:
		std::string m_filename;
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Model> m_model;
		uint32_t m_numberOfRows;
		Colour m_colourOffset;
		float m_lifeLength;
		float m_scale;

		InstanceBuffer m_instanceBuffer;
		DescriptorsHandler m_descriptorSet;
	public:
		static const uint32_t MAX_TYPE_INSTANCES;

		/// <summary>
		/// Will find an existing particle type with the same filename, or create a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		static std::shared_ptr<ParticleType> Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale);

		/// <summary>
		/// Will find an existing particle type with the same filename, or create a new particle type.
		/// </summary>
		/// <param name="data"> The combined data for the particle type. </param>
		static std::shared_ptr<ParticleType> Resource(const std::string &data);

		/// <summary>
		/// Creates a new particle type.
		/// </summary>
		/// <param name="texture"> The particles texture. </param>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		/// <param name="colourOffset"> The particles texture colour offset. </param>
		/// <param name="lifeLength"> The averaged life length for the particle. </param>
		/// <param name="scale"> The averaged scale for the particle. </param>
		ParticleType(const std::shared_ptr<Texture> &texture = nullptr, const uint32_t &numberOfRows = 1, const Colour &colourOffset = Colour::BLACK, const float &lifeLength = 10.0f, const float &scale = 1.0f);

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		bool CmdRender(const CommandBuffer &commandBuffer, Pipeline &pipeline, UniformHandler &uniformScene, const std::vector<ParticleData> &instanceData);

		std::string GetFilename() override { return m_filename; }

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		void SetTexture(const std::shared_ptr<Texture> &texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}

		uint32_t GetNumberOfRows() const { return m_numberOfRows; }

		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		Colour GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) { m_colourOffset = colourOffset; }

		float GetLifeLength() const { return m_lifeLength; }

		void SetLifeLength(const float &lifeLength) { m_lifeLength = lifeLength; }

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	private:
		static std::string ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale);
	};
}
