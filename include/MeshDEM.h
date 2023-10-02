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
#include <CGAL/Simple_cartesian.h>

#include <gdal/gdal_priv.h>
#include <errno.h>

#include "Mesh.h"

//#include <vector>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Delaunay_triangulation_2.h>

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef K::Point_2 Point_2;
//typedef CGAL::Delaunay_triangulation_2<K> Delaunay;


using namespace std;


class MeshDEM : public Mesh {
public:
    void loadVerticesFromTIFF(std::string filepath);
    void decimateGrid();
    void computeIndexes();
    void computeNormals();
    void printInfo();
    GDALDatasetUniquePtr m_ptr_dataset;
};

#endif // DIGITALELEVATIONMODEL_H