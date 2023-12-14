// begin include guard
#ifndef TEXTUREREGIONS_MAINTEXTURE
#define TEXTUREREGIONS_MAINTEXTURE

// define names for single regions
#define RegionCardBack  0
#define RegionHighlight  1

// define names for first region in region matrices
#define FirstRegionCards  2

// your initialization code must call this function
void DefineRegions_MainTexture()
{
    select_texture( 0 );

    // define single regions
    select_region( RegionCardBack );
    define_region( 0,39,  37,76,  0,39 );
    select_region( RegionHighlight );
    define_region( 39,39,  79,79,  39,39 );

    // define region matrices
    define_region_matrix( FirstRegionCards, 0,0,  37,37,  0,0,  12,1,  1 );
}

// end include guard
#endif
