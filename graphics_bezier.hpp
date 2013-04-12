#ifndef MFL_GRAPHICS_BEZIER_INCLUDED
#define MFL_GRAPHICS_BEZIER_INCLUDED

#include <type_traits>
#include <vector>

#include <glm/glm.hpp>

#include "graphics_common.hpp"

GRAPHICS_BEGIN

/// \class BezierCurve
/// \brief A simple implementation of a Bezier curve.
/// \details This Class uses the headers vector and glm.
/// It supports variable size Bezier curves.
/// Functionality includes curve approximation using de Casteljau's algorithm
/// and extraction  of the derivative of the curve.
/// \author mojo
class BezierCurve{
	typedef std::vector<glm::vec3> Points;
public:
	BezierCurve()
		: controlpoints_() {}
    /// Constructor for passing iterators.
    /// \param[in] first  Iterator for the first point.
    /// \param[in] last   Iterator after the last point.
    template <typename Iterator>
	BezierCurve(Iterator first,Iterator last) : controlpoints_(first,last) {
        static_assert(
            std::is_same<
                std::iterator_traits<Iterator>::value_type,
                glm::vec3
            >::value,"Value type of iterator is not \'glm::vec3\'.");
    }

    /// Appends a point to the end of the list.
    /// \param[in] x   X-coordinate of the point.
    /// \param[in] y   Y-coordinate of the point.
    /// \param[in] z   Z-coordinate of the point.
	void AddPoint(float x,float y,float z) {
		AddPoint(glm::vec3(x,y,z));
	}
    /// Appends a point to the end of the list.
    /// \param[in] p Coordinates of the point.
	void AddPoint(glm::vec3 p) {
		controlpoints_.push_back(p);
	}

    /// Calculates a point of the Bezier curve.
    /// \details This function uses the recursive de Castlejau's algorithm.
    /// Which can be written as 
    /// \f$ B_{i}^{n}(t):=(1-t)B_{i-1}^{n}(t)+tB_{i-1}^{n+1}(t)\f$
    /// with \f$ B_{0}^{n}(t):= P_{n}\f$
    /// where \f$ P_{n}\f$ is the n-th control point.
    /// \param[in] t  Parameter describing a point on the curve.
    /// Must be between 0 and 1.
    /// \return Returns a point on the curve.
	glm::vec3 GetPoint(float t) const {
		if(controlpoints_.size() < 2) return glm::vec3(0);
		float inv_t = 1 - t;
		auto subCurve = controlpoints_;
		for (uint i = 1;i < controlpoints_.size(); ++i) {
			for (auto it = subCurve.begin();it + 1 != subCurve.end(); ++it) {
				*it = *it * inv_t + *(it + 1) * t;
			}
			subCurve.pop_back();
		}
		return subCurve[0];
	}
    /// Derivative BezierCurve.
    /// \details The derivative curve is exactly the tangent for every point.
    /// This value might not be correct for \f$ t = 0 \f$ and \f$ t = 1 \f$.
    /// \return Returns the derivative curve.
	BezierCurve GetTangentCurve() const {
		Points derivate_array;

		float n = static_cast<float>(controlpoints_.size() - 1);
		for (auto iter = controlpoints_.begin();
             iter+1 != controlpoints_.end(); ++iter) {
			derivate_array.push_back(n * (*(iter + 1) - *iter));
		}
		return BezierCurve(derivate_array);
	}
    /// Polynomial degree of the curve.
    /// \details The degree is the number of control points minus 1.
    /// \return Returns either 0 or a positive integer.
	uint GetDegree() const {
		return static_cast<uint>(controlpoints_.size() - 1);
	}
    /// Constructs an approximation of the resulting curve.
    /// \details Calculates an approximation including the endpoints.
    /// \param[in] n Number of points.
    /// \return Returns the points of the curve-approximation.
	std::vector<glm::vec3> constructApproximation(uint n) const {
		float inv_n = 1 / static_cast<float>(n - 1);
		Points points;
		for (uint i = 0;i < n; ++i)	points.push_back(GetPoint(i * inv_n));
		return points;
	}
private:
	Points controlpoints_; ///< The controlpoints of the bezier curve.
};

/// \class BezierSurface
/// \brief A simple implementation of a Bezier surface.
/// \details This Class uses the headers vector and glm.
/// It supports variable size Bezier surfaces and uses BezierCurve.
/// Functionality includes curve approximation using de Casteljau's algorithm.
/// \todo Switch to a generic field for member points.
/// \author mojo
class BezierSurface{
	typedef std::vector< std::vector<glm::vec3> > Points;
public:
	BezierSurface() : controlpoints() {}
	BezierSurface(std::vector< std::vector<glm::vec3> > points)
		: controlpoints(points) {}
    /// Constructor for passing iterators.
    /// \todo Do some typechecks on the iterators.
    /// \param[in] first  Iterator for the first point.
    /// \param[in] last   Iterator after the last point.
    template <typename Iterator>
	BezierSurface(Iterator first,Iterator last) : controlpoints(first,last) {
        //static_assert(
          //std::is_same<
           //     std::iterator_traits<Iterator>,std::vectoglm::vec3>
          //              ::value,"Value type of iterator is not \'glm::vec3\'.");
    }

	BezierCurve& GetArray(uint index) {
		if (controlpoints.size() <= index) controlpoints.resize(index + 1);
		return *reinterpret_cast<BezierCurve*>(&controlpoints[index]);
	}

	glm::vec3 GetPoint(float u,float v) const {
		// The following should also work:
		// return _constructCurve_X(v).getPoint(u);
		// but due to x major ordering might be less efficient
		return _ConstructCurve_Y(u).GetPoint(v);
	}
	glm::vec3 GetNormal(float u,float v) const {
		glm::vec3 u_tangent = _ConstructCurve_X(v).GetTangentCurve().GetPoint(u);
		glm::vec3 v_tangent = _ConstructCurve_Y(u).GetTangentCurve().GetPoint(v);
		return glm::normalize(glm::cross(u_tangent,v_tangent));
	}
    /// Approximates the surface points and normals.
    /// \details Returns resulting position and if a pointer is passed to
    /// the third argument also normals.
    /// Watch out! The function calls clear on the normal container!
    /// \param[in]  n       Number of points in the first direction.
    /// \param[in]  m       Number of points in the second direction.
    /// \param[out] normal  Pointer to a container
    ///  for storage of surface normals.(optional)
    /// \return Returns the surface positions.
	std::vector<glm::vec3> ConstructApproximation(
            uint n,
            uint m,
            std::vector<glm::vec3> *normal = nullptr) {
		float inv_n = 1 / static_cast<float>(n),
			  inv_m = 1 / static_cast<float>(m);
        std::vector<glm::vec3> position;
        if (normal) {
            normal->clear();
            for (uint i=0;i < n;++i)
                for (uint j=0;j < m;++j) {
                    position.push_back(GetPoint(i * inv_n,j * inv_m));
                    normal->push_back(GetNormal(i * inv_n,j * inv_m));
                }
        } else {
            for (uint i=0;i < n;++i)
                for (uint j=0;j < m;++j)
                    position.push_back(GetPoint(i * inv_n,j * inv_m));
        }
		return position;
	}
	
private:
	// constructs curve for v parameter
	BezierCurve _ConstructCurve_Y(float u) const {
		Points::value_type subCurves(controlpoints.size());

		auto control_iter = controlpoints.begin();
		for (Points::value_type::iterator sub_iter = subCurves.begin();
			 sub_iter != subCurves.end(); ++sub_iter,++control_iter) {
            *sub_iter = BezierCurve(*control_iter).GetPoint(u);
		}
		return BezierCurve(subCurves);
	}
	// constructs curve for u parameter
	// more expensive since parameters are saved x major
	BezierCurve _ConstructCurve_X(float v) const {
		Points::value_type subCurves(controlpoints[0].size());
		Points::value_type tmp(controlpoints.size());

		for (int j = 0;j < subCurves.size();++j) {
            for (int i = 0;i < controlpoints.size(); ++i)
                tmp.at(i) = controlpoints[i][j];
			subCurves[j] = BezierCurve(tmp).GetPoint(v);
		}
		return BezierCurve(subCurves);
	}

 	Points controlpoints; ///< The controlpoints of the bezier surface.
};

GRAPHICS_END

#endif//MFL_GRAPHICS_BEZIER_INCLUDED