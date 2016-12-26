#version 400

in vec3 oEntryPoint_ModelSpace;

uniform sampler2D uExitPoints;
uniform sampler3D uVolumeTexture;
uniform sampler1D uTransferFunction;  
uniform float     uStepSize;
uniform vec2      uScreenSize;

out vec4 oFragColor;

void main()
    {
    vec3 exitPoint = texture( uExitPoints, gl_FragCoord.st / uScreenSize ).xyz;

    if ( oEntryPoint_ModelSpace == exitPoint)
        {
    	//background need no raycasting
    	discard;
        }
        
    vec3 rayDir = exitPoint - oEntryPoint_ModelSpace;
    float rayLen = length( rayDir ); // the length from front to back is calculated and used to terminate the ray
    vec3 deltaRayVec = normalize( rayDir ) * uStepSize;
    float deltaDirLen = length( deltaRayVec );
    vec3 voxelCoord = oEntryPoint_ModelSpace;
    vec4 colorAcum = vec4(0.0, 0.0, 0.0, 0.0); // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending
    
    // for each sampling point in volume
    float intensity;
    float lengthAcum = 0.0;
    vec4 colorMapping; // The src color 
    
    // backgroundColor
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0);
 
    while( true )
        {
    	intensity =  texture( uVolumeTexture, voxelCoord ).x;
        // Get the color mapping in transfer function texture
    	colorMapping = texture( uTransferFunction, intensity );

    	// front-to-back compositing
    	if ( colorMapping.a > 0.0 ) 
            {
            colorAcum.rgb += ( 1.0 - colorAcum.a ) * colorMapping.rgb * colorMapping.a;
    	    colorAcum.a += ( 1.0 - colorAcum.a ) * colorMapping.a;
            }
    	voxelCoord += deltaRayVec;
    	lengthAcum += deltaDirLen;
    	if (lengthAcum >= rayLen )
            {	
    	   // colorAcum.rgb = colorAcum.rgb * colorAcum.a + ( 1 - colorAcum.a ) * bgColor.rgb;		
    	    break;  // terminate if opacity > 1 or the ray is outside the volume	
            }	
    	else if (colorAcum.a > 1.0)
            {
    	    colorAcum.a = 1.0;
    	    break;
            }
        }
    oFragColor = colorAcum;

    // FragColor = vec4( oEntryPoint_ModelSpace, 1.0);
    // oFragColor = vec4(exitPoint, 1.0);
   
    }
