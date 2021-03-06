#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"

namespace acid
{
	class ACID_EXPORT RendererGizmos :
		public RenderPipeline
	{
	public:
		explicit RendererGizmos(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		PipelineGraphics m_pipeline;
		UniformHandler m_uniformScene;
	};
}
