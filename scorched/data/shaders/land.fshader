uniform sampler2DShadow shadow;
uniform sampler2D mainmap;
uniform sampler2D detailmap;

varying vec3 position;
varying vec4 vertex;
varying vec3 normal;

void main()
{
    // Look up the diffuse color and shadow states for each light source.
    float s0 = shadow2DProj(shadow, gl_TexCoord[1]).r;
    
    // Compute the lighting vectors.
    vec3 N  = normalize(normal);
    vec3 L0 = normalize(gl_LightSource[0].position.xyz);

    // Compute the illumination coefficient for each light source.
    vec3 d0 = vec3(max(dot(N, L0), 0.0)) * s0;

	// add exp fog (depth cueing)
	float fog = exp2(-gl_Fog.density * gl_FogFragCoord * 7.0 * 1.442695);
	float fog_factor = clamp(fog, 0.8, 1.0);

    // Compute the final pixel color from the diffuse and ambient lighting.
	vec4 groundColor = texture2D(mainmap, gl_TexCoord[0]);
	vec4 detailColor = texture2D(detailmap, gl_TexCoord[2]);
	
	vec3 finalColor = 
		((groundColor.rgb * 3.5) + detailColor.rgb) / 4.0 * 
		(gl_LightSource[0].diffuse.rgb * d0 + 
		gl_LightSource[0].ambient.rgb);
		
	gl_FragColor = vec4(mix(vec3(0.1, 0.1, 0.1), finalColor, fog_factor), 1.0);
}
