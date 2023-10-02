#include "MeshDEM.h"

void MeshDEM::loadVerticesFromTIFF(std::string filepath)
{
    GDALAllRegister();

    m_ptr_dataset = GDALDatasetUniquePtr(GDALDataset::FromHandle(GDALOpen(filepath.c_str(), GA_ReadOnly)));
    if(!m_ptr_dataset)
    {
        std::cout << "GDAL error: Impossible to open dataset" << std::endl;
    }

    GDALRasterBand *ptr_band = m_ptr_dataset->GetRasterBand(1);
    m_size_x = ptr_band->GetXSize();
    m_size_y = ptr_band->GetYSize();

    // Create a vector to store vertices
    m_vertices.reserve(m_size_x * m_size_y);

    // Read elevation data and convert to vertices
    float *ptr_scanline = static_cast<float*>(CPLMalloc(sizeof(float) * m_size_x));
    
    float scale_factor = 1.0f/15000.0f;
    float scale_factor_plane = 15900.0f/128.0f * scale_factor;
    float scale_factor_height = 638.0f/256.0f * scale_factor;

    Vertex vertex;
    for (int j = 0; j < m_size_y; ++j) {
        CPLErr error = ptr_band->RasterIO(GF_Read, 0, j, m_size_y, 1, ptr_scanline, m_size_x, 1, GDT_Float32, 0, 0);
        
        for (int i = 0; i < m_size_x; ++i) {
            vertex.x = static_cast<float>(i)*scale_factor_plane; 
            vertex.y = static_cast<float>(ptr_scanline[i])*scale_factor_height;
            vertex.z = static_cast<float>(-j)*scale_factor_plane;
            m_vertices.push_back(vertex);
       }
    }

    CPLFree(ptr_scanline);

    this->computeIndexes();
    this->computeNormals();
}

void MeshDEM::computeIndexes()
{
    auto t0 = glfwGetTime();
    // Create a vector to store vertices
    m_faces.reserve((m_size_x - 1) * (m_size_y - 1) * 2);

    for (unsigned int j = 0; j < (m_size_y - 1); ++j) {
        for (unsigned int i = 0; i < (m_size_x - 1); ++i) {   

            unsigned int k = i + m_size_x * j;

            Face face;
            face.p1 = k;
            face.p2 = k+1;
            face.p3 = k+m_size_x;
            m_faces.push_back(face);

            face.p1 = k+1;
            face.p2 = k+m_size_x+1;
            face.p3 = k+m_size_x;
            m_faces.push_back(face);

        }
    }
    auto t1 = glfwGetTime();
}

void MeshDEM::computeNormals()
{
    // Array to store the faces normal
    vector<glm::vec3> faces_normal;
    faces_normal.reserve(m_faces.size());
    
    // Calculate face normals
    for (int i = 0; i < m_faces.size(); ++i) {
        Face face = m_faces[i];
        glm::vec3 position_p1 = glm::vec3(m_vertices[face.p1].x, m_vertices[face.p1].y, m_vertices[face.p1].z);
        glm::vec3 position_p2 = glm::vec3(m_vertices[face.p2].x, m_vertices[face.p2].y, m_vertices[face.p2].z);
        glm::vec3 position_p3 = glm::vec3(m_vertices[face.p3].x, m_vertices[face.p3].y, m_vertices[face.p3].z);

        glm::vec3 edge1 = position_p2 - position_p1;
        glm::vec3 edge2 = position_p3 - position_p1;
        glm::vec3 normal = normalize(cross(edge1, edge2));
        faces_normal.push_back(normal);
    }
    
    // Calculate vertex normals
    for (int i = 0; i < m_vertices.size(); ++i) {
        
        if (i%1000 == 0) std::cout << "Iteration" << i << "/" << m_vertices.size() << std::endl;
        
        // Find all the faces 
        std::vector<int> matching_indices;

        for (int j = 0; j < m_faces.size(); ++j) {
            if (m_faces[j].p1 == i || m_faces[j].p2 == i || m_faces[j].p3 == i) {
                matching_indices.push_back(j);
            }
        }

        glm::vec3 normalSum(0.0f);
        
        for (auto index : matching_indices) {
            normalSum += faces_normal[index];
        }
        m_vertices[i].normal_x = normalSum.x;
        m_vertices[i].normal_y = normalSum.y;
        m_vertices[i].normal_z = normalSum.z;
    }

    // Normalize vertex normals
    for (Vertex vertex: m_vertices) {
        glm::vec3 normal(vertex.normal_x, vertex.normal_y, vertex.normal_z);
        normal = normalize(normal);
        vertex.normal_x = normal.x;
        vertex.normal_y = normal.y;
        vertex.normal_z = normal.z;
    }
    

}

void MeshDEM::printInfo()
{
    double adfGeoTransform[6];
    printf("Driver: %s/%s\n",
            m_ptr_dataset->GetDriver()->GetDescription(),
            m_ptr_dataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
    printf("Size is %dx%dx%d\n",
            m_ptr_dataset->GetRasterXSize(), m_ptr_dataset->GetRasterYSize(),
            m_ptr_dataset->GetRasterCount());
    if( m_ptr_dataset->GetProjectionRef() != NULL)
        printf( "Projection is `%s'\n", m_ptr_dataset->GetProjectionRef());
    if( m_ptr_dataset->GetGeoTransform(adfGeoTransform) == CE_None)
    {
        printf("Origin = (%.6f,%.6f)\n",
                adfGeoTransform[0], adfGeoTransform[3]);
        printf("Pixel Size = (%.6f,%.6f)\n",
                adfGeoTransform[1], adfGeoTransform[5]);
    }
}