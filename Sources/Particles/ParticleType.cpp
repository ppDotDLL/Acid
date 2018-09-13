﻿#include "ParticleType.hpp"

#include "Resources/Resources.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Helpers/String.hpp"

namespace acid
{
	const uint32_t ParticleType::MAX_TYPE_INSTANCES = 512;

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale)
	{
		auto resource = Resources::Get()->Get(ToFilename(texture, numberOfRows, colourOffset, lifeLength, scale));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ParticleType>(resource);
		}

		auto result = std::make_shared<ParticleType>(texture, numberOfRows, colourOffset, lifeLength, scale);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::string &data)
	{
		auto split = String::Split(data, "_");
		auto texture = Texture::Resource(split[1]);
		uint32_t numberOfRows = String::From<uint32_t>(split[2]);
		Colour colourOffset = Colour(split[3]);
		float lifeLength = String::From<float>(split[4]);
		float scale = String::From<float>(split[5]);
		return Resource(texture, numberOfRows, colourOffset, lifeLength, scale);
	}

	ParticleType::ParticleType(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale) :
		m_filename(ToFilename(texture, numberOfRows, colourOffset, lifeLength, scale)),
		m_texture(texture),
		m_model(ModelRectangle::Resource(-0.5f, 0.5f)),
		m_numberOfRows(numberOfRows),
		m_colourOffset(colourOffset),
		m_lifeLength(lifeLength),
		m_scale(scale),
		m_instanceBuffer(InstanceBuffer(sizeof(ParticleData) * MAX_TYPE_INSTANCES)),
		m_descriptorSet(DescriptorsHandler())
	{
	}

	void ParticleType::Decode(const Metadata &metadata)
	{
		TrySetTexture(metadata.GetChild<std::string>("Texture"));
		m_numberOfRows = metadata.GetChild<uint32_t>("Number Of Rows");
		m_colourOffset = metadata.GetChild<Colour>("Colour Offset");
		m_lifeLength = metadata.GetChild<float>("Life Length");
		m_scale = metadata.GetChild<float>("Scale");
		m_filename = ToFilename(m_texture, m_numberOfRows, m_colourOffset, m_lifeLength, m_scale);
	}

	void ParticleType::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Texture", m_texture == nullptr ? "" : m_texture->GetFilename());
		metadata.SetChild<uint32_t>("Number Of Rows", m_numberOfRows);
		metadata.SetChild<Colour>("Colour Offset", m_colourOffset);
		metadata.SetChild<float>("Life Length", m_lifeLength);
		metadata.SetChild<float>("Scale", m_scale);
	}

	bool ParticleType::CmdRender(const CommandBuffer &commandBuffer, Pipeline &pipeline, UniformHandler &uniformScene, const std::vector<ParticleData> &instanceData)
	{
		m_instanceBuffer.Update(instanceData.data());

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("samplerColour", m_texture);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer);

		VkBuffer vertexBuffers[] = {m_model->GetVertexBuffer()->GetBuffer(), m_instanceBuffer.GetBuffer()};
		VkDeviceSize offsets[] = {0, 0};
		vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 2, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer.GetCommandBuffer(), m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexBuffer()->GetIndexType());
		vkCmdDrawIndexed(commandBuffer.GetCommandBuffer(), m_model->GetIndexBuffer()->GetIndexCount(), MAX_TYPE_INSTANCES, 0, 0, 0);
		return true;
	}

	std::string ParticleType::ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale)
	{
		std::stringstream result;
		result << "ParticleType_" << (texture == nullptr ? "nullptr" : texture->GetFilename()) << "_" << numberOfRows << "_" << colourOffset.GetHex() << "_" << lifeLength << "_" << scale;
		return result.str();
	}
}
