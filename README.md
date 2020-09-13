### COMPUTER GRAPHICS - Ray Tracer Project

##### [Abraão Vitor Lopes Dantas and Henrique David de Medeiros](#author)

# Project 1 - Basic Infrastructure

This is a project to Computer Graphics I class (DIM0451-UFRN).

In this project, it was necessary to create the basic infrastructure to make the ray tracing. The first project was divided in three subprojects, create a Paramset class, that is responsible to take info from XML file to the project; to add a structure to Camera, Film and Background; and generate two outputs from two inputs (single color background and a gradient from 4 colors background).

The ParamSet is a class that it's gonna help us to take the inputs of an external file, responsible to create our configuration and our world, that is pass in XML format. The class saves in a dictionary the attributes (keys and values) and then we begin to configure everything that we need.  

After create the ParamSet class, we focus on development the world configurations, in the case: Camera, Film and Background's classes. They are responsible to manager the world that is gonna be created.

The next step of the project, we had to generate an image with just a single background color (light blue sky, in ./scene/basic_scene.xml). And in the last step, we had to generate another image with 4 input colors, then we make an interpolation and we generate a beautiful gradient of those input colors, being each color in each corner of the rectangle.

All images generated has type ppm, for more performance.

## Compile

`cmake CMakeLists.txt`

`make`

## Execute

`./main <input_file_name>.xml`

## Results

The image with just a single color can be found in `simple_bkg.ppm`; and the image with the four colors gradient in `interpolated_bkg.ppm`.


## Author
Abraão Vitor Lopes Dantas
IT and Computer Science Student. IT Technician(since 2016)
Email: _abraaovld@gmail.com_

Henrique David de Medeiros  
IT and Computer Science Student. IT Technician (since 2016)  
Email: _henriquemed101@gmail.com_
