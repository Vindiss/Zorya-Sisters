#pragma once
#include <SFML/System/Vector2.hpp>

namespace Maths
{
	template<typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		/**
		 * \brief Default constructor, set 0 to both components
		 */
		Vector2();

		/**
		 * \brief Constructor with explicit values for both components
		 * \param _x X component
		 * \param _y Y component
		 */
		Vector2(T _x, T _y);

		/**
		 * \brief Default destructor
		 */
		~Vector2() = default;

		/**
		 * \brief Explicit conversion to sf::Vector2
		 */
		explicit operator sf::Vector2<T>() const { return sf::Vector2<T>(x, y); }

		Vector2(const Vector2& _other) = default;
		Vector2(Vector2&& _other) noexcept = default;
		Vector2& operator=(const Vector2& _other) = default;
		Vector2& operator=(Vector2&& _other) noexcept = default;

		//Addition
		Vector2 operator+(const Vector2& _rhs);
		//Subtraction
		Vector2 operator-(const Vector2& _rhs);
		//Multiplication
		Vector2 operator*(const Vector2& _rhs);
		//Division
		Vector2 operator/(const Vector2& _rhs);

		//Multiplication
		Vector2 operator*(const T& _rhs);
		//Division
		Vector2 operator/(const T& _rhs);

		//Addition
		Vector2 operator+=(const Vector2& _rhs);
		//Subtraction
		Vector2 operator-=(const Vector2& _rhs);
		//Multiplication
		Vector2 operator*=(const Vector2& _rhs);
		//Division
		Vector2 operator/=(const Vector2& _rhs);

		//Multiplication
		Vector2 operator*=(const T& _rhs);
		//Division
		Vector2 operator/=(const T& _rhs);

		//Dot Product
		float Dot(const Vector2& _rhs) const;
		//Cross Product
		float Cross(const Vector2& _rhs) const;
		//Magnitude
		float Magnitude() const;
		//Magnitude Squared
		float MagnitudeSquared() const;
		//Normalize
		Vector2 Normalize() const;
		//Distance
		float Distance(const Vector2& _rhs) const;
		//Distance Squared
		float DistanceSquared(const Vector2& _rhs) const;
		//Angle
		float Angle(const Vector2& _rhs) const;
		//Rotate
		Vector2 Rotate(const T& _angle) const;

		//Lerp
		static Vector2 Lerp(const Vector2& _lhs, const Vector2& _rhs, const T& _alpha);
		//Max
		static Vector2 Max(const Vector2& _lhs, const Vector2& _rhs);
		//Min
		static Vector2 Min(const Vector2& _lhs, const Vector2& _rhs);

		//Get the X value
		T GetX();
		//Get the Y value
		T GetY();
		//Set the X value
		void SetX(const T& _new_x);
		//Set the Y value
		void SetY(const T& _new_y);
		//Set the X and Y values
		void SetXY(const T& _new_x, const T& _new_y);
		//Set the X and Y values
		void SetXY(const Vector2& _rhs);
		//Set the X and Y values

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Left;
		static const Vector2 Right;

		friend bool operator==(const Vector2& _lhs, const Vector2& _rhs)
		{
			return _lhs.x == _rhs.x && _lhs.y == _rhs.y;
		}

		friend bool operator!=(const Vector2& _lhs, const Vector2& _rhs) { return !(_lhs == _rhs); }
	};

	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
}

template class Maths::Vector2<int>;
template class Maths::Vector2<unsigned int>;
template class Maths::Vector2<float>;
template class Maths::Vector2<double>;

#include "Maths/Vector2.inl"
