#ifndef DIGITALELEVATIONMODEL_H
#define DIGITALELEVATIONMODEL_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gdal/gdal_priv.h>
#include <errno.h>

using namespace std;


class DigitalElevationModel {
public:
    DigitalElevationModel();
    ~DigitalElevationModel();

    struct Vertex 
    {
        float x, y, z;
    };

    struct TriangleIndexes 
    {
        unsigned int p1, p2, p3;
    };

    void load_vertices_from_TIFF(std::string filepath);
    void load_predefined_vertices();
    void compute_indexes();
    void print_info();
    void bind();
    void draw();
    void bind_vertex_array();
 
    std::vector<Vertex> m_vertices;
    std::vector<TriangleIndexes> m_indexes;
    GDALDatasetUniquePtr m_ptr_dataset;
    unsigned int m_size_x, m_size_y;
    unsigned int m_VBO, m_VAO, m_EBO;
    
};

#endif // DIGITALELEVATIONMODEL_H