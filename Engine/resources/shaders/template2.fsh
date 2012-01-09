#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D baseTexture;

varying vec4 normalVarying;
varying vec2 uvVarying;
varying vec4 posVarying;

void main()
{
    vec3 N = normalize(normalVarying.xyz);
    vec4 diffuse = texture2D(baseTexture, uvVarying);
    
    vec3 keyLightDir = normalize(vec3(5, 5, 5));
    vec4 keyLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    vec3 rimLightDir = normalizevec3(-5,-5,-4));
    vec4 rimLightColor = vec4(0.2, 1.0, 1.0, 1.0);
    float rimLightPower = 0.4;
    
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 R = -reflect(keyLightDir, N);
    vec3 V = normalize(-posVarying.xyz);
    vec4 specular = specularColor * max(0.0, dot(R, V)) * keyLightColor;
    
    
    float keyNdL = max(0.0, dot(N, keyLightDir));
    keyNdL = keyNdL * keyNdL;
    float rimNdL = max(0.0, dot(N, (rimLightDir));
    
    vec4 combine = (keyLightColor * keyNdL) + (rimLightColor * rimNdL * rimLightPower);
    
	gl_FragColor = diffuse * max(ambient, combine);
}