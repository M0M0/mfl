#ifndef MFL_QUADTREE_INCLUDED
#define MFL_QUADTREE_INCLUDED

#include "MFL_LinAlg.hpp"
#include <bitset>
#include <memory>

namespace MFL{

enum class QT_SubSpace : unsigned char{ LL=0,LR=1,UL=2,UR=3 };

	template <typename T,int D>
	class BoundingBox{
		typedef LinAlg::Vector<T,D> vec_t;
		typedef unsigned int		uint;

	public:
		BoundingBox(vec_t center,vec_t halfSpace)
			:_center(center),_halfSpace(halfSpace){}

		BoundingBox(BoundingBox&& bb)
			:_center(std::forward<vec_t>(bb._center)),
			 _halfSpace(std::forward<vec_t>(bb._halfSpace)){}

		bool contains(vec_t point)
		{
			vec_t UpperLimit = _center + _halfSpace,
				  LowerLimit = _center - _halfSpace;
			for(int i=0;i<D;++i)
				if(LowerLimit(i)>point(i)||UpperLimit(i)<point(i))
					return false;

			return true;
		}

		uint selectSubvolume(vec_t point)
		{
			std::bitset<D> bitindex;
			for(int i=0;i<D;++i)
				bitindex[i] = point(i) > _center(i) ? 1 : 0;
			return bitindex.to_ulong();
		}
		BoundingBox returnSubvolume(uint index)
		{
			std::bitset<D> bitindex(index);
			vec_t center = _center, halfSpace = _halfSpace * (1/2);
			for(int i=0;i<D;++i)
				center(i) += bitindex[i]?halfSpace(i):(-halfSpace(i));

			return BoundingBox(center,halfSpace);
		}
	private:
		vec_t _center;
		vec_t _halfSpace;
	};

	template <typename T>
	class Quadtree{
		typedef BoundingBox<T,2>			BB_t;
		typedef MFL::LinAlg::Vector<T,2>	vec_t;
	public:
		Quadtree(BB_t bb):_ptr(nullptr),_bb(bb){
			std::fill(_child,_child+N_C,std::unique_ptr<Quadtree>(nullptr));
		}

		bool insert(vec_t point){
			if(!_bb.contains(point))
				return false;

			if(isleaf()){
				if(!_ptr){
					_ptr = new vec_t(point);
					return true;
				}
				else if( *_ptr==point)
					return true;
				
				// remove point and initate childs for both values
				auto idx = _bb.selectSubvolume(*_ptr);
				if(!_child[idx])
					_child[idx] = new Quadtree(_bb.returnSubvolume(idx));
				if(!_child[idx]->insert(*_ptr))// maybe a throw is more appropriate
					return false;
				_ptr = nullptr;
				
			}
			// if it is not a leaf just continue otherwise just continue anyway
			auto index = _bb.selectSubvolume(point);
			if(!_child[index])
				_child[index] = new Quadtree(_bb.returnSubvolume(index));

			return _child[index]->insert(point);
		}
		// might be preferable to vectors only!
		template <template <class> class container>
		bool insert(container<vec_t> points){
			bool returnvalue = true;
			for( auto& iter : points )
				returnvalue &= insert(*iter);
				
			return returnvalue;
		}
		bool isleaf(){
			for(int i=0;i<N_C;++i)
				if(_child[i])
					return false;
			return true;
		}
	private:
		static const int	N_C = 1<<2;

		std::unique_ptr<Quadtree>	_child[N_C];
		BB_t						_bb; 
		std::unique_ptr<vec_t>		_ptr;
	};

	// should serve as an extension/generalisation of quadtree -> not finished!
	template <typename T,int D> // quad/oct tree
	class SpacingTree{
		typedef LinAlg::Vector<T,D> vec_t;
		typedef BoundingBox<T,D>	BB_t;
	public:
		SpacingTree(BB_t bb):_bb(bb),_ptr(nullptr){
			std::fill(_child,_child+_NumC,std::unique_ptr<Quadtree>(nullptr));
		}

		bool insert(vec_t point){
			if(!_bb.contains(point))
				return false;

			if(isleaf()){
				if(!_ptr){
					_ptr = new vec_t(point);
					return true;
				}
				else if( *_ptr==point)
					return true;
				
				// remove point and initate childs for both values
				auto idx = _bb.selectSubvolume(*_ptr);
				if(!_child[idx])
					_child[idx] = new SpacingTree(_bb.returnSubvolume(idx));
				if(!_child[idx]->insert(*_ptr))// maybe a throw is more appropriate
					return false;
				_ptr = nullptr;
				
			}
			// if it is not a leaf just continue otherwise just continue anyway
			auto index = _bb.selectSubvolume(point);
			if(!_child[index])
				_child[index] = new SpacingTree(_bb.returnSubvolume(index));

			return _child[index]->insert(point);
		}

		bool isleaf(){
			for(int i=0;i<_NumC;++i)
				if(_child[i])
					return false;
			return true;
		}
	private:
		static const int	_NumC = 1<<D;

		BB_t							_bb;
		std::unique_ptr<SpacingTree>	_child[_NumC];
		std::unique_ptr<vec_t>			_ptr;
	};

}

#endif//MFL_QUADTREE_INCLUDED