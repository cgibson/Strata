attribute vec4 position;
attribute vec4 normal;
attribute vec2 uv;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

varying vec4 normalVarying;
varying vec2 uvVarying;
varying vec4 posVarying;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;
    posVarying = modelViewMatrix * position;
	normalVarying = normalMatrix * normal;
    uvVarying = uv;
}
