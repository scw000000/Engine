#version 430

struct TileFrustum
    {
    vec4 m_Planes[ 4 ];
    };    
    
uniform uvec2 uTileSize;
uniform uvec2 uScreenSize;
uniform  vec2 uHalfSizeNearPlane;
 
uniform mat4 uProj;

// frustum facing inward
layout ( std430, binding = 0 ) writeonly buffer tileFrustumSSBO
    {
    TileFrustum data[];
    }TileFrustumSSBO;

 layout ( local_size_x = 1, local_size_y = 1, local_size_z = 1 ) in;
 
 vec4 ComputePlane( vec3 p0, vec3 p1, vec3 p2 )
    {
    vec4 ret = vec4( normalize( cross( p1 - p0, p2 - p0 ) ), 0.0 );
    // dot( p0, normal ) + d = 0 -> d = -dot( p0, normal ) 
    ret.w = -dot( p0, ret.xyz );
    return ret;
    }
 // transform from [ 0, 1 ] to view space
 // Reference: https://www.khronos.org/opengl/wiki/Compute_eye_space_from_window_space
 // http://www.derschmale.com/2014/01/26/reconstructing-positions-from-the-depth-buffer/
 // http://www.derschmale.com/2014/09/28/unprojections-explained/ (invprojection)
 vec3 TexCoordinatesToView( vec2 p )
    {
    vec3 eyeDir = vec3( 2 * uHalfSizeNearPlane * p - uHalfSizeNearPlane, -1 );
    // ndcZ = -1.0, uProj[2][3] = -1.0, and we need positive value of depth value
    float eyeDirScale = uProj[3][2] / ( -1.0 + uProj[2][2] ); 
    return eyeDir * eyeDirScale;
    }
 
 void main()
    {
    uvec2 tilePos = gl_WorkGroupID.xy;
    // uvec2 tileCount = gl_NumWorkGroups.xy; 
    
    // shift on screen pixels
    vec2 frustumShift[ 4 ];
    frustumShift[ 0 ] = vec2( 0.0, 0.0 ); // bottom left
    frustumShift[ 1 ] = vec2( uTileSize.x - 1, 0.0 ); // bottom right
    frustumShift[ 2 ] = vec2( uTileSize.x - 1, uTileSize.y - 1 ); // top right
    frustumShift[ 3 ] = vec2( 0.0, uTileSize.y - 1 ); // top left
    
    vec2 frustumPos[ 4 ]; // screen space position
    for( int i = 0; i < 4; ++i )
        {
        frustumPos[ i ] = ( tilePos * uTileSize ) + frustumShift[ i ];
        frustumPos[ i ] = vec2( min( frustumPos[ i ].x, uScreenSize.x ), min( frustumPos[ i ].y, uScreenSize.y ) );
        frustumPos[ i ] /= uScreenSize;
        }

    vec3 frustumPosVS[ 4 ];
    for( int i = 0; i < 4; ++i )
        {
        frustumPosVS[ i ] = TexCoordinatesToView( frustumPos[ i ] );
        }
        
    // Generate 4 frustum planes, pointing inward
    vec3 cameraPosVS = vec3( 0.0, 0.0, 0.0 );
    //uint tileIndex = gl_WorkGroupID.x + gl_WorkGroupID.y * gl_NumWorkGroups.x;
    TileFrustumSSBO.data[ gl_NumWorkGroups.x * gl_WorkGroupID.y + gl_WorkGroupID.x ].m_Planes[ 0 ] = ComputePlane( cameraPosVS, frustumPosVS[ 0 ], frustumPosVS[ 3 ] ); // left plane
    TileFrustumSSBO.data[ gl_NumWorkGroups.x * gl_WorkGroupID.y + gl_WorkGroupID.x ].m_Planes[ 1 ] = ComputePlane( cameraPosVS, frustumPosVS[ 1 ], frustumPosVS[ 0 ] ); // bottom plane
    TileFrustumSSBO.data[ gl_NumWorkGroups.x * gl_WorkGroupID.y + gl_WorkGroupID.x ].m_Planes[ 2 ] = ComputePlane( cameraPosVS, frustumPosVS[ 2 ], frustumPosVS[ 1 ] ); // right plane
    TileFrustumSSBO.data[ gl_NumWorkGroups.x * gl_WorkGroupID.y + gl_WorkGroupID.x ].m_Planes[ 3 ] = ComputePlane( cameraPosVS, frustumPosVS[ 3 ], frustumPosVS[ 2 ] ); // top plane
    
  //  if( uScreenSize.x == 1095 )
  //      {
 //           TileFrustumSSBO.data[ gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y ].m_Planes[ 0 ] = vec4( 0.5, 0.6, 0.7, 0.8 ); // left plane
  //  TileFrustumSSBO.data[ gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y ].m_Planes[ 1 ] = vec4( 0.5, 0.6, 0.7, 0.8 ); // left plane
  //  TileFrustumSSBO.data[ gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y ].m_Planes[ 2 ] = vec4( 0.5, 0.6, 0.7, 0.8 ); // left plane
  //  TileFrustumSSBO.data[ gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y ].m_Planes[ 3 ] = vec4( 0.5, 0.6, 0.7, 0.8 ); // left plane
 //       }

    } 
    