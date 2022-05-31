#ifndef __ANIMATOR_HPP__
#define __ANIMATOR_HPP__

#include <omniax/utils/Types.hpp>
#include <omniax/utils/Logger.hpp>
#include <omniax/graphics/Color.hpp>

#include <vector>

namespace ox
{
	template<typename T>
	class FrameInterpolator
	{
		public:
			inline FrameInterpolator(void)
			{
				m_endFrame = 10;
				m_loop = false;
				m_backwards = false;
				m_back = false;
				m_currentFrame = -1;

				m_valid = false;
			}
			inline FrameInterpolator(T startValue, T endValue, uint32_t length_in_frames = 60, bool loop = false, bool backwards = false)
			{
				if constexpr(std::is_same_v<T, float>) { }
				if constexpr(std::is_same_v<T, int64_t>) { }
				if constexpr(std::is_same_v<T, int32_t>) { }
				if constexpr(std::is_same_v<T, int16_t>) { }
				if constexpr(std::is_same_v<T, int8_t>) { }
				else if constexpr(std::is_same_v<T, ox::Color>) { }
				else
				{
					OX_WARN("Invalid type in ox::FrameInterpolator."); //TODO: Add error (warning)
					return;
				}
				set(startValue, endValue, length_in_frames, loop, backwards);
			}
			inline void set(T startValue, T endValue, uint32_t length_in_frames = 60, bool loop = false, bool backwards = false)
			{
				m_startValue = startValue;
				m_endValue = endValue;
				m_endFrame = length_in_frames;
				m_interpolated = m_startValue;
				m_loop = loop;
				m_backwards = backwards;
				m_back = false;
				m_currentFrame = -1;

				m_valid = true;
			}
			inline void reset(void)
			{
				m_interpolated = m_startValue;
				m_back = false;
				m_currentFrame = -1;
				m_done = false;
			}
			inline bool isDone(void) const { return !m_loop && m_done; }
			inline bool isLooping(void) const { return m_loop; }
			inline T get(void) { return m_interpolated; }
			void update(void)
			{
				if (!m_valid || isDone()) return;
				if (m_back) m_currentFrame--;
				else m_currentFrame++;
				if (m_currentFrame >= m_endFrame)
				{
					if (!m_loop)
					{
						m_done = true;
						return;
					}
					else if (m_backwards)
					{
						if (m_back) m_currentFrame = 1;
						else m_currentFrame = m_endFrame - 2;
						m_back = !m_back;
					}
					else m_currentFrame = 0;
				}
				float t = (float)m_currentFrame / (float)(m_endFrame - 1);
				if constexpr(std::is_same_v<T, float>)
				{
					m_interpolated = std::lerp(m_startValue, m_endValue, t);
				}
				else if constexpr(std::is_same_v<T, int64_t>)
				{
					m_interpolated = (int)std::round(std::lerp(m_startValue, m_endValue, t));
				}
				else if constexpr(std::is_same_v<T, int32_t>)
				{
					m_interpolated = (int)std::round(std::lerp(m_startValue, m_endValue, t));
				}
				else if constexpr(std::is_same_v<T, int16_t>)
				{
					m_interpolated = (int)std::round(std::lerp(m_startValue, m_endValue, t));
				}
				else if constexpr(std::is_same_v<T, int8_t>)
				{
					m_interpolated = (int)std::round(std::lerp(m_startValue, m_endValue, t));
				}
				else if constexpr(std::is_same_v<T, ox::Color>)
				{
					uint8_t r = (int)std::round(std::lerp(m_startValue.r, m_endValue.r, t));
					uint8_t g = (int)std::round(std::lerp(m_startValue.g, m_endValue.g, t));
					uint8_t b = (int)std::round(std::lerp(m_startValue.b, m_endValue.b, t));
					uint8_t a = (int)std::round(std::lerp(m_startValue.a, m_endValue.a, t));

					m_interpolated.set(r, g, b, a);
				}
			}

		private:
			T m_startValue;
			T m_endValue;
			T m_interpolated;
			bool m_valid { false };
			bool m_loop { false };
			bool m_backwards { false };
			bool m_back { false };
			bool m_done { false };
			uint32_t m_endFrame { 60 };
			uint32_t m_currentFrame { 0 };
	};

	template<typename T>
	class FrameInterpolatorChain
	{
		public:
			inline FrameInterpolatorChain(bool loop = true)
			{
				m_loop = loop;
				m_done = false;
				m_currentInter = 0;
			}
			inline bool isDone(void) { return !m_loop && m_done; }
			inline void add(FrameInterpolator<T>& _fi)
			{
				if (_fi.isLooping()) return; //TODO: Warning
				m_interList.push_back(&_fi);
			}

			inline void update(void)
			{
				if (m_interList.size() == 0 || isDone()) return;
				std::cout << m_interList[m_currentInter]->isDone() << "\n";
				if (m_interList[m_currentInter]->isDone())
				{
					m_interList[m_currentInter]->reset();
					if (++m_currentInter >= m_interList.size())
					{
						if (!m_loop)
						{
							m_done = true;
							return;
						}
						m_currentInter = 0;
					}
				}
				m_interList[m_currentInter]->update();
			}

			inline T get(void)
			{
				//TODO: Error Checking
				return m_interList[m_currentInter]->get();
			}

		private:
			std::vector<FrameInterpolator<T>*> m_interList;
			bool m_done { false };
			bool m_loop { true };
			uint32_t m_currentInter { 0 };
	};
} // namespace ox

#endif