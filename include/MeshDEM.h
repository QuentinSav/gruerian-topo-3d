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

#include "Mesh.h"

using namespace std;


class MeshDEM : public Mesh {
public:
    void load_vertices_from_TIFF(std::string filepath);
    void compute_indexes();
    void print_info();
    GDALDatasetUniquePtr m_ptr_dataset;
};

#endif // DIGITALELEVATIONMODEL_H