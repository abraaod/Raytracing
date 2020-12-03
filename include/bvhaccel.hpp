#ifndef _BVHACCEL_
#define _BVHACCEL_

#include "bounds3.hpp"
#include <memory>
#include <vector>
class BvhAccel {
    public:
        BvhAccel() {}

        std::shared_ptr<BvhAccel> buildTree(const std::vector<std::shared_ptr<Bounds3>>& b, size_t start, size_t end);

        ~BvhAccel() = default;

        std::shared_ptr<BvhAccel> left = nullptr;
        std::shared_ptr<BvhAccel> right = nullptr;
        std::shared_ptr<Bounds3> box = nullptr;
        std::vector<std::shared_ptr<GeometricPrimitive>> g;

        void printBVH(){
            if(left != nullptr){
                left->printBVH();
            } else if(right != nullptr){
                right->printBVH();
            } else if(left == nullptr and right == nullptr){
                for(int i = 0; i< g.size(); i++){
                    g[i]->printCenter();
                }
            }
            
        }
};
inline bool comparator(const std::shared_ptr<Bounds3> a, const std::shared_ptr<Bounds3> b) {
	
	return a->pMin.v1 < b->pMin.v1;
	
}
std::shared_ptr<BvhAccel> BvhAccel::buildTree(const std::vector<std::shared_ptr<Bounds3>>& src_objects, size_t start, size_t end) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    // int axis = random_int(0,2);
    // auto comparator = (axis == 0) ? box_x_compare
    //                 : (axis == 1) ? box_y_compare
    //                               : box_z_compare;

    size_t object_span = end - start;
    std::cout << object_span << std::endl;
    if (object_span == 1) {
        left = std::make_shared<BvhAccel>();
        left->box = objects[start];
        left->g.push_back(objects[start]->geo);
        //right = nullptr;
        //left = right = objects[start];
    } else if (object_span == 2) {
        left = std::make_shared<BvhAccel>();
        right = std::make_shared<BvhAccel>();
        left->box = objects[start];
        left->g.push_back(objects[start]->geo);
        right->box = objects[start+1];
        right->g.push_back(objects[start]->geo);
        // if (comparator(objects[start], objects[start+1])) {
        //     left->box = objects[start];
        //     right->box = objects[start+1];
        // } else {
        //     left->box = objects[start+1];
        //     right->box = objects[start];
        // }
    } else {
        auto mid = start + object_span/2;
        left = buildTree(objects, start, mid);
        right = buildTree(objects, mid, end);
    }

    std::cout << "FINALIZOU" << std::endl;
    for(int i = 0; i< g.size(); i++){
                    g[i]->printCenter();
                }
    if(right ==  nullptr){
        box = left->box;
        g.push_back(box->geo);
    } else {
        box = unionBounds(left->box, right->box);
        for(int i = 0; i< left->g.size(); i++){
            this->g.push_back(left->g[i]);
        }
        for(int i = 0; i< right->g.size(); i++){
            this->g.push_back(right->g[i]);
        }
    }

   //box = unionBounds(left->box, right->box);
    return std::make_shared<BvhAccel>(*this);
}
#endif