#ifndef _FILM_
#define _FILM_

class Film
{
private:
    /* data */
    std::string filename;
    std::string img_type;
    std::string type;
    int x_res;
    int y_res;

public:
    Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res);
    ~Film();
};

Film::Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res)
{
    this->filename = filename;
    this->img_type = img_type;
    this->type = type;
    this->x_res = x_res;
    this->y_res = y_res;
}

Film::~Film() = default;

#endif
