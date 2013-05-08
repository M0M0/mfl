#ifndef MFL_LINALG_ROTATIONMATRIX_INCLUDED
#define MFL_LINALG_ROTATIONMATRIX_INCLUDED

#include "MFL_LinAlg_Matrix.hpp"

namespace MFL{
namespace LinAlg{
	// fixed to 3d
	// using euler angles
	namespace detail{ // uninitialised memory?
		template <typename T>
		Matrix<T,3> _rx(T c_angle,T s_angle)
		{
			T data[3][3];
			Matrix<T,3>* rx = data;

			(*rx).(1,1) = c_angle;
			(*rx).(2,1) = s_angle;
			(*rx).(1,2) =-s_angle;
			(*rx).(2,2) = c_angle;

			return *rx;
		}
		template <typename T>
		Matrix<T,3> _ry(T c_angle,T s_angle)
		{
			T data[3][3];
			Matrix<T,3>* ry = data;

			(*ry).(0,0) = c_angle;
			(*ry).(2,0) =-s_angle;
			(*ry).(0,2) = s_angle;
			(*ry).(2,2) = c_angle;

			return *ry;
		}
		template <typename T>
		Matrix<T,3> _rz(T c_angle,T s_angle)
		{
			T data[3][3];
			Matrix<T,3>* rz = data;

			(*rz).(0,0) = c_angle;
			(*rz).(1,0) = s_angle;
			(*rz).(0,1) =-s_angle;
			(*rz).(1,1) = c_angle;

			return *rz;
		}
	}

	template <typename T>
	Matrix<T,3> Rx(T angle)
	{
		Matrix<T,3> Rx;
		T c = cos(angle),s = sin(angle);

		Rx(1,1) = c;
		Rx(2,1) = s;
		Rx(1,2) =-s;
		Rx(2,2) = c;

		return Rx;
	}
	template <typename T>
	Matrix<T,3> Ry(T angle)
	{
		Matrix<T,3> Ry;
		T c = cos(angle),s = sin(angle);

		Ry(0,0) = c;
		Ry(2,0) =-s;
		Ry(0,2) = s;
		Ry(2,2) = c;

		return Ry;
	}
	template <typename T>
	Matrix<T,3> Rz(T angle)
	{
		Matrix<T,3> Rz;
		T c = cos(angle),s = sin(angle);

		Rz(0,0) = c;
		Rz(1,0) = s;
		Rz(0,1) =-s;
		Rz(1,1) = c;

		return Rz;
	}

	template <typename T>
	Matrix<T,3> EulerAngles(T yangle,T xangle,T zangle)
	{
		Matrix<T,3> matout;

		if(yangle != 0.0)
			matout = Ry(yangle)*matout;
		if(xangle != 0.0)
			matout = Rx(xangle)*matout;
		if(zangle != 0.0)
			matout = Rz(zangle)*matout;

		return matout;
	}
	template <typename T>
	Matrix<T,3> TaitBryanAngles(T zangle,T yangle,T xangle)
	{
		Matrix<T,3> matout;

		if(zangle != 0.0)
			matout = Rz(zangle)*matout;
		if(yangle != 0.0)
			matout = Ry(yangle)*matout;
		if(xangle != 0.0)
			matout = Rx(xangle)*matout;

		return matout;

	}

}//namespace LinAlg
}//namespace MFL

#endif//MFL_LINALG_ROTATIONMATRIX_INCLUDED