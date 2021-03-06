#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "RenderPipeline.hpp"

namespace acid
{
	/// <summary>
	/// A class that contains and manages renderers registered to a render manager.
	/// </summary>
	class ACID_EXPORT RendererContainer :
		public NonCopyable
	{
	public:
		RendererContainer();

		const std::map<Pipeline::Stage, std::vector<std::unique_ptr<RenderPipeline>>> &GetStages() const { return m_stages; }

		void Clear() { m_stages.clear(); }

		/// <summary>
		/// Gets a renderer instance by type from this register.
		/// </summary>
		/// <param name="T"> The renderer type to find. </param>
		/// <param name="allowDisabled"> If disabled renderers will be returned. </param>
		/// <returns> The found renderer. </returns>
		template<typename T>
		T *Get(const bool &allowDisabled = false) const
		{
			T *alternative = nullptr;

			for (const auto &[key, renderers] : m_stages)
			{
				for (const auto &renderer : renderers)
				{
					auto casted = dynamic_cast<T *>(renderer.get());

					if (casted != nullptr)
					{
						if (!allowDisabled && !casted->IsEnabled())
						{
							alternative = casted;
							continue;
						}

						return casted;
					}
				}
			}

			return alternative;
		}

		/// <summary>
		/// Adds a renderer to this register.
		/// </summary>
		/// <param name="renderer"> The renderer to add. </param>
		/// <returns> The added renderer. </returns>
		RenderPipeline *Add(RenderPipeline *renderer);

		/// <summary>
		/// Creates a renderer by type to be added this register.
		/// </summary>
		/// <param name="T"> The type of renderer to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added renderer. </returns>
		template<typename T, typename... Args>
		T *Add(Args &&... args)
		{
			auto created = new T(std::forward<Args>(args)...);
			Add(created);
			return created;
		}

		/// <summary>
		/// Removes a renderer from this register.
		/// </summary>
		/// <param name="renderer"> The renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		void Remove(RenderPipeline *renderer);

		/// <summary>
		/// Removes a renderer by type from this register.
		/// </summary>
		/// <param name="T"> The type of renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		template<typename T>
		void Remove()
		{
			for (auto it = m_stages.begin(); it != m_stages.end(); ++it)
			{
				for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it)
				{
					auto casted = dynamic_cast<T *>((*it2).get());

					if (casted != nullptr)
					{
						(*it).second.erase(it2);

						if ((*it).second.empty())
						{
							m_stages.erase(it);
						}
					}
				}
			}
		}

	private:
		std::map<Pipeline::Stage, std::vector<std::unique_ptr<RenderPipeline>>> m_stages;
	};
}
