#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A bounce driver that uses a sine wave.
	/// </summary>
	template<typename T>
	class DriverBounce :
		public IDriver<T>
	{
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="start"> The start value. </param>
		/// <param name="end"> The end value. </param>
		/// <param name="length"> The length between two waves. </param>
		DriverBounce(const T &start, const T &end, const Time &length) :
			IDriver<T>(length),
			m_start(start),
			m_end(end)
		{
		}

		/// <summary>
		/// Gets the start time.
		/// </summary>
		/// <returns> The start time. </returns>
		const T &GetStart() const { return m_start; }

		/// <summary>
		/// Sets the start time.
		/// </summary>
		/// <param name="start"> The new start time. </param>
		void SetStart(const T &start) { m_start = start; }

		/// <summary>
		/// Gets the end time.
		/// </summary>
		/// <returns> The ebd time. </returns>
		const T &GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
		void SetEnd(const T &end) { m_end = end; }
	protected:
		T Calculate(const float &factor) override
		{
			float value = 0.5f + std::sin(Maths::Pi * 2.0f * factor) * 0.5f;

			if (IDriver<T>::m_actualTime > IDriver<T>::GetLength() / 2.0f)
			{
				value = 0.0f;
			}

			return m_start + value * (m_end - m_start);
		}

	private:
		T m_start;
		T m_end;
	};
}
