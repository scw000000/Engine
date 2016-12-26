#version 430

struct TileFrustum
    {
    vec4 m_Planes[ 4 ];
    };    
    
uniform uvec2 uTileSize;
uniform vec2 uScreenSize;

uniform mat4 uInvProj;

layout ( std430, binding = 0 ) writeonly buffer tileFrustumSSBO
    {
    TileFrustum data[][];
    }TileFrustumSSBO;

 layout ( local_size_x = 1, local_size_y = 1, local_size_z = 1 ) in;
 
 vec4 ComputePlane( vec3 p0, vec3 p1, vec3 p2 )
    {
    vec4 ret = vec4( normalize( cross( p1 - p0, p2 - p0 ) ), 0.0 );
    ret.w = -dot( p0, ret.xyz );
    return ret;
    }
    
 vec3 ScreenToView( vec2 p )
    {
    p = p * vec2( 2.0, 2.0 ) - vec2( 1.0, 1.0 ); // to NDC space [ -1, +1 ]
    vec4 ret = uInvProj * vec4( p.xy, -1.0, 1.0 );
    ret = ret / ret.w;
    return ret.xyz;
    }
 
 void main()
    {
    uvec2 tilePos = gl_WorkGroupID.xy;
    
    vec2 frustumShift[ 4 ];
    frustumShift[ 0 ] = vec2( 0.0, 0.0 ); // bottom left
    frustumShift[ 1 ] = vec2( uTileSize.x - 1, 0.0 ); // bottom right
    frustumShift[ 2 ] = vec2( uTileSize.x - 1, uTileSize.y - 1 ); // top right
    frustumShift[ 3 ] = vec2( 0.0, uTileSize.y - 1 ); // top left
    
    vec2 frustumPosSS[ 4 ]; // screen space position
    for( int i = 0; i < 4; ++i )
        {
        frustumPosSS[ i ] = ( tilePos * uTileSize ) + frustumShift[ i ];
        frustumPosSS[ i ] = vec2( min( frustumPosSS[ i ].x, uScreenSize.x ), min( frustumPosSS[ i ].y, uScreenSize.y ) );
        frustumPosSS[ i ] /= uScreenSize;
        }

    vec3 frustumPosVS[ 4 ];
    for( int i = 0; i < 4; ++i )
        {
        frustumPosVS[ i ] = ScreenToView( frustumPosSS[ i ] );
        }
        
    // Generate 4 frustum planes, pointing inward
    vec3 cameraPosVS = vec3( 0.0, 0.0, 0.0 );
    //uint tileIndex = gl_WorkGroupID.x + gl_WorkGroupID.y * gl_NumWorkGroups.x;
    TileFrustumSSBO.data[ gl_WorkGroupID.x ][ gl_WorkGroupID.y ].m_Planes[ 0 ] = ComputePlane( cameraPosVS, frustumPosVS[ 0 ], frustumPosVS[ 3 ] ); // left plane
    TileFrustumSSBO.data[ gl_WorkGroupID.x ][ gl_WorkGroupID.y ].m_Planes[ 1 ] = ComputePlane( cameraPosVS, frustumPosVS[ 1 ], frustumPosVS[ 0 ] ); // bottom plane
    TileFrustumSSBO.data[ gl_WorkGroupID.x ][ gl_WorkGroupID.y ].m_Planes[ 2 ] = ComputePlane( cameraPosVS, frustumPosVS[ 2 ], frustumPosVS[ 1 ] ); // right plane
    TileFrustumSSBO.data[ gl_WorkGroupID.x ][ gl_WorkGroupID.y ].m_Planes[ 3 ] = ComputePlane( cameraPosVS, frustumPosVS[ 3 ], frustumPosVS[ 2 ] ); // top plane
    } 
    