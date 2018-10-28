uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;
uniform vec3 emissiveColor;
uniform vec3 transparentColor;

uniform float shininess;
uniform float opacity;
uniform float indexOfRefraction;
uniform float specularScale;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D ambientTexture;
uniform sampler2D shininessTexture;
uniform sampler2D emissiveTexture;
uniform sampler2D opacityTexture;
uniform sampler2D normalTexture;
uniform sampler2D heightMapTexture;
uniform sampler2D displacementTexture;
uniform sampler2D lightMapTexture;
uniform sampler2D reflectionTexture;
uniform sampler2D otherTexture;