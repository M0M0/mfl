#ifndef MFL_GRAPHICS3D_CAMERA_INCLUDED
#define MFL_GRAPHICS3D_CAMERA_INCLUDED

#include "MFL_Graphics3D_Transformations.hpp"
#include "MFL_LinAlg.hpp"

namespace MFL{
namespace Graphics3D{

	template <typename T> // another name might be better!
	LinAlg::Matrix<T,4> LookAt( //viewmatrix
		LinAlg::Vector<T,3> eye,
		LinAlg::Vector<T,3> target,
		LinAlg::Vector<T,3> up = LinAlg::Vector3<T>(0,1,0) ){
		using namespace LinAlg;

		Vector<T,3> z = (target-eye).getnormalised();
		Vector<T,3> x = CrossProduct(up,z).getnormalised();
		Vector<T,3> y = CrossProduct(z ,x).getnormalised();

		Matrix<T,4> outmat;
		outmat.SetColumn(x,0);
		outmat.SetColumn(y,1);
		outmat.SetColumn(z,2);
		outmat.SetColumn(eye,3);

		return outmat;
	}

	template <typename T>
	class Camera{
		LinAlg::Matrix<T,4> _mat;
	public:
		Camera(){};
		Camera(	LinAlg::Vector<T,3> eye,
		LinAlg::Vector<T,3> target,
		LinAlg::Vector<T,3> up = LinAlg::Vector3<T>(0,1,0) )
		{ set(eye,target,up); }

		Camera& set(LinAlg::Vector<T,3> eye,
		LinAlg::Vector<T,3> target,
		LinAlg::Vector<T,3> up = LinAlg::Vector3<T>(0,1,0) ){
			_mat = LookAt(eye,target,up);
			return *this;
		}

		void rotateX(T angle){
			_mat = ToAffine( LinAlg::Rx(angle) )*_mat;
		}
		void rotateY(T angle){
			_mat = ToAffine( LinAlg::Ry(angle) )*_mat;
		}
		void move(T distance,T x,T y,T z){
			move(distance*LinAlg::Vector3<T>(x,y,z));
		}
		void move(T x,T y,T z){
			move(LinAlg::Vector3<T>(x,y,z));
		}
		void move(LinAlg::Vector<T,3> move_t){
			_mat = Graphics3D::TranslationMatrix(move_t)*_mat;
		}

		const LinAlg::Matrix<T,4>& getMatrix() const{
			return _mat;
		}
		const LinAlg::Vector<T,3>& getPosition() const{
			return LinAlg::Vector3<T>(_mat(0,3),_mat(1,3),_mat(2,3));
		}
	};


}
}


#endif//MFL_GRAPHICS3D_CAMERA_INCLUDED