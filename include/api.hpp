#ifndef _API_
#define _API_

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "paramset.hpp"
#include "vec.hpp"
#include "scene.hpp"
#include "geometricprimitive.hpp"

#include "sphere.hpp"
#include "triangle.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "flatmaterial.hpp"
#include "blinnmaterial.hpp"
#include "aggregateprimitive.hpp"
#include "integrator.hpp"
#include "light/light.hpp"
#include "light/ambient.hpp"
#include "light/directional.hpp"
#include "light/point.hpp"
#include "light/spot.hpp"
#include "triangle.hpp"
#include "bvhaccel.hpp"

#include <time.h>

class Api
{
private:
    /* data */
    Camera *camera;

    Film *film;

    Background *background;
    //Material* mat;
    std::vector<std::shared_ptr<GeometricPrimitive>> obj_list_;
    std::vector<std::shared_ptr<Light>> light_list;
    Scene *scene;

    Integrator *integrator;

public:
    Api(/* args */);
    ~Api();

    void CAMERA(Paramset<std::string, std::string> ps);

    void FILM(Paramset<std::string, std::string> ps);

    void BACKGROUND(Paramset<std::string, std::string> ps);

    void MATERIAL(Paramset<std::string, std::string> ps);

    void OBJECTS(std::vector<std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>>> ps);

    void INTEGRATOR(Paramset<std::string, std::string> ps);

    void LIGHTS(std::vector<Paramset<std::string, std::string>> ps);

    Camera getCamera();

    Film getFilm();

    Background getBackground();

    void render();
};

Api::Api(/* args */) : camera{nullptr}, film{nullptr}, background{nullptr}, integrator{nullptr}
{
    scene = new Scene();
}

Api::~Api()
{
    if (scene)
        delete scene;
    if (camera)
        delete camera;
    if (film)
        delete film;
    if (background)
        delete background;
    if (integrator)
        delete integrator;
}

void Api::CAMERA(Paramset<std::string, std::string> ps)
{
    std::string type = ps.find("type");
    Vec lookat(ps.find("look_at"));
    Vec lookfrom(ps.find("look_from"));
    Vec vup(ps.find("up"));
    std::cout << "vup: " << ps.find("up") << std::endl;
    std::string screen = ps.find("screen_window");

    if (type.compare("orthographic") == 0)
    {
        camera = new OrthograficCamera(type, screen, lookat, lookfrom, vup);
    }
    else if (type.compare("perspective") == 0)
    {
        float fovy = std::stof(ps.find("fovy"));
        camera = new PerspectiveCamera(type, fovy, lookat, lookfrom, vup);
    }

    scene->setCamera(camera);
}

void Api::FILM(Paramset<std::string, std::string> ps)
{
    std::string filename = ps.find("filename");
    std::string img_type = ps.find("img_type");
    std::string type = ps.find("type");
    int x_res = std::stoi(ps.find("x_res"));
    int y_res = std::stoi(ps.find("y_res"));
    film = new Film(filename, img_type, type, x_res, y_res);

    scene->setFilm(film);
}

void Api::BACKGROUND(Paramset<std::string, std::string> ps)
{
    std::string type = ps.find("type");
    std::string color(ps.find("color"));
    if (color != "")
    {
        Vec c(color);
        background = new Background(type, c);
    }
    else
    {
        Vec bl(ps.find("bl"));
        Vec br(ps.find("br"));
        Vec tl(ps.find("tl"));
        Vec tr(ps.find("tr"));
        std::cout << std::endl;

        std::string mapping = ps.find("mapping");
        background = new Background(type, bl, br, tl, tr, mapping);
    }

    scene->setBackground(background);
}

// void Api::MATERIAL(Paramset<std::string, std::string> ps){
//     std::string type = ps.find("type");
//     if(type == "flat"){
//         std::string color = ps.find("color");
//         Vec c(color);
//         mat = new FlatMaterial(c);

//     } else{
//         std::cout << "TIPO DO MATERIAL NÃO COMPATIVEL\n";
//     }
// }

void Api::OBJECTS(std::vector<std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>>> ps)
{

    std::vector<std::shared_ptr<Shape>> mesh;

    for (std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>> p : ps)
    {
        std::string type_object = std::get<1>(p).find("type");
        std::string type_integrator = std::get<0>(p).find("type");

        std::shared_ptr<GeometricPrimitive> geo_pri = std::make_shared<GeometricPrimitive>();

        //std::cout << type_integrator <<  "\n\n\n";

        if (type_object == "sphere")
        {
            //std::cout << "ENTROU\n\n\n";

            float radius_ = std::stof(std::get<1>(p).find("radius"));
            std::string center = std::get<1>(p).find("center");
            // Quebrar os espaços do centro e gerar o vetor centro
            std::vector<std::string> result;
            std::istringstream iss(center);
            for (std::string s; iss >> s;)
                result.push_back(s);
            Vec center_(std::stof(result[0]), std::stof(result[1]), std::stof(result[2]));
            center_.print();
            center_.v4 = 0;
            auto obj_ = std::make_shared<Sphere>(false, center_, radius_);
            geo_pri->set_shape(obj_);
        }

        if (type_object == "trianglemesh")
        {
            bool flip_normals{false};
            //std::string filename = std::get<1>(p).find("filename");
            Paramset<std::string, std::string> p_ = std::get<1>(p);
            mesh = create_triangle_mesh_shape(flip_normals, p_);
            // size_t tri_count{0};
            // for ( const auto & tri : mesh ){
            //     std::cout << "Triangle #" << ++tri_count << '\n';
            //     // Cast shape back to a triangle.
            //     Triangle * obj_ = dynamic_cast< Triangle* >( tri.get() );
            //     geo_pri->set_shape(obj_);
            // }
        }

        if (type_integrator == "flat")
        {
            std::string color = std::get<0>(p).find("color");
            // Quebrar os espaços do centro e gerar o vetor centro
            std::vector<std::string> result;
            std::istringstream iss(color);
            for (std::string s; iss >> s;)
                result.push_back(s);
            Vec center_(std::stof(result[0]), std::stof(result[1]), std::stof(result[2]));
            //center_.print();
            center_.v4 = 0;
            FlatMaterial *fl_ma = new FlatMaterial(center_);
            std::shared_ptr<Material> material = std::make_shared<FlatMaterial>(*fl_ma);
            geo_pri->set_material(material);
        }

        if (type_integrator == "blinn")
        {

            std::string diffuse = std::get<0>(p).find("diffuse");
            std::string specular = std::get<0>(p).find("specular");
            std::string ambient = std::get<0>(p).find("ambient");
            std::string name = std::get<0>(p).find("name");
            std::string mirror = std::get<0>(p).find("mirror");
            std::string glossiness = std::get<0>(p).find("glossiness");

            // Quebrar os espaços do centro e gerar o vetor centro
            Vec kd(diffuse);
            Vec ks(specular);
            Vec ka(ambient);
            Vec m(mirror);

            //m.print();
            BlinnMaterial *fl_ma = new BlinnMaterial(kd, ks, ka, name, m, std::stof(glossiness));
            std::shared_ptr<Material> material = std::make_shared<BlinnMaterial>(*fl_ma);
            geo_pri->set_material(material);
        }

        if (mesh.size() > 0)
        {
            size_t tri_count{0};
            for (const auto &tri : mesh)
            {
                std::cout << "Triangle #" << ++tri_count << '\n';
                // Cast shape back to a triangle.
                //Shape *obj_ = dynamic_cast< Triangle* >( tri.get() );
                //std::cout << *obj_ << std::endl;
                geo_pri->set_shape(tri);
                //geo_pri->get_material()->kd().print();
                auto t_ = std::make_shared<GeometricPrimitive>(*geo_pri);
                obj_list_.push_back(t_);
                //obj_list_[0]->printCenter();
            }
        }
        else
        {
            obj_list_.push_back(geo_pri);
        }
    }

    scene->setObjList(obj_list_);
}

void Api::INTEGRATOR(Paramset<std::string, std::string> ps)
{
    std::string type = ps.find("type");
    if (type == "flat")
    {
        integrator = new FlatIntegrator(type);
    }
    else if (type == "normal_flat")
    {
        integrator = new NormalIntegrator(type);
    }
    else if (type == "blinn_phong")
    {
        int depth = std::stoi(ps.find("depth"));
        integrator = new BlinnPhongIntegrator(type, depth);
    }
}

void Api::LIGHTS(std::vector<Paramset<std::string, std::string>> ps)
{
    for (auto lig : ps)
    {
        std::string type = lig.find("type");
        if (type == "ambient" || type == "directional")
        {
            Vec l(lig.find("L"));
            if (type == "ambient")
            {
                //std::shared_ptr<Light> al = std::make_shared<AmbientLight>(type, l);
                //light_list.push_back(al);
                scene->ambient = new AmbientLight(type, l);
            }

            if (type == "directional")
            {
                Vec scale(lig.find("scale"));
                Vec from(lig.find("from"));
                Vec to(lig.find("to"));
                // CREATE DIRECTIONAL LIGHT
                std::shared_ptr<Light> dl = std::make_shared<DirectionalLight>(type, l, scale, from, to);
                light_list.push_back(dl);
            }
        }

        if (type == "point" || type == "spot")
        {
            Vec i(lig.find("I"));
            Vec from(lig.find("from"));
            Vec scale(lig.find("scale"));

            if (type == "point")
            {
                std::shared_ptr<Light> pl = std::make_shared<PointLight>(type, i, scale, from);
                light_list.push_back(pl);
            }

            if (type == "spot")
            {
                Vec to(lig.find("to"));
                float cutoff = std::stof(lig.find("cutoff"));
                float falloff = std::stof(lig.find("falloff"));
                std::shared_ptr<Light> sl = std::make_shared<SpotLight>(type, i, scale, from, to, cutoff, falloff);
                light_list.push_back(sl);
            }
        }
    }

    scene->setLight(light_list);
}

Camera Api::getCamera()
{
    return *camera;
}

Film Api::getFilm()
{
    return *film;
}

Background Api::getBackground()
{
    return *background;
}

void Api::render()
{

    auto w = film->width();
    auto h = film->height();

    camera->setHeightWidth(h, w);

    // for(int k  = 0; k < scene->obj_list.size(); k++){
    //     auto o = obj_list_[k];
    //     o->get_material()->kd().print();
    //     Triangle *t = dynamic_cast<Triangle *>(o->get_Shape().get());
    //     std::cout << *t << std::endl;
    // }

    Bvh_node b;
    auto bounds = b.accel(obj_list_);
    // for (int i = 0; i < bounds.size(); i++)
    // {
    //     bounds[i]->geo->printCenter();
    // }

    BvhAccel ba;
    auto a = ba.buildTree(bounds, 0, bounds.size());
    // std::vector<std::shared_ptr<LinearBVH>> lista;
    // ba.LinearizeBVH(lista);

    //std::cout << "Tamanho " << lista.size() << std::endl;
    // std::cout << "Printando árvore" << std::endl;
    // a->printBVH();

    //b.buildTree(bounds, 0, bounds.size(), 0.0001, MAXFLOAT);

    scene->bvh_ = a;
    scene->bounds = bounds;

    auto t = clock();
    // scene->bvh_node->printBVH(scene->bvh_node);
    for (int j = h - 1; j >= 0; j--)
    {
        for (int i = 0; i < w; i++)
        {

            Ray ray = camera->generate_ray(i, j);
            Vec color_(0, 0, 0);

            // std::cout << i << " " << j << std::endl;
            // std::cout << ray << std::endl;

            if (background->getMapping() == "screen")
            {
                color_ = background->sample(float(i) / float(w), float(j) / float(h));
            }

            Color24 c = integrator->Li(ray, scene, color_, 0);
            film->add(i, j, c);
        }
    }
    t = clock() - t;
    printf ("It took %f seconds.\n",((float)t)/CLOCKS_PER_SEC);
    film->write_image();
}

#endif