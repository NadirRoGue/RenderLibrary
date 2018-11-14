
mat3
computeTangentSpace(in vec3 inNormal, in vec3 inTangent)
{
  vec3 normalizedTangent = normalize(inTangent);
  vec3 normalizedNormal  = normalize(inNormal);
  normalizedTangent      = normalize(
      normalizedTangent
      - (dot(normalizedTangent, normalizedNormal) * normalizedNormal));
  vec3 normalizedBitangent
      = normalize(cross(normalizedNormal, normalizedTangent));
  return mat3(normalizedTangent, normalizedBitangent, normalizedNormal);
}

vec3
computeBumpMapping(in vec4 inBumpVector, in mat3 tangentSpace)
{
  vec3 bumpMapNormal = normalize((inBumpVector.rgb * 2.0) - 1.0);
  return normalize(tangentSpace * bumpMapNormal);
}

vec2
parallaxUVMapping(in mat3 TBN, in vec3 viewV, in vec3 posV,
                  in sampler2D heightMap, in vec2 UV)
{
  // La inversa para trabajar con el plano XY del vector de vista
  mat3 tTBN    = transpose(TBN);
  vec3 viewTBN = tTBN * viewV;
  vec3 posTBN  = tTBN * posV;
  vec3 V       = normalize(viewTBN - posTBN);

  const float minLayers = 8.0;
  const float maxLayers = 32.0;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), V)));
  // number of depth layers
  //const float numLayers = 10.0;
  // calculate the size of each layer
  float layerDepth = 1.0 / numLayers;
  // depth of current layer
  float currentLayerDepth = 0.0;
  // the amount to shift the texture coordinates per layer (from vector P)
  vec2 P              = V.xy * 0.1; // 0.1 == height scale
  vec2 deltaTexCoords = P / numLayers;

  // get initial values
  vec2 currentTexCoords      = UV;
  float currentDepthMapValue = texture(heightMap, currentTexCoords).r;

  while (currentLayerDepth < currentDepthMapValue)
  {
    // shift texture coordinates along direction of P
    currentTexCoords -= deltaTexCoords;
    // get depthmap value at current texture coordinates
    currentDepthMapValue = texture(heightMap, currentTexCoords).r;
    // get depth of next layer
    currentLayerDepth += layerDepth;
  }

  // get texture coordinates before collision (reverse operations)
  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

  // get depth after and before collision for linear interpolation
  float afterDepth = currentDepthMapValue - currentLayerDepth;
  float beforeDepth
      = texture(heightMap, prevTexCoords).r - currentLayerDepth + layerDepth;

  // interpolation of texture coordinates
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords
      = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}