varying vec3 position;
varying vec4 vertex;
varying vec3 normal;
varying vec3 lightVec; 
varying vec3 eyeVec;

void main()
{
    position = gl_Vertex.xyz;

    vertex = gl_ModelViewMatrix * gl_Vertex;
    normal = gl_NormalMatrix    * gl_Normal;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_TextureMatrix[1] * vertex;
    gl_TexCoord[2] = gl_MultiTexCoord2;
    gl_TexCoord[3] = gl_MultiTexCoord0 * 25.0;

    gl_Position = ftransform();
	gl_FogFragCoord = max(gl_Position.z - 100.0, 0.0);
	
	// Calculate tangent
	vec3 tangent; 
	vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0)); 	
	if(length(c1)>length(c2))
	{
		tangent = normalize(gl_NormalMatrix * c1);	
	}
	else
	{
		tangent = normalize(gl_NormalMatrix * c2);	
	}	

	// Calculate bumpmap params
	vec3 binormal = cross(normal, tangent);
	
	vec3 tmpVec = gl_LightSource[0].position.xyz - vertex.xyz;

	lightVec.x = dot(tmpVec, tangent);
	lightVec.y = dot(tmpVec, binormal);
	lightVec.z = dot(tmpVec, normal);

	tmpVec = -vertex;
	eyeVec.x = dot(tmpVec, tangent);
	eyeVec.y = dot(tmpVec, binormal);
	eyeVec.z = dot(tmpVec, normal);
}
