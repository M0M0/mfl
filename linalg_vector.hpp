// A very general Vector class
// TODO:	- General overhaul of functionality (done some depricated features are still !)
//			- Change operator ^ to Cross
//			- Should +/- operator be defined without references to other operators?
#ifndef MFL_LINALG_VECTOR_INCLUDED
#define MFL_LINALG_VECTOR_INCLUDED

#include "MFL_LinAlg_Common.hpp"

#include <cassert>
#include <algorithm>

LINALG_BEGIN

template <class T,uint D>
class Vector {
public:
	Vector(const T s = 0)
		{ std::fill(_s,_s+D,s); }
	Vector(const Vector& in)
		{ std::copy(in._s,in._s+D,_s); }

	// explicit casting might be preferable
	Vector(const Vector<T,D-1>& in){
		std::copy(&in(0),&in(D-2)+1,_s);
		(*this)(D-1) = static_cast<T>(1);
	}
	// explicit casting might be preferable
	Vector(const Vector<T,D+1>& in){
		std::copy(&(in(0)),&(in(D)),_s);
		this->operator/=(in(D));
	}

	Vector& operator= (const Vector& rhs){
		if( this != &rhs )
			std::copy(rhs._s,rhs._s+D,_s);
		return *this;
	}

	template <typename S>
	operator Vector<S,D>(){
		Vector<S,D> tmp;
		for(uint i=0; i<D; ++i)
			tmp(i) = static_cast<S>( _s[i] );
		return tmp;
	}

	// Non-const member access
	T&      operator()(const int index){
		assert(index>=0&&index<D);
		return _s[index];
	}
	// Const member access
	const T operator()(const int index) const{
		assert(index>=0&&index<D);
		return this->_s[index];
	}

	const Vector operator-() const
		{ return Vector() -= *this;}

	Vector& operator+=(const Vector& rhs){
		for(uint i=0; i<D; ++i)
			_s[i] += rhs._s[i];
		return *this;
	}
	const Vector operator+(const Vector& rhs) const
		{ return Vector(*this)+=rhs;}

	Vector&	operator-=(const Vector& rhs){
		for(uint i=0; i<D; ++i)
			_s[i] -= rhs._s[i];
		return *this;
	}
	const Vector operator-(const Vector& rhs) const
		{ return Vector(*this)-=rhs; }

	Vector&     operator*=(const T       rhs){
		for(uint i=0; i<D; ++i)
			_s[i] *= rhs;
		return *this;
	}
	const Vector operator*(const T       rhs) const{
		return Vector(*this)*=rhs;
	}

	Vector&     operator+=(const T       rhs){
		for(uint i=0; i<D; ++i)
			_s[i] += rhs;
		return *this;
	}	
	const Vector operator+(const T		 rhs) const // performs a deep copy and calls (+=)
		{ return Vector(*this)+=rhs; }

	Vector&     operator-=(const T		 rhs){
		for(uint i=0; i<D; ++i)
			_s[i] -= rhs;
		return *this;
	}	
	const Vector operator-(const T 		 rhs) const // performs a deep copy and calls (-=)
		{ return Vector(*this)-=rhs; }

	//Inner product / scalar product (should be named and not used like that
	const T		 operator*(const Vector& rhs) const{
		T product = 0;
		for(uint i=0; i<D; ++i)
			product += this->operator()(i)*rhs(i);
		return product;
	}

	//Magnitude of Vector ( adding a norm function to be plugged in as a template argument )
	const T magnitudeSq() const{
		return this->operator*(*this);
	}
	const T magnitude() const{
		return sqrt(magnitudeSq());
	}

	const T distance(const Vector& rhs) const{
		return Vector(*this-rhs).magnitude();
	}

	Vector&		 normalise()
		{ return (*this)*=1/magnitude(); }
	const Vector getnormalised() const
		{ return Vector(*this).normalise(); }

	bool operator==(const Vector& rhs) const{
		if(this == &rhs)
			return true;
		return std::equal(_s,_s+D,rhs._s);
	}
	bool operator!=(const Vector& rhs) const
		{ return !(*this==rhs); }

private:
	T _s[D];

	template <typename,uint> friend
	const Vector operator*(const T,const Vector&);
	template <typename,uint> friend
	const Vector operator+(const T,const Vector&);
	template <typename,uint> friend
	const Vector operator-(const T,const Vector&);

	template <typename,uint> friend
	const Vector DotProduct(const Vector&,const Vector&);
};


template <typename T,uint D>
inline const Vector<T,D> operator*(const T lhs,const Vector<T,D>& rhs){
	Vector<T,D> out;
	for(uint i=0; i<D; ++i)
		out._s[i] = lhs * rhs._s[i];
	return out;
}
template <typename T,uint D>
inline const Vector<T,D> operator+(const T lhs,const Vector<T,D>& rhs){
	Vector<T,D> out;
	for(uint i=0; i<D; ++i)
		out._s[i] = lhs + rhs._s[i];
	return out;
};
template <typename T,uint D>
inline const Vector<T,D> operator-(const T lhs,const Vector<T,D>& rhs){
	Vector<T,D> out;
	for(uint i=0; i<D; ++i)
		out._s[i] = lhs - rhs._s[i];
	return out;
};

template <typename T,uint D>
const Vector<T,D> VectorFromArray(T s[D]){
	if(s)
		return (*reinterpret_cast<Vector<T,D>*>(s));
	return Vector<T,D>(); //or maybe throw an exception?
}

template <typename T,uint D>
const T* ArrayFromVector(const Vector<T,D>& s)
	{ return reinterpret_cast<const T*>(&s); }

// Outer Product in 3D -> Cross Product
template <typename T>
inline const Vector<T,3>   operator^(const Vector<T,3>& lhs,const Vector<T,3>& rhs){
	Vector<T,3> out;

	out(0) = lhs(1)*rhs(2) - lhs(2)*rhs(1);
	out(1) = lhs(2)*rhs(0) - lhs(0)*rhs(2);
	out(2) = lhs(0)*rhs(1) - lhs(1)*rhs(0);

	return out;
}

template <typename T>
inline const Vector<T,3> CrossProduct(const Vector<T,3>& lhs,const Vector<T,3>& rhs){
	Vector<T,3> out;

	out(0) = lhs(1)*rhs(2) - lhs(2)*rhs(1);
	out(1) = lhs(2)*rhs(0) - lhs(0)*rhs(2);
	out(2) = lhs(0)*rhs(1) - lhs(1)*rhs(0);

	return out;
}

template <typename T,int D>
inline const T			 DotProduct(const Vector<T,D>& lhs,const Vector<T,D>& rhs){
		T product = 0;
		for(uint i=0; i<D; ++i)
			product += lhs._s[i]*rhs._s[i];
		return product;
}

template <typename T,int D>
inline const T			 Cosine(const Vector<T,D>& lhs,const Vector<T,D>& rhs){

	if(lhs==0||rhs==0)
		return 0;
	else if(lhs==rhs)
		return 1;

	return (lhs*rhs)/(lhs.magnitude()*rhs.magnitude());
}

template <typename T,uint D>
inline const Vector<T,D+1> ToAffine(const Vector<T,D>& in){
	Vector<T,D+1> out;
	std::copy(ArrayFromVector(in),// just copy [1,..,D) in new Vector
		ArrayFromVector(in)+D,
		ArrayFromVector(out));
	out(D) = 1;
	return out;
}
template <typename T,uint D>
inline const Vector<T,D-1> FromAffine(const Vector<T,D>& in){
	Vector<T,D-1> out;
	std::copy(ArrayFromVector(in),// just copy [1,..,D-1) in new Vector
		ArrayFromVector(in)+D-1,
		ArrayFromVector(out));
	out *= 1/in(D-1);
	return out;
}


//Overloaded bitshift operators
template <typename T,uint D>
inline std::istream& operator>>(std::istream& in, Vector<T,D>& p){
	for(uint i=0; i<D; ++i)
		in >> p(i);
	return in;
}

template <typename T,uint D> // some formating change might be good! maybe (x,y,z)
inline std::ostream& operator<<(std::ostream& out,const Vector<T,D>& p){
	out << '(';
	for(uint i=0; i<D-1; ++i)
		out << p(i) << ',';
	out << p(D-1) << ')' << std::endl;
	return out;
}


//Typedefs for most used types
typedef Vector<double,4> Vector4d;
typedef Vector<float,4>  Vector4f;
typedef Vector<int,4>	 Vector4i;

typedef Vector<double,3> Vector3d;
typedef Vector<float,3>  Vector3f;
typedef Vector<int,3>	 Vector3i;

typedef Vector<double,2> Vector2d;
typedef Vector<float,2>  Vector2f;
typedef Vector<int,2>	 Vector2i;

LINALG_END

#endif//MFL_LINALG_VECTOR_INCLUDED