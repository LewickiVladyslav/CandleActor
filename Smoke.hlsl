//----------INPUTS---------- 
//UV 
//Time 
//Texture
//Particle Color
//Dynamic Param
//Noise Texture
//----------PROPERTIES---------- 


//----------MAIN---------- 
float4 MainTex = Tex.Sample(TexSampler, UV).rgba; 
float FinalTex = pow(MainTex.r, 2.6) * 2; 

float Mask = ((1.0 - UV.y) * UV.y) * 3; 
float MaskFinal = saturate(pow(Mask, 2.0)); 
float Result = MaskFinal * FinalTex; 
float3 FinalColor = Result * ParticleColor.rgb; 

float3 NoiseMask = (NoiseTex - 0.5) + (DynamicPar * 4); 
float DissolveMask = saturate(NoiseMask); 

//----------OUTPUTS---------- 
Opacity = (1 - DissolveMask) * (Result * ParticleColor.a); 

return FinalColor;
