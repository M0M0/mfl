#ifndef MFL_GRAPHICS_BEZIER_INCLUDED
#define MFL_GRAPHICS_BEZIER_INCLUDED

#include "MFL_Graphics_Common.hpp"

#include <glm\glm.hpp>
#include <vector>

GRAPHICS_BEGIN

class BezierCurve{
	typedef std::vector<glm::vec3> PointArray_t;
	PointArray_t controlpoints;

public:
	BezierCurve()
		:controlpoints(){}
	BezierCurve(PointArray_t points)
		:controlpoints(points){}
	BezierCurve(PointArray_t::iterator first,PointArray_t::iterator last)
		:controlpoints(first,last){}

	void addPoint(float x,float y,float z){
		addPoint(glm::vec3(x,y,z));
	}
	void addPoint(glm::vec3 p){
		controlpoints.push_back(p);
	}

	// construction by De Casteljau algorithm
	// thus increased numerical stability
	// but decreased efficiency
	glm::vec3 getPoint(float t) const {
		std::vector<glm::vec3> subCurve(controlpoints);
		float inv_t = 1-t;

		if(controlpoints.size() < 2)
			return glm::vec3(0);

		for( uint i=1; i < controlpoints.size(); ++i){
			for( auto it = subCurve.begin(); it+1 != subCurve.end(); ++it){
				*it = *it * inv_t + *(it+1) * t;
			}
			subCurve.pop_back();
		}
		return subCurve[0];
	}
	BezierCurve getTangentCurve() const {
		PointArray_t derivate_array;

		float n = static_cast<float>(controlpoints.size()-1);

		for( auto iter = controlpoints.begin();
			 iter+1 != controlpoints.end();
			 ++iter ){
			derivate_array.push_back( n*( *(iter+1) - (*iter) ) );
		}

		return BezierCurve(derivate_array);
	}

	uint getOrder() const{
		return static_cast<uint>(controlpoints.size() - 1);
	}

	std::vector<glm::vec3> constructApproximation(uint n) const{
		float inv_n = 1/static_cast<float>(n-1);

		std::vector<glm::vec3> points;

		for( uint i=0; i<n; ++i ){
			points.push_back( getPoint( i*inv_n ) );
		}
		return points;
	}
};

class BezierSurface{
	typedef std::vector<std::vector<glm::vec3>> PointArray_t;
	// make sure that all subvectors are of the same size!!!
	PointArray_t controlpoints;

public:
	BezierSurface()
		:controlpoints(){}
	BezierSurface(PointArray_t points)
		:controlpoints(points){}
	BezierSurface(PointArray_t::iterator first,PointArray_t::iterator last)
		:controlpoints(first,last){}

	BezierCurve& getArray(uint index){
		if(controlpoints.size() <= index)
			controlpoints.resize(index+1);
		return *reinterpret_cast<BezierCurve*>(&controlpoints[index]);
	}

	glm::vec3 getPoint(float u,float v) const {
		// The following should also work:
		// return _constructCurve_X(v).getPoint(u);
		// but due to x major ordering might be less efficient
		return _constructCurve_Y(u).getPoint(v);
	}
	glm::vec3 getNormal(float u,float v) const {
		glm::vec3 u_tangent = _constructCurve_X(v).getTangentCurve().getPoint(u);
		glm::vec3 v_tangent = _constructCurve_Y(u).getTangentCurve().getPoint(v);

		return glm::normalize(glm::cross(u_tangent,v_tangent));
	}

	std::vector<std::pair<glm::vec3,glm::vec3>> constructApproximation(uint n,uint m){
		float inv_n = 1/static_cast<float>(n),
			  inv_m = 1/static_cast<float>(m);

		std::vector<std::pair<glm::vec3,glm::vec3>> position_normal;

		for( uint i=0; i<n; ++i )
			for( uint j=0; j<m; ++j )
				position_normal.push_back( 
					std::pair<glm::vec3,glm::vec3>
						( getPoint(i*inv_n,j*inv_m), getNormal(i*inv_n,j*inv_m) ) );

		return position_normal;
	}
	
private:

	// constructs curve for v parameter
	BezierCurve _constructCurve_Y(float u) const {
		PointArray_t::value_type subCurves(controlpoints.size());

		auto control_iter = controlpoints.begin();
		for( PointArray_t::value_type::iterator
				sub_iter  = subCurves.begin();
				sub_iter != subCurves.end();
				++sub_iter,++control_iter ){
				 *sub_iter = BezierCurve(*control_iter).getPoint(u);
		}
		return BezierCurve(subCurves);
	}
	// constructs curve for u parameter
	// more expensive since parameters are saved x major
	BezierCurve _constructCurve_X(float v) const {
		PointArray_t::value_type subCurves(controlpoints[0].size());
		PointArray_t::value_type tmp(controlpoints.size());

		for( int j = 0; j < subCurves.size(); ++j ) {
				for( int i = 0; i<controlpoints.size(); ++i)
					tmp.at(i) = controlpoints[i][j];
				 subCurves[j] = BezierCurve(tmp).getPoint(v);
		}
		return BezierCurve(subCurves);
	}

};

GRAPHICS_END

#endif//MFL_GRAPHICS_BEZIER_INCLUDED