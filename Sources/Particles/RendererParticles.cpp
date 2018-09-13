#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"}, {VertexModel::GetVertexInput(), GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {})))
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		auto particles = Particles::Get()->GetParticles();

		m_pipeline.BindPipeline(commandBuffer);

		for (auto &[type, particles] : particles)
		{
			auto instanceData = std::vector<ParticleData>();
			instanceData.resize(ParticleType::MAX_TYPE_INSTANCES);
			uint32_t i = 0;

			for (auto &particle : particles)
			{
				if (!camera.GetViewFrustum().SphereInFrustum(particle.GetPosition(), particle.GetScale()))
				{
					continue;
				}

				instanceData[i] = GetInstanceData(particle, camera.GetViewMatrix());
				i++;

				if (i > instanceData.size())
				{
					break;
				}
			}

			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene, instanceData);
		}
	}

	ParticleData RendererParticles::GetInstanceData(Particle &particle, const Matrix4 &viewMatrix)
	{
		ParticleData instanceData = {};

		Matrix4 modelMatrix = Matrix4().Translate(particle.GetPosition());
		modelMatrix[0][0] = viewMatrix[0][0];
		modelMatrix[0][1] = viewMatrix[1][0];
		modelMatrix[0][2] = viewMatrix[2][0];
		modelMatrix[1][0] = viewMatrix[0][1];
		modelMatrix[1][1] = viewMatrix[1][1];
		modelMatrix[1][2] = viewMatrix[2][1];
		modelMatrix[2][0] = viewMatrix[0][2];
		modelMatrix[2][1] = viewMatrix[1][2];
		modelMatrix[2][2] = viewMatrix[2][2];
		modelMatrix = modelMatrix.Rotate(Maths::Radians(particle.GetRotation()), Vector3::FRONT);
		modelMatrix = modelMatrix.Scale(Vector3(particle.GetScale(), particle.GetScale(), particle.GetScale()));
		instanceData.mvp = modelMatrix;

		instanceData.colourOffset = particle.GetParticleType()->GetColourOffset();

		Vector4 offsets = Vector4();
		offsets.m_x = particle.GetTextureOffset1().m_x;
		offsets.m_y = particle.GetTextureOffset1().m_y;
		offsets.m_z = particle.GetTextureOffset2().m_x;
		offsets.m_w = particle.GetTextureOffset2().m_y;
		instanceData.offsets = offsets;

		Vector3 blend = Vector3();
		blend.m_x = particle.GetTextureBlendFactor();
		blend.m_y = particle.GetTransparency();
		blend.m_z = static_cast<float>(particle.GetParticleType()->GetNumberOfRows());
		instanceData.blend = blend;

		return instanceData;
	}

	VertexInput RendererParticles::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The input input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(ParticleData);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

		// MVP row 1 attribute.
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(7);
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ParticleData, mvp.m_rows[0]);

		// MVP row 2 attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(ParticleData, mvp.m_rows[1]);

		// MVP row 3 attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(ParticleData, mvp.m_rows[2]);

		// MVP row 4 attribute.
		attributeDescriptions[3].binding = binding;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(ParticleData, mvp.m_rows[3]);

		// Colour offset attribute.
		attributeDescriptions[4].binding = binding;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[4].offset = offsetof(ParticleData, colourOffset);

		// UV1,UV2 offsets attribute.
		attributeDescriptions[5].binding = binding;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(ParticleData, offsets);

		// Blend,transparency,rows attribute.
		attributeDescriptions[6].binding = binding;
		attributeDescriptions[6].location = 6;
		attributeDescriptions[6].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[6].offset = offsetof(ParticleData, blend);

		return VertexInput(binding, bindingDescriptions, attributeDescriptions);
	}
}
