#include "MeshDEM.h"

void MeshDEM::load_vertices_from_TIFF(std::string filepath)
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

void MeshDEM::print_info()
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