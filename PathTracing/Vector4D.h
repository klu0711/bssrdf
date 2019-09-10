#pragma once
#include <cmath>

	class Vector4D
	{
	private:
		float vector[4];

	public:
		Vector4D(float x, float y, float z, float w);
		Vector4D();
		~Vector4D();
		Vector4D operator+(const Vector4D& rhs) const;
		Vector4D operator-(const Vector4D& rhs) const;
		Vector4D operator*(const float& rhs) const;
		void operator=(const Vector4D& rhs) ;
		float operator[](const int index) const;
		float & operator[](const int index);
		float length() const;
		Vector4D normalize() const;
		float dotProduct(const Vector4D& vec) const;
		Vector4D crossProduct(const Vector4D& vec) const;
		float getVectorValue(int index);
		float* getPointer();
	};


	/// Constructor when no arguments are provided (Defaut constructor
	inline Vector4D::Vector4D()
	{
		vector[0] = 0;
		vector[1] = 0;
		vector[2] = 0;
		vector[3] = 1;
	}
	/// Deconstructor
	inline Vector4D::~Vector4D()
	{

	}
	/// Operator overload for standard vector addition
	inline Vector4D Vector4D::operator+(const Vector4D& rhs) const
	{
		return Vector4D(vector[0] + rhs.vector[0], vector[1] + rhs.vector[1], vector[2] + rhs.vector[2],1);
	}
	/// Operator overload for standard vector subtraction
	inline Vector4D Vector4D::operator-(const Vector4D& rhs) const
	{
		return Vector4D(vector[0] - rhs.vector[0], vector[1] - rhs.vector[1], vector[2] - rhs.vector[2], 1);
	}
	/// Operator overload for the product of a vector and a scalar
	inline Vector4D Vector4D::operator*(const float& rhs) const
	{
		return Vector4D(vector[0] * rhs, vector[1] * rhs, vector[2] * rhs,1);
	}
	/// Getter
	inline float Vector4D::operator[](const int index) const
	{
		return vector[index];
	}
	/// Setter
	inline float & Vector4D::operator[](const int index)
	{
		return vector[index];
	}
	inline void Vector4D::operator=(const Vector4D &rhs)
	{
	    vector[0] = rhs[0];
	    vector[1] = rhs[1];
	    vector[2] = rhs[2];
	    vector[3] = rhs[3];
	}
	/// Calculates the lenth of the vector
	inline float Vector4D::length() const
	{
		return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
	}
	/// Divides all the values of the vector with the length of the vector (normalization)
	inline Vector4D Vector4D::normalize() const
	{
		float length = this->length();
		return Vector4D(vector[0] / length, vector[1] / length, vector[2] / length, 1);
	}
	/// Returns a float of the dot product of the vectors
	inline float Vector4D::dotProduct(const Vector4D& vec) const
	{
		return ((vector[0] * vec.vector[0]) + (vector[1] * vec.vector[1]) + (vector[2] * vec.vector[2]));
	}
	/// Returns a new vector with the cross product of the vectors
	inline Vector4D Vector4D::crossProduct(const Vector4D& vec) const
	{
		return(Vector4D((vector[1]*vec[2])-(vector[2]*vec[1]), (vector[2]*vec[0])-(vector[0]*vec[2]),
						(vector[0]*vec[1])-(vector[1]*vec[0]),1));
	}
	inline float Vector4D::getVectorValue(int index)
	{
		return vector[index];
	}
	inline float * Vector4D::getPointer()
	{
		return vector;
	}
	/// standard constructor to set all four values of the vactor
	inline Vector4D::Vector4D(float x, float y, float z, float w)
	{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;

	}

