#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cstdint>
//#include <SFML/Graphics/Color.hpp>
#include <omniax/utils/Types.hpp>
#include <omniax/utils/BaseObject.hpp>

namespace ox
{
	class Color : public BaseObject
	{
		public: struct FloatCol
		{
			float r;
			float g;
			float b;
			float a;

			FloatCol(void) : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {  }
			FloatCol(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {  }
		};

		public:
			Color(void);
			Color(uint8_t rgb_single_value, uint8_t alpha = 255);
			Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t alpha = 255);
			//Color(const sf::Color& sfml_color);
			Color(const String& color_string);
			Color(const FloatCol& normalized_color);

			bool operator==(const Color& col2);
			bool operator!=(const Color& col2);

			Color& set(void);
			Color& set(uint8_t rgb_single_value, uint8_t alpha = 255);
			Color& set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t alpha = 255);
			//Color& set(const sf::Color& sfml_color);
			Color& set(const String& color_string);
			Color& set(const FloatCol& normalized_color);

			//sf::Color sf(void) const;
			String hexString(bool include_alpha = false, String prefix = "0x") const;
			String rgbString(bool include_parenthesis = true, bool include_alpha = false) const;
			uint32_t asInteger(void) const;
			FloatCol getNormalizedColor(void) const;

			String toString(void) const override;
			void print(bool newLine = true, IOutputHandler* __destination = nullptr) const override;
			inline void invalidate(void) override {  }
			inline void setValid(bool valid) override {  }

		public:
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
	};
} //namesoace ox



#endif