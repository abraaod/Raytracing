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
	void accel(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
    std::shared_ptr<Bounds3> buildTree(std::vector<std::shared_ptr<Bounds3>> bounds, size_t start, size_t end, float tmin, float tmax);

    bool intersect_p(Ray &r, float tmin, float tmax, std::vector<std::shared_ptr<GeometricPrimitive>> * geo);

    virtual bool bounding_box(float t0, float t1, Bounds3& box) const;

    std::shared_ptr<Bounds3>  left;
    std::shared_ptr<Bounds3>  right;
    std::shared_ptr<Bounds3> box;

	std::shared_ptr<GeometricPrimitive> geoleft;
	std::shared_ptr<GeometricPrimitive> georight;
	std::vector<std::shared_ptr<Bounds3>> bounds;
};

void Bvh_node::accel(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
	for(int i = 0; i < obj_list.size(); i++){
		
		auto ge = obj_list[i]->world_bounds();
		std::shared_ptr<Bounds3> aux = std::make_shared<Bounds3>(ge);

		//obj_list[i]->get_Shape()->printCenter();
		aux->setGeoPrimitive(obj_list[i]);
		bounds.push_back(aux);
	}
}

bool Bvh_node::bounding_box(float t0, float t1, Bounds3& outbox) const{
	outbox = box;
	return true;
}

bool box_x_compare(const std::shared_ptr<Bounds3> a, const std::shared_ptr<Bounds3> b) {
	
	return a->pMin.v1 < b->pMin.v1;
	
}

bool box_y_compare(const std::shared_ptr<Bounds3> a, const std::shared_ptr<Bounds3> b) {
	
	return a->pMin.v2 < b->pMin.v2;
	
}

bool box_z_compare(const std::shared_ptr<Bounds3> a, const std::shared_ptr<Bounds3> b) {
	
	return a->pMin.v3 < b->pMin.v3;
	
}

std::shared_ptr<Bounds3> Bvh_node::buildTree(std::vector<std::shared_ptr<Bounds3>> b, size_t start, size_t end, float tmin, float tmax){
    int axis = int(3 * (rand() % 100 / float(100)));
	if(axis == 0){
		std::sort(bounds.begin() + start, bounds.begin() + end, box_x_compare);
	} else if (axis == 1){
		std::sort(bounds.begin() + start, bounds.begin() + start, box_y_compare);
	} else {
		std::sort(bounds.begin() + start, bounds.begin() + start, box_z_compare);
	}

	size_t  spanning = end - start;
	//std::cout << spanning << std::endl;

	if(spanning == 1){

		left =  right = bounds[start];
		geoleft = georight = bounds[start]->geo;
	} else if (spanning == 2){
		left = bounds[start];
		geoleft = bounds[start]->geo;
		right = bounds[start+1];
		georight = bounds[start+1]->geo;
	} else {

		size_t mid =  start + spanning/2;
		left = buildTree(b, start, mid, tmin, tmax);
		geoleft  =left->geo;
		right = buildTree(b, mid, end, tmin, tmax);
		georight = right->geo;
	}

	//std::cout << "FINALIZOU\n";


	box = std::make_shared<Bounds3>(unionBounds(*left, *right));
	return box;
}

bool Bvh_node::intersect_p(Ray &r, float tmin, float tmax, std::vector<std::shared_ptr<GeometricPrimitive>> * g){
	//std::shared_ptr<GeometricPrimitive> start = std::make_shared<GeometricPrimitive>(geo(0));
	if(box->intersect_p(r) == nullptr){
		return false;
	}

	// std::shared_ptr<GeometricPrimitive> leftgeo;
	// std::shared_ptr<GeometricPrimitive> rightgeo;

	auto hit_left = left->intersect_p(r, &tmin, &tmax);
	auto hit_right = right->intersect_p(r, &tmin, &tmax);
	if (hit_left != nullptr && hit_right != nullptr) {
		g->push_back(geoleft);
		g->push_back(georight);
		geoleft->printCenter();
		georight->printCenter();
		return true;
	} else if (hit_left != nullptr) {
		geoleft->printCenter();
		g->push_back(geoleft);
		return true;
	} else if (hit_right != nullptr) {
		g->push_back(georight);
		georight->printCenter();
			return true;
	} else {
		return false;
	}
		
	

	// bool hit_left = left->intersect_p;
    // bool hit_right = right->intersect_p(r, tmin, hit_left ? ray->tmax : tmax, geo);

    //return hit_left || hit_right;
}
#endif