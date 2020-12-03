#ifndef _BVH_
#define _BVH_

#include "primitive.hpp"
#include "geometricprimitive.hpp"
#include "bounds3.hpp"
#include <memory>
#include <algorithm>

class Bvh_node {
	public:
    Bvh_node() {}
	std::vector<std::shared_ptr<Bounds3>> accel(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
    std::shared_ptr<Bvh_node> buildTree(std::vector<Bounds3> bounds, size_t start, size_t end, float tmin, float tmax);
	void printBVH(std::shared_ptr<Bvh_node> root);

    bool intersect_p(Bounds3 box, Ray &r, float tmin, float tmax, std::vector<std::shared_ptr<GeometricPrimitive>> * geo);

    virtual bool bounding_box(float t0, float t1, Bounds3& box) const;

    std::shared_ptr<Bvh_node>  left = nullptr;
    std::shared_ptr<Bvh_node>  right = nullptr;
    std::shared_ptr<Bounds3> box;

	//std::vector<std::shared_ptr<Bounds3>> bounds;
	~Bvh_node() = default;
};

std::vector<std::shared_ptr<Bounds3>> Bvh_node::accel(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
	std::vector<std::shared_ptr<Bounds3>> bounds;
	for(int i = 0; i < obj_list.size(); i++){
		
		Bounds3 ge = obj_list[i]->world_bounds();
		//ge.setGeoPrimitive(obj_list[i]);
		std::shared_ptr<Bounds3> aux = std::make_shared<Bounds3>(ge);

		aux->setGeoPrimitive(obj_list[i]);
		bounds.push_back(aux);
	}

	return bounds;
}

bool Bvh_node::bounding_box(float t0, float t1, Bounds3& outbox) const{
	outbox = box;
	return true;
}

bool box_x_compare(const Bounds3 a, const Bounds3 b) {
	
	return a.pMin.v1 < b.pMin.v1;
	
}

bool box_y_compare(const Bounds3 a, const Bounds3 b) {
	
	return a.pMin.v2 < b.pMin.v2;
	
}

bool box_z_compare(const Bounds3 a, const Bounds3 b) {
	
	return a.pMin.v3 < b.pMin.v3;
	
}

std::shared_ptr<Bvh_node> Bvh_node::buildTree(std::vector<Bounds3> b, size_t start, size_t end, float tmin, float tmax){
    // int axis = int(3 * (rand() % 100 / float(100)));
	// if(axis == 0){
	// 	std::sort(b.begin(), b.end(), box_x_compare);
	// } else if (axis == 1){
	// 	std::sort(b.begin(), b.end(), box_y_compare);
	// } else {
	// 	std::sort(b.begin(), b.end(), box_z_compare);
	// }

	size_t  spanning = end - start;
	std::cout << spanning << std::endl;

	if(spanning == 1){
		this->left = std::make_shared<Bvh_node>();
		//this->left->bounds.push_back(std::make_shared<Bounds3>(b[start]));
		this->left->box = std::make_shared<Bounds3>(b[start]);
		this->box = this->left->box;

	} else if(spanning == 2){
		this->left = std::make_shared<Bvh_node>();
		//this->left->bounds.push_back(std::make_shared<Bounds3>(b[start]));
		this->left->box = std::make_shared<Bounds3>(b[start]);
		this->right = std::make_shared<Bvh_node>();
		//this->right->bounds.push_back(std::make_shared<Bounds3>(b[start]));
		this->right->box = std::make_shared<Bounds3>(b[start]);
		this->box = unionBounds(this->left->box, this->right->box);

	} else {
		size_t mid = start + spanning/2;
		std::vector<Bounds3> vleft;
		std::vector<Bounds3> vright;
		for(int i = start; i < mid; i++){
			vleft.push_back(b[i]);
		}
		for(int i = start + mid;  i < end; i++){
			vright.push_back(b[i]);
		}

		for(int i = 0; i < vleft.size(); i++){
        	vleft[i].geo->printCenter();
    	}
		std::cout << "left side" << std::endl;

		for(int i = 0; i < vright.size(); i++){
        	vright[i].geo->printCenter();
    	}
		std::cout << "right side" << std::endl;
		// std::vector<Bounds3> vleft(b.begin(), b.begin() + mid);
		// std::vector<Bounds3> vright(b.begin() + mid, b.end());
		this->left = buildTree(vleft, start, mid, tmin, tmax);
		this->right = buildTree(vright, mid, end, tmin, tmax);
	}

	//std::shared_ptr<Bvh_node> a
	return std::make_shared<Bvh_node>(*this);
}

void Bvh_node::printBVH(std::shared_ptr<Bvh_node> root){
	// for(int i = 0; i < this->bounds.size(); i++){
	// 		this->left->bounds[i]->geo->printCenter();
	// 	}
}
// bool Bvh_node::intersect_p(Bounds3 b, Ray &r, float tmin, float tmax, std::vector<std::shared_ptr<GeometricPrimitive>> * geo){
// 	//std::shared_ptr<GeometricPrimitive> start = std::make_shared<GeometricPrimitive>(geo(0));
// 	if(!box->intersect_p(r, &tmin , &tmax)){
// 		return false;
// 	} 

// 	if(this->left == nullptr and this->right == nullptr){
// 		// box->left->printCenter();
// 		// box->right->printCenter();
// 		for(int i = 0; i < this->bounds.size(); i++){
// 			this->bounds[i]->geo->printCenter();
// 		}
// 	} else if(this->left != nullptr){
// 		intersect_p(this->left->box, r, tmin, tmax, geo);
// 	} else {
// 		intersect_p(this->right->box, r, tmin, tmax, geo);
// 	}

// 	return true;

// 	// std::shared_ptr<GeometricPrimitive> leftgeo;
// 	// std::shared_ptr<GeometricPrimitive> rightgeo;
		
	

// 	// bool hit_left = left->intersect_p;
//     // bool hit_right = right->intersect_p(r, tmin, hit_left ? ray->tmax : tmax, geo);

//     //return hit_left || hit_right;
// }
#endif