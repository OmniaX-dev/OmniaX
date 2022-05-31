/*
License
~~~~~~~
Copyright (C) 2018  Javidx9
GNU GPLv3 https://github.com/OneLoneCoder/videos/blob/master/LICENSE

Author
~~~~~~
	Twitter: @javidx9
	Blog: www.onelonecoder.com
	Original works located at:
	https://www.github.com/onelonecoder
	https://www.onelonecoder.com
	https://www.youtube.com/javidx9

	Videos Explaining Splines:
	~~~~~~~~~~~~~~~~~~~~~~~~
	https://youtu.be/9_aJGUTePYo
	https://youtu.be/DzjtU4WLYNs
*/

#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/runtime/Signals.hpp>

namespace ox
{
	struct tSplineNode
	{
		Vec2 position;
		float length;
	};

	class Spline : public BaseObject
	{
		public:
			Spline(void);
			void handleSignal(tSignal& signal) override;
			tSplineNode getPoint(float t);
			tSplineNode getGradient(float t);
			float updateSegmentLength(int32_t node);
			float getNormalisedOffset(float p);
			void connectSignals(void);
			void draw(float resolution = 0.01f, float lineWidth = 4.0f, float controlPointSize = 9.0f);
			void updateTotalLength(void);
			inline bool exists(void) { return m_points.size() > 1; }
			inline void addPoint(Vec2 position) { m_points.push_back({ position, 0.0f }); updateTotalLength(); }
			inline float getTotalLength(void) { return m_totalLength; }
			inline bool isEditable(void) { return m_editable; }
			inline bool isLooping(void) { return m_looping; }
			inline void setEditable(bool e) { m_editable = e; }
			inline void setLooping(bool l) { m_looping = l; }
			inline uint32_t getPointCount(void) { return m_points.size(); }
			inline void clear(void) { m_points.clear(); }
			inline bool isEnabled(void) { return m_enabled; }
			inline void enable(bool e = true) { m_enabled = e; }

		private:
			std::vector<tSplineNode> m_points;
			float m_totalLength = 0.0f;
			tSplineNode* m_selectedNode { nullptr };
			bool m_editable { false };
			bool m_looping { true };
			bool m_enabled { true };
			bool m_signalsConnected { false };
	};
}

#endif