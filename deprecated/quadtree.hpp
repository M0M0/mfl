#ifndef MFL_QUADTREE_INCLUDED
#define MFL_QUADTREE_INCLUDED

#include "MFL_LinAlg.hpp"
#include <bitset>
#include <memory>

namespace MFL{

enum QT_SubSpace { LL = 0,LR = 1,UL = 2,UR = 3 };

template <typename T,int D>
class BoundingBox {
  typedef LinAlg::Vector<T,D> vec_t;
  typedef unsigned int		uint;
  
 public:
  BoundingBox(vec_t center,vec_t half_space)
    : center_(center),
      half_space_(half_space) {
  }
  
  BoundingBox(BoundingBox bb) : center_(bb.center_), halfSpace_(bb.halfSpace_) {
  }

  bool Contains(vec_t point) {
    vec_t upper_limit = center_ + half_space_,
      lower_limit = center_ - half_space_;
    for (int i = 0;i < D; ++i)
      if (lower_limit(i) > point(i) || upper_limit(i) < point(i))
	return false;
    return true;
  }

  uint SelectSubvolume(vec_t point) {
    std::bitset<D> bitindex;
    for(int i=0;i<D;++i)
      bitindex[i] = point(i) > center_(i) ? 1 : 0;
    return bitindex.to_ulong();
  }
  BoundingBox ReturnSubvolume(uint index) {
    std::bitset<D> bitindex(index);
    vec_t center = center_, halfSpace = half_space_ / 2;
    for (int i = 0;i < D; ++i)
      center(i) += halfSpace(i) * (bitindex[i] ? 1 : -1);
    return BoundingBox(center,halfSpace);
  }
 private:
  vec_t center_;
  vec_t half_space_;
};

template <typename T>
class Quadtree{
  typedef BoundingBox<T,2> BB_t;
  typedef MFL::LinAlg::Vector<T,2> vec_t;
 public:
  Quadtree(BB_t bb) : _ptr(nullptr),bb_(bb){
    std::fill(child,child_+N_C,std::unique_ptr<Quadtree>(nullptr));
  }

  bool Insert(vec_t point){
    if(!bb_.Contains(point)) return false;
    
    if (IsLeaf()) {
      if (!ptr_) {
	ptr_ = new vec_t(point);
	return true;
      } else if( *ptr_ == point) {
	return true;
      }
      // remove point and initate childs for both values
      auto idx = bb_.SelectSubvolume(*ptr_);
      if (!child_[idx])
	child_[idx] = new Quadtree(bb_.ReturnSubvolume(idx));
      if (!child[idx]_->Insert(*ptr_))// maybe a throw is more appropriate
	return false;
      ptr_ = nullptr;
    }
    // if it is not a leaf just continue otherwise just continue anyway
    auto index = bb_.SelectSubvolume(point);
    if (!child_[index])
      child_[index] = new Quadtree(bb_.ReturnSubvolume(index));
    return child_[index]->Insert(point);
  }
  // might be preferable to vectors only!
  template <template <class> class container>
  bool Insert(container<vec_t> points){
    bool returnvalue = true;
    for (auto& iter : points) returnvalue &= Insert(*iter);
    return returnvalue;
  }
  bool IsLeaf(){
    for (int i=0;i<N_C;++i)
      if (child_[i]) return false;
    return true;
  }
private:
  static const int N_C = 1<<2;
  std::uniqueptr<Quadtree> child_[N_C];
  BB_t bb_; 
  std::uniqueptr<vec_t> ptr_;
};
  
// should serve as an extension/generalisation of quadtree -> not finished!
template <typename T,int D> // quad/oct tree
class SpacingTree{
  typedef LinAlg::Vector<T,D> vec_t;
  typedef BoundingBox<T,D> BB_t;
public:
  SpacingTree(BB_t bb) : bb_(bb),ptr_(nullptr){
    std::fill(child_,child_+NumC_,std::unique_ptr<Quadtree>(nullptr));
  }
  
  bool insert(vec_t point){
    if (!bb_.Contains(point)) return false;
    if (IsLeaf()) {
      if (!ptr_) {
	ptr_ = new vec_t(point);
	return true;
      } else if (*ptr_ == point) return true;
      
      // remove point and initate childs for both values
      auto idx = bb_.SelectSubvolume(*ptr_);
      if (!child_[idx])
	child_[idx] = new SpacingTree(bb_.ReturnSubvolume(idx));
      if (!child_[idx]->Insert(*ptr_))// maybe a throw is more appropriate
	return false;
      ptr_ = nullptr;
    }
    // if it is not a leaf just continue otherwise just continue anyway
    auto index = bb_.SelectSubvolume(point);
    if(!child_[index])
      child_[index] = new SpacingTree(bb_.ReturnSubvolume(index));
    return child_[index]->Insert(point);
  }
  
  bool IsLeaf(){
    for (int i = 0;i < NumC_;++i) 
      if (child_[i]) return false;
    return true;
  }
private:
  static const int NumC_ = 1<<D;
  BB_t bb_;
  std::unique_ptr<SpacingTree>	child_[NumC_];
  std::unique_ptr<vec_t> ptr_;
};

}

#endif//MFL_QUADTREE_INCLUDED
