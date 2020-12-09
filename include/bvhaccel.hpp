#ifndef _BVHACCEL_
#define _BVHACCEL_

#include "bounds3.hpp"
#include <memory>
#include <vector>
class BvhAccel
{
public:
    BvhAccel() {}

    std::shared_ptr<BvhAccel> buildTree(const std::vector<std::shared_ptr<Bounds3>> &b, size_t start, size_t end);
    bool hit(Ray &r, float t_min, float t_max, std::vector<std::shared_ptr<GeometricPrimitive>> &b)
    {
        if (!box->intersect_p(r, &t_min, &t_max))
        {
            return false;
        }

        if(left == nullptr and right == nullptr){
            if(box->geo->intersect_p(r)){
                b.push_back(box->geo);
                return true;
            } else {
                return false;
            }
            // b.push_back(box->geo);
            // return true;
        }
        
        bool hit_left, hit_right;
        if(left != nullptr)
            hit_left = left->hit(r, t_min, t_max, b);

        if(right != nullptr)
            hit_right = right->hit(r, t_min, t_max, b);

        return hit_left || hit_right;
        
    }

    ~BvhAccel() = default;

    std::shared_ptr<BvhAccel> left = nullptr;
    std::shared_ptr<BvhAccel> right = nullptr;
    std::shared_ptr<Bounds3> box = nullptr;
    //std::vector<std::shared_ptr<GeometricPrimitive>> g;

    void printBVH()
    {
        if (left == nullptr and right == nullptr)
        {
            if (box->geo != nullptr)
            {
                box->geo->printCenter();
            }
            //
        }

        if (left != nullptr)
        {
            left->printBVH();
            // std::cout << "Imprimindo box" << std::endl;
            // left->box->pMin.print();
            // left->box->pMax.print();
            // std::cout << "finalizando box" << std::endl;
        }

        if (right != nullptr)
        {
            right->printBVH();
            // std::cout << "Imprimindo box" << std::endl;
            // right->box->pMin.print();
            // right->box->pMax.print();
            // std::cout << "finalizando box" << std::endl;
        }
    }
};
inline bool comparator(const std::shared_ptr<Bounds3> a, const std::shared_ptr<Bounds3> b)
{

    return a->pMin.v1 < b->pMin.v1;
}
std::shared_ptr<BvhAccel> BvhAccel::buildTree(const std::vector<std::shared_ptr<Bounds3>> &src_objects, size_t start, size_t end)
{
    auto objects = src_objects; // Create a modifiable array of the source scene objects
    std::cout << "PASS\n"
              << std::endl;
    int axis = int(3 * (rand() % 100 / float(100)));
    if (axis == 0)
    {
        std::sort(objects.begin(), objects.end(), box_x_compare);
    }
    else if (axis == 1)
    {
        std::sort(objects.begin(), objects.end(), box_y_compare);
    }
    else
    {
        std::sort(objects.begin(), objects.end(), box_z_compare);
    }

    size_t object_span = end - start;
    std::cout << object_span << std::endl;
    if (object_span == 1)
    {

        left = std::make_shared<BvhAccel>();
        left->box = objects[start];
        objects[start]->geo->printCenter();

        return left;
    }
    else if (object_span == 2)
    {
        auto root = std::make_shared<BvhAccel>();
        root->left = std::make_shared<BvhAccel>();
        root->right = std::make_shared<BvhAccel>();

        root->left->box = objects[start];
        objects[start]->geo->printCenter();

        root->right->box = objects[start + 1];
        objects[start + 1]->geo->printCenter();

        root->box = unionBounds(root->right->box, root->left->box);
        return root;
    }
    else
    {
        auto mid = start + object_span / 2;
        auto root = std::make_shared<BvhAccel>();

        std::vector<std::shared_ptr<Bounds3>> v1;
        for (int i = start; i < mid; i++)
        {
            v1.push_back(objects[i]);
        }

        std::vector<std::shared_ptr<Bounds3>> v2;
        for (int i = mid; i < end; i++)
        {
            v2.push_back(objects[i]);
        }

        root->left = buildTree(v1, 0, mid);
        root->right = buildTree(v2, 0, end - mid);

        root->box = unionBounds(root->right->box, root->left->box);
        return root;
    }
}
#endif