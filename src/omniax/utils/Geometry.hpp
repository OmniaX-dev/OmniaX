#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include <cmath>
#include <algorithm>
#include <cstdint>
#include "Types.hpp"

namespace ox
{
	class IOutputHandler;
	template<class T>
	class Point
	{
		public:
			T x;
			T y;

		public:
			inline Point(void) : x(0), y(0) {}
			inline Point(T xx, T yy) : x(xx), y(yy) {}
			template <class T2> inline Point(Point<T2> copy)
			{
				x = (T2)(copy.x);
				y = (T2)(copy.y);
			}
	};

	typedef Point<float> FPoint;
	typedef Point<double> DPoint;
	typedef Point<uint32_t> UIPoint;
	typedef Point<uint64_t> UI64Point;
	typedef Point<float> UI16Point;
	typedef Point<uint8_t> UI8Point;
	typedef Point<int32_t> IPoint;
	typedef Point<int64_t> I64Point;
	typedef Point<int16_t> I16Point;
	typedef Point<int8_t> I8Point;

	struct Vec2
	{
		//======================== Data ========================
		float x;
		float y;
		//======================================================


		//==================== Construction ====================
		inline Vec2(float xx = 0, float yy = 0) 			: x(xx), y(yy) {  }
		inline Vec2(const Vec2& v2) 						{ set(v2); }
		inline Vec2& set(const Vec2& v2) 					{ x = v2.x; y = v2.y; return *this; }
		inline Vec2& set(float xx, float yy) 				{ x = xx; y = yy; return *this; }
		//======================================================


		//================== Static Functions ==================
		inline static Vec2 fromAngle(float angle)			{ return Vec2(std::cos(angle), std::sin(angle)); }
		inline static float angleBetween(const Vec2& v1, const Vec2& v2) { return std::acos(v1.dot(v2) / (v1.mag() * v2.mag())); }
		//======================================================


		//===================== Conversion =====================
		inline Vec2 toIsometric(void) 			const		{ return Vec2(x - y, (x + y) / 2.0f); }
		inline Vec2 toCartesian(void) 			const 		{ return Vec2((2 * y + x) / 2.0f, (2 * y - x) / 2.0f); }
		String toString(void)					const;
		//======================================================


		//===================== Operations =====================
		inline float mag(void) 					const 		{ return std::sqrt((x * x) + (y * y)); 					}

		inline Vec2 add(Vec2 v2) 				const 		{ return {		x + v2.x, 			y + v2.y 		}; 	}
		inline Vec2 add(float x2, float y2) 	const 		{ return {		x + x2, 			y + y2 			}; 	}
		inline Vec2 sub(Vec2 v2) 				const 		{ return {		x - v2.x, 			y - v2.y 		}; 	}
		inline Vec2 sub(float x2, float y2) 	const 		{ return {		x - x2, 			y - y2 			}; 	}
		inline Vec2 mul(float scalar) 			const 		{ return {		x * scalar, 		y * scalar 		}; 	}
		inline Vec2 div(float scalar) 			const 		{ return {		x / scalar, 		y / scalar 		}; 	}

		inline Vec2 normalize(void)				const		{ float m = _zp(mag()); return { x / m, 	y / m 	};	}
		inline float dist(Vec2 v2)				const		{ return std::sqrt((float)((v2.x - x) * (v2.x - x)) + ((v2.y - y) * (v2.y - y))); }
		inline float heading(void)				const		{ return std::atan2(y, x); }
		inline Vec2 rotate(float angle)			const		{ return Vec2(*this).rotate(angle); }
		inline float dot(const Vec2& v2)		const		{ return (x * v2.x) + (y * v2.y); }
		inline float cross(const Vec2& v2)		const		{ return (x * v2.y) - (v2.x * y); }
		//======================================================


		//===================== Modifiers ======================
		inline Vec2& addm(const Vec2& v2) 			 		{ 		x += v2.x; 			y += v2.y; 	 	return *this;	}
		inline Vec2& addm(const float& x2, const float& y2) { 		x += x2; 			y += y2; 		return *this; 	}
		inline Vec2& subm(const Vec2& v2) 					{ 		x -= v2.x; 			y -= v2.y; 	 	return *this;	}
		inline Vec2& subm(const float& x2, const float& y2) { 		x -= x2; 			y -= y2; 		return *this; 	}
		inline Vec2& mulm(const float& scalar) 				{ 		x *= scalar; 		y *= scalar; 	return *this; 	}
		inline Vec2& divm(const float& scalar) 				{ 		x /= scalar; 		y /= scalar; 	return *this; 	}

		inline Vec2& normalizem(void)						{ float m = _zp(mag()); x /= m; y /= m; 	return *this;	}
		inline Vec2& setMag(const float& mag)				{ return normalizem().mulm(mag); }
		inline Vec2& setHeading(const float& angle) 		{ float m = mag(); set(m * std::cos(angle), m * std::sin(angle)); return *this; }
		inline Vec2& rotate(const float& angle)				{ return setHeading(heading() + angle); }
		inline Vec2& limit(const float& max)				{ float msq = mag(); msq *= msq; if (msq > (max * max)) divm(std::sqrt(msq)).mulm(max); return *this; }
		//======================================================


		//===================== Operators ======================
		inline bool  operator==(const Vec2& op2 ) 		{ return (x == op2.x && y == op2.y); }
		inline bool  operator!=(const Vec2& op2 ) 		{ return (x != op2.x && y != op2.y); }
		inline Vec2  operator+ (const Vec2& op2 ) 		{ return add(op2); }
		inline Vec2  operator- (const Vec2& op2 ) 		{ return sub(op2); }
		inline Vec2  operator+ (const float& op2) 		{ return add(op2, op2); }
		inline Vec2  operator- (const float& op2) 		{ return sub(op2, op2); }
		inline Vec2  operator* (const float& op2) 		{ return mul(op2); }
		inline Vec2  operator/ (const float& op2) 		{ return div(op2); }
		inline Vec2& operator= (const Vec2& val ) 		{ return set(val); }
		inline Vec2& operator= (const float& val) 		{ return set(val, val); }
		inline Vec2& operator+=(const Vec2& op2 ) 		{ return addm(op2); }
		inline Vec2& operator-=(const Vec2& op2 ) 		{ return subm(op2); }
		inline Vec2& operator+=(const float& op2) 		{ return addm(op2, op2); }
		inline Vec2& operator-=(const float& op2) 		{ return subm(op2, op2); }
		inline Vec2& operator*=(const float& op2) 		{ return mulm(op2); }
		inline Vec2& operator/=(const float& op2) 		{ return divm(op2); }

		friend std::ostream& operator<<(std::ostream& out, const Vec2& val);
		//======================================================

	private:
		inline float _zp(float n1) const { return (n1 == 0 ? 1 : n1); }	

	};

	inline std::ostream& operator<<(std::ostream& out, const Vec2& val)
	{
		out << val.toString();
		return out;
	}

	struct Vec3
	{
		inline Vec3(float xx = 0, float yy = 0, float zz = 0)
		{
			x = xx;
			y = yy;
			z = zz;
		}
		inline Vec2 xy(void) const { return Vec2(x, y); }
		inline Vec2 yz(void) const { return Vec2(y, z); }
		inline Vec2 zx(void) const { return Vec2(z, x); }


		float x;
		float y;
		float z;
	};

	struct Triangle
	{
		Vec2 A;
		Vec2 B;
		Vec2 C;
		inline Triangle(void) {}
		inline Triangle(Vec2 a, Vec2 b, Vec2 c)
		{
			A = a;
			B = b;
			C = c;
		}
		inline Triangle(float ax, float ay, float bx, float by, float cx, float cy)
		{
			A.x = ax;
			A.y = ay;
			B.x = bx;
			B.y = by;
			C.x = cx;
			C.y = cy;
		}
		bool contains(Vec2 p)
		{
			float d1, d2, d3;
			bool has_neg, has_pos;

			d1 = __sign(p, A, B);
			d2 = __sign(p, B, C);
			d3 = __sign(p, C, A);

			has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

			return !(has_neg && has_pos);
		}

	private:
		inline float __sign(Vec2 p1, Vec2 p2, Vec2 p3)
		{
			return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		}
	};

	class Rectangle
	{
		public:
			inline Rectangle(void) : x(0), y(0), w(0), h(0) {}
			inline Rectangle(float xx, float yy, float ww, float hh) : x(xx), y(yy), w(ww), h(hh) {}
			inline Rectangle(float xw, float yh, bool position = true) { if (position) { x = xw; y = yh; } else { w = xw; h = yh; } }
			inline Rectangle(float xx, float yy, Vec2 size) { x = xx; y = yy; w = size.x; h = size.y; }
			inline Rectangle(Vec2 position, float ww, float hh) { x = position.x; y = position.y; w = ww; h = hh; }
			inline Rectangle(Vec2 position, Vec2 size) { x = position.x; y = position.y; w = size.x; h = size.y; }
			virtual ~Rectangle(void) = default;

			inline float getx(void) const { return x; }
			inline float gety(void) const { return y; }
			inline float getw(void) const { return w; }
			inline float geth(void) const { return h; }
			inline float getCenterX(void) const { return getx() + getw() / 2; }
			inline float getCenterY(void) const { return gety() + geth() / 2; }

			inline void setx(float xx) { x = xx; }
			inline void sety(float yy) { y = yy; }
			inline void setw(float ww) { w = ww; }
			inline void seth(float hh) { h = hh; }

			inline Vec2 getPosition(void) const { return Vec2(getx(), gety()); }
			inline Vec2 getSize(void) const { return Vec2(getw(), geth()); }
			inline Vec2 getCenter(void) const { return Vec2(getx() + getw() / 2, gety() + geth() / 2); }

			inline void setPosition(Vec2 pos) { setx(pos.x); sety(pos.y); }
			inline void setPosition(float xx, float yy) { setx(xx); sety(yy); }
			inline void setSize(Vec2 size) { setw(size.x); seth(size.y); }
			inline void setSize(float ww, float hh) { setw(ww); seth(hh); }
			inline void setBounds(float xx, float yy, float ww, float hh) { setx(xx); sety(yy); setw(ww); seth(hh); }

			inline float addx(float xx) { setx(getx() + xx); return getx(); }
			inline float addy(float yy) { sety(gety() + yy); return gety(); }
			inline Vec2 addPos(float xx, float yy) { return Vec2(addx(xx), addy(yy)); }
			inline Vec2 addPos(Vec2 pos) { return addPos(pos.x, pos.y); }
			inline float addw(float ww) { setw(getw() + ww); return getw(); }
			inline float addh(float hh) { seth(geth() + hh); return geth(); }
			inline Vec2 addSize(float ww, float hh) { return Vec2(addw(ww), addh(hh)); }
			inline Vec2 addSize(Vec2 size) { return addPos(size.x, size.y); }

			inline float subx(float xx) { setx(getx() - xx); return getx(); }
			inline float suby(float yy) { sety(gety() - yy); return gety(); }
			inline Vec2 subPos(float xx, float yy) { return Vec2(subx(xx), suby(yy)); }
			inline Vec2 subPos(Vec2 pos) { return subPos(pos.x, pos.y); }
			inline float subw(float ww) { setw(getw() - ww); return getw(); }
			inline float subh(float hh) { seth(geth() - hh); return geth(); }
			inline Vec2 subSize(float ww, float hh) { return Vec2(subw(ww), subh(hh)); }
			inline Vec2 subSize(Vec2 size) { return subPos(size.x, size.y); }

			inline float mulx(float xx) { setx(getx() * xx); return getx(); }
			inline float muly(float yy) { sety(gety() * yy); return gety(); }
			inline Vec2 mulPos(float xx, float yy) { return Vec2(mulx(xx), muly(yy)); }
			inline Vec2 mulPos(Vec2 pos) { return mulPos(pos.x, pos.y); }
			inline float mulw(float ww) { setw(getw() * ww); return getw(); }
			inline float mulh(float hh) { seth(geth() * hh); return geth(); }
			inline Vec2 mulSize(float ww, float hh) { return Vec2(mulw(ww), mulh(hh)); }
			inline Vec2 mulSize(Vec2 size) { return mulPos(size.x, size.y); }

			inline float divx(float xx) { setx(getx() / xx); return getx(); }
			inline float divy(float yy) { sety(gety() / yy); return gety(); }
			inline Vec2 divPos(float xx, float yy) { return Vec2(divx(xx), divy(yy)); }
			inline Vec2 divPos(Vec2 pos) { return divPos(pos.x, pos.y); }
			inline float divw(float ww) { setw(getw() / ww); return getw(); }
			inline float divh(float hh) { seth(geth() / hh); return geth(); }
			inline Vec2 divSize(float ww, float hh) { return Vec2(divw(ww), divh(hh)); }
			inline Vec2 divSize(Vec2 size) { return divPos(size.x, size.y); }

			inline Vec2 topLeft(void) const { return getPosition(); }
			inline Vec2 topRight(void) const { return Vec2(getx() + getw(), gety()); }
			inline Vec2 bottomLeft(void) const { return Vec2(getx(), gety() + geth()); }
			inline Vec2 bottomRight(void) const { return Vec2(getx() + getw(), gety() + geth()); }

			inline bool intersects(Rectangle rect, bool includeBounds = true) const
			{
				if (includeBounds)
				{
					if (x + w <= rect.x || x >= rect.x + rect.w)
						return false;
					if (y + h <= rect.y || y >= rect.y + rect.h)
						return false;
				}
				else
				{
					if (x + w < rect.x || x > rect.x + rect.w)
						return false;
					if (y + h < rect.y || y > rect.y + rect.h)
						return false;
				}
				return true;
			}
			inline Rectangle getIntersection(Rectangle rect, bool includeBounds = true) const
			{
				if (!intersects(rect, includeBounds))
					return Rectangle();
				float leftX   = std::max(x, rect.x);
				float rightX  = std::min(x + w, rect.x + rect.w);
				float topY    = std::max(y, rect.y);
				float bottomY = std::min(y + h, rect.y + rect.h);
				return { leftX, topY, rightX - leftX, bottomY - topY };
			}
			inline bool contains(Vec2 p, bool includeBounds = false) const
			{
				if (includeBounds)
					return p.x >= x && p.y >= y & p.x <= x + w && p.y <= y + h;
				else
					return p.x > x && p.y > y & p.x < x + w && p.y < y + h;
			}
			inline bool contains(float xx, float yy, bool includeBounds = false) const { return contains({ xx, yy }); }

			inline float getDistance(Vec2 p) const { return sqrt(fabs((p.x - getx()) * (p.x - getx()) + (p.y - gety()) * (p.y - gety()))); }

		public:
			float x;
			float y;
			float w;
			float h;
	};
} // namespace ox

 #endif