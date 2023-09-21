#include "DEM.h"

DEM::DEM()
{

}

DEM::~DEM()
{
    // Need to free memory of ptr_dataset
    //GDALClose(static_cast<GDALDatasetH>(m_ptr_dataset));

    m_vertices.clear();

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void DEM::load_vertices_from_TIFF(std::string filepath)
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
    float max_elevation = 1996.19;
    float min_elevation = 1486.64;
    float scale_factor = 50;
    Vertex vertex;
    for (int j = 0; j < m_size_y; ++j) {
        CPLErr error = ptr_band->RasterIO(GF_Read, 0, j, m_size_y, 1, ptr_scanline, m_size_x, 1, GDT_Float32, 0, 0);
        
        
        for (int i = 0; i < m_size_x; ++i) {
            
            vertex.x = static_cast<float>(i)/scale_factor; // X-coordinate
            vertex.y = (ptr_scanline[i] - min_elevation)/scale_factor;
            vertex.z = static_cast<float>(j)/scale_factor;
            m_vertices.push_back(vertex);
        }
    }

    

    CPLFree(ptr_scanline);
     
}

void DEM::load_predefined_vertices()
{
    /*m_vertices = {
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},

    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},

    {-0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

    { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

    {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f},

    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f}
    }; */

    m_vertices = {
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},

    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},

    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f}
    };

    m_indexes = {
        {0, 1, 2},
        {3, 4, 5},
        
        {6, 7, 8},
        {9, 10, 11},
        
        {12, 13, 14},
        {15, 16, 17},

        {18, 19, 20},
        {21, 22, 23},

        {24, 25, 26},
        {27, 28, 29},

        {30, 31, 32},
        {33, 34, 35}
    };
}

void DEM::bind()
{
    
    // Generate a buffer with unique ID VBO
    glGenVertexArrays(1, &m_VAO); 
    glGenBuffers(1, &m_VBO);  
    glGenBuffers(1, &m_EBO); 
    
    //Bind VAO first
    glBindVertexArray(m_VAO);
    //Bind the created buffer to the GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    // Copy the vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);  
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * sizeof(TriangleIndexes), m_indexes.data(), GL_STATIC_DRAW);
    
    // Positions attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors attributes
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); 
}

void DEM::bind_vertex_array()
{
    glBindVertexArray(m_VAO);
}

void DEM::compute_indexes()
{
    // Create a vector to store vertices
    m_indexes.reserve((m_size_x - 1) * (m_size_y - 1) * 2);

    for (unsigned int j = 0; j < m_size_y; ++j) {
        for (unsigned int i = 0; i < (m_size_x - 1); ++i) {   
        
            unsigned int k = i + m_size_x * j;

            TriangleIndexes indexes;
            indexes.p1 = k;
            indexes.p2 = k+1;
            indexes.p3 = k+m_size_x;
            m_indexes.push_back(indexes);

            indexes.p1 = k+1;
            indexes.p2 = k+m_size_x+1;
            indexes.p3 = k+m_size_x;
            m_indexes.push_back(indexes);
        }
    }

    std::cout << m_indexes.size() << std::endl;

}

void DEM::draw()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
}

void DEM::print_info()
{
    double adfGeoTransform[6];
    printf( "Driver: %s/%s\n",
            m_ptr_dataset->GetDriver()->GetDescription(),
            m_ptr_dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
    printf( "Size is %dx%dx%d\n",
            m_ptr_dataset->GetRasterXSize(), m_ptr_dataset->GetRasterYSize(),
            m_ptr_dataset->GetRasterCount() );
    if( m_ptr_dataset->GetProjectionRef()  != NULL )
        printf( "Projection is `%s'\n", m_ptr_dataset->GetProjectionRef() );
    if( m_ptr_dataset->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        printf( "Origin = (%.6f,%.6f)\n",
                adfGeoTransform[0], adfGeoTransform[3] );
        printf( "Pixel Size = (%.6f,%.6f)\n",
                adfGeoTransform[1], adfGeoTransform[5] );
    }
}