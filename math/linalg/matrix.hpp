#ifndef MFL_LINALG_MATRIX_INCLUDED
#define MFL_LINALG_MATRIX_INCLUDED


#include "linalg_common.hpp"
#include "linalg_vector.hpp"

LINALG_BEGIN

	template <typename,int,int> class Matrix;
	template <typename T,int N> const Matrix<T,N,N> Diag(const T diag_value);

	//Rowmajor Matrix
	template <typename T,int N,int M=N>
	class Matrix{
	protected:
		T _s[N][M];

		void setzero(){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j)=0;
		}
	public:
		//Constructor and copy operators
		Matrix(){
			N==M?this->operator=(Diag<T,N>(1)):setzero();
		}
		Matrix(T s_in){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j)= s_in;
		}
		Matrix(T s_in[N*M],bool rowmajor){
			assert(s_in);
			if(rowmajor)
				std::copy(s_in,&s_in[N*(M-1)],_s);
			else
				for(int i=0;i<N;++i)
					for(int j=0;j<M;++j)
						this->operator()(i,j)=s_in[j*M+i];
		}

		Matrix(const Matrix& rhs){
			//assert(rhs!=nullptr);
			std::copy(&(rhs._s[0][0]),&(rhs._s[N-1][M]),&(this->_s[0][0]));
		}
		Matrix& operator= (const Matrix& rhs){
			//assert(rhs!=nullptr);
			std::copy(&(rhs._s[0][0]),&(rhs._s[N-1][M]),&(this->_s[0][0]));
			return *this;
		}

		Matrix& operator= (const T rhs){
			std::fill(this->_s,&(this->_s[N-1][M]),rhs);
			return *this;
		}

		// for accessing members
		T&      operator() (const int i,const int j){
			assert(i>=0&&i<N);
			assert(j>=0&&j<M);
			return this->_s[i][j];
		}
		const T operator() (const int i,const int j) const{
			assert(i>=0&&i<N);
			assert(j>=0&&j<M);
			return this->_s[i][j];
		}

		// untested
		operator  T*(){
			return reinterpret_cast<T*>(_s);
		}

		Matrix& SetRow(Vector<T,M> rvector,int row){
			assert(row>=0&&row<N);
			std::copy(&rvector(0),&rvector(M),&_s[row][0]);
			return *this;
		}
		Matrix& SetRow(Vector<T,M-1> rvector,int row){
			assert(row>=0&&row<N);
			std::copy(&rvector(0),&rvector(M-1),&_s[row][0]);
			return *this;
		}

		Matrix& SetColumn(Vector<T,N> cvector,int col){
			assert(col>=0&&col<M);
			for(int i=0;i<N;++i)
				this->operator()(i,col) = cvector(i);
			return *this;
		}
		Matrix& SetColumn(Vector<T,N-1> cvector,int col){
			assert(col>=0&&col<M);
			for(int i=0;i<N-1;++i)
				this->operator()(i,col) = cvector(i);
			return *this;
		}



		//transpose self
		Matrix&				transpose(){
			assert(N==M);
			return (*this=this->transposed());
		}
		//returns transposed copy
		const Matrix<T,M,N> transposed() const{
			Matrix<T,M,N> out;
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					out(j,i)=this->operator()(i,j);
			return out;
		};

		const T trace() const{
			assert( N==M );
			T ret = 0;
			for(int i=0; i<N; ++i)
				ret += this->operator()(i,i);
			return ret;
		}

		Matrix& operator+=(const Matrix& rhs){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j) += rhs(i,j);

			return *this;
		}
		Matrix& operator-=(const Matrix& rhs){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j) -= rhs(i,j);

			return *this;
		}
		Matrix& operator*=(const Matrix<T,M,M>& rhs){
			auto tmp = Matrix<T,N,M>(0);
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					for(int k=0;k<M;++k)
						tmp(i,j) += this->operator()(i,k)*rhs(k,j);

			return ((*this) = tmp);
		}

		Matrix& operator*=(const T       rhs){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j) *= rhs;

			return *this;
		}
		Matrix& operator/=(const T       rhs){
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					this->operator()(i,j) /= rhs;

			return *this;
		}

		const Matrix operator+(const Matrix& rhs) const{
			return Matrix(*this).operator+=(rhs);
		}
		const Matrix operator-(const Matrix& rhs) const{
			return Matrix(*this).operator-=(rhs);
		}
		template <int O>
		const Matrix<T,N,O> operator*(const Matrix<T,M,O>& rhs) const{
			auto tmp = Matrix<T,N,O>(0);
			for(int i=0;i<N;++i)
				for(int j=0;j<O;++j)
					for(int k=0;k<M;++k)
						tmp(i,j) += this->operator()(i,k)*rhs(k,j);
			return tmp;
		}

		const Vector<T,N> operator*(const Vector<T,M>& rhs) const{
			 Vector<T,N> tmp(0);
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					tmp(i) += this->operator()(i,j)*rhs(j);
			return tmp;
		}

		const Matrix operator*(const T       rhs)      const{
			return Matrix(*this).operator*=(rhs);
		}
		const Matrix operator/(const T       rhs)      const{
			return Matrix(*this).operator/=(rhs);
		}

		bool operator==(const Matrix rhs){
			bool b=true;
			for(int i=0;i<N;++i)
				for(int j=0;j<M;++j)
					b&=this->operator()(i,j)==rhs(i,j);
			return b;
		}
	};

	template <typename T,int N>
	inline const Matrix<T,N,N> Diag(const T diag_array[N]){
		Matrix<T,N,N> out(0);

		for(int i=0;i<N;++i)
			out(i,i) = diag_array[i];

		return out;
	}
	template <typename T,int N>
	inline const Matrix<T,N,N> Diag(const Vector<T,N>& vec){
		return Diag( (T*)&vec );
	}
	template <typename T,int N>
	inline const Matrix<T,N,N> Diag(const T diag_value){
		Matrix<T,N,N> out(0);

		for(int i=0;i<N;++i)
			out(i,i) = diag_value;

		return out;
	}

	//untested -- 2d tensor product
	template <typename T,int N,int M,int O,int P>
	inline Matrix<T,N*O,M*P> KroneckerProduct(const Matrix<T,N,M> a,const Matrix<T,O,P> b){
		Matrix<T,N*O,M*P> out;
		for(int i=0;i<N;++i)
			for(int j=0;j<M;++j)
				for(int k=0;k<O;++k)
					for(int l=0;l<O;++l)
						out(O*(i)+k,P*(j)+l) = a(i,j)*b(k,l);

		return out;
	}



#ifdef _OSTREAM_
	template <typename T,int N,int M>
	inline std::ostream& operator<<(std::ostream& out,const Matrix<T,N,M>& rhs){

		out << "{\n";
		for(int i=0;i<N;++i){
			out << "( ";
			for(int j=0;j<M;++j)
				out << rhs(i,j) << ' ';

			out << ")\n";
		}
		out << "}\n";
		return out;
	}
#endif

	
	template <typename T,int N>
	Matrix<T,N+1> ToAffine(const Matrix<T,N>& inmat){
		Matrix<T,N+1> outmat;
		for(int i=0;i<N;++i)
			for(int j=0;j<N;++j)
				outmat(i,j) = inmat(i,j);
		return outmat;
	}
	
	template <typename T,int N> 
	Matrix<T,N-1> FromAffine(const Matrix<T,N>& inmat){// does not check anything!
		Matrix<T,N-1> outmat;
		for(int i=0;i<N-1;++i)
			for(int j=0;j<N-1;++j)
				outmat(i,j) = inmat(i,j);
		return outmat;
	}

	// Discussion: Rename to ToAffine()/FromAffine() ? 
	// done added ( Homgenize  might be removed )
	template <typename T,int N>
	Matrix<T,N+1> Homogenize(const Matrix<T,N>& inmat){
		Matrix<T,N+1> outmat;
		for(int i=0;i<N;++i)
			for(int j=0;j<N;++j)
				outmat(i,j) = inmat(i,j);
		return outmat;
	}
	template <typename T,int N>
	Matrix<T,N-1> Dehomogenize(const Matrix<T,N>& inmat){
		Matrix<T,N-1> outmat;
		for(int i=0;i<N-1;++i)
			for(int j=0;j<N-1;++j)
				outmat(i,j) = inmat(i,j);
		return outmat;
	}

	typedef Matrix<double,2>	Mat2d;
	typedef Matrix<float,2>		Mat2f;
	typedef Matrix<int,2>		Mat2i;

	typedef Matrix<double,3>	Mat3d;
	typedef Matrix<float,3>		Mat3f;
	typedef Matrix<int,3>		Mat3i;

	typedef Matrix<double,4>	Mat4d;
	typedef Matrix<float,4>		Mat4f;
	typedef Matrix<int,4>		Mat4i;

LINALG_END

#endif//MFL_LINALG_MATRIX_INCLUDED
/*#include <cassert>


typedef unsigned int uint;

#define M_OFFSET N*M

template <typename T,uint N,uint M=N>
class Matrix{
public:
	Matrix(T s=0)
		{std::fill(_s,_s+M_OFFSET,s);}
	Matrix(const Matrix& mat)
		{std::copy(mat._s,mat._s+M_OFFSET,_s);}

	Matrix& operator=(const Matrix& rhs){
		std::copy(rhs._s,rhs._s+M_OFFSET,_s);
		return *this;
	}

	T& operator()(const uint index)
		{assert(index<M_OFFSET);return _s[index];}
	const T operator()(const uint index) const
		{return operator()(index);}

	T& operator()(const uint i,const uint j){
		assert(i<N);
		assert(j<M);
		assert((i+M*j)<M_OFFSET);
		return _s[i+N*j]; // columnmajor
	}
	const T operator()(const uint i,const uint j) const{
		assert(i<N);
		assert(j<M);
		assert((i+M*j)<M_OFFSET); 
		return _s[i+N*j]; // columnmajor
	}

	template <typename S>
	operator Matrix<S,N,M>(){
		Matrix<S,N,M> mat;
		for(uint idx=0; idx<M_OFFSET; ++idx)
			mat(idx) = static_cast<S>(_s[idx]);
		return mat;
	}

	Matrix& operator+=(const Matrix& rhs){
		for(uint i=0; i<M_OFFSET; ++i)
			_s[i] += rhs._s[i];
		return *this;
	}
	const Matrix operator+(const Matrix& rhs) const
		{return Matrix(*this)+=rhs; }

	Matrix& operator-=(const Matrix& rhs){
		for(uint idx=0; i<M_OFFSET; ++i)
			_s[idx] -= rhs._s[idx];
		return *this;
	}
	const Matrix operator-(const Matrix& rhs) const
		{return Matrix(*this)-=rhs; }

	const Matrix operator-()
		{ return Matrix() -= *this; }

	template <uint O>
	const Matrix operator*(const Matrix<T,M,O>& rhs) const{
		Matrix<T,N,O> m(0);
		for(int j=0;j<O;++j)
			for(int k=0;k<M;++k)
				for(int i=0;i<N;++i)
					m(i,j) += (*this)(i,k)*rhs(k,j);
		return m;
	}

	Matrix& operator*=(const T rhs){
		for(uint i=0; i<M_OFFSET; ++i)
			_s[i] *= rhs;
		return *this;
	}

	const Matrix operator*(const T rhs) const
		{return Matrix(*this)*=rhs;}

	const Matrix<T,M,N> get_transposed() const{
		Matrix<T,M,N> m;
		for(uint j=0; j<M; ++j)
			for(uint i=0; i<N; ++i)
				m(j,i) = operator()(i,j);
		return m;
	}

	bool operator==(const Matrix& rhs){
		if(this==&rhs)
			return true;
		return std::equal(_s,_s+M_OFFSET,rhs._s);
	}


private:
	T _s[N*M]; 
};


#undef M_OFFSET

template <typename T,uint N,uint M>
const Matrix<T,N,M> MatrixFromArray(T s[N*M]){
	if(s)
		return (*reinterpret_cast<Matrix<T,N,M>*>(s));
	return Matrix<T,N,M>();
}

template <typename T,uint N,uint M>
const T* ArrayFromMatrix(const Matrix<T,N,M>& s)
	{ return reinterpret_cast<const T*>(&s); }



template <typename T,uint N>
const Matrix<T,N,N> Diag(const T d_value){
	Matrix<T,N,N> m;
	for(uint i=0; i<N*N; ++i)
		m(i,i) = d_value;
	return m;
}
template <typename T,uint N>
const Matrix<T,N,N> Diag(const T d_value[N]){
	Matrix<T,N,N> m;
	for(uint i=0; i<N*N; ++i)
		m(i,i) = d_value[i];
	return m;
}
*/