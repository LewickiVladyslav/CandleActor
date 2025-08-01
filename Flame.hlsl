HLSL code from Material custom node

//----------Input----------
//Color Top
//Color Bottom
//UV
//NormalTexture
//Time
//DistortStrenght Param

//----------PROPERTIES----------
float CandleSize = 0.9; 

float2 DistortParam1 = float2(0.3, 0.4);     
float2 DistortParam2 = float2(0.2, 0.3);     
float2 DistortSpeed1 = float2(0.2, 0.8);     
float2 DistortSpeed2 = float2(0.4, 1.2);    
//float DistortStrength = 5.0;                 
float2 NoiseScale1 = float2(0.08, 0.12);     
float2 NoiseScale2 = float2(0.15, 0.18);     

float2 scale1 = float2(1.25, 0.45);    
float intensity1 = 8.0;              
float power1 = 6;                    
float2 offset1 = float2(0.5, 0.5);   

float2 scale2 = float2(0.95, 0.5);    
float intensity2 = 8.0;              
float power2 = 12;                     
float2 offset2 = float2(0.5, 0.75);  

float3 baseGrayColor1 = float3(0.7, 0.7, 0.7);  
float grayIntensity1 = 0.8;                     
float colorBlend1 = 0.8;                        

float3 baseGrayColor2 = float3(0.7, 0.7, 0.7);  
float grayIntensity2 = 0.1;                     
float colorBlend2 = 0.1;                        

//----------MAIN----------
float2 TiledUV1 = UV * NoiseScale1;
float2 PannedUV1 = TiledUV1 + DistortSpeed1 * Time;
float3 NormalPan1 = NormalTex.Sample(NormalTexSampler, PannedUV1).rgb;
float2 DistortUV1 = (NormalPan1.rg - 0.5) * DistortParam1;

float2 TiledUV2 = UV * NoiseScale2;
float2 PannedUV2 = TiledUV2 + DistortSpeed2 * Time;
float3 NormalPan2 = NormalTex.Sample(NormalTexSampler, PannedUV2).rgb;
float2 DistortUV2 = (NormalPan2.rg - 0.5) * DistortParam2;

float2 CombinedDistort = (DistortUV1 + DistortUV2 * 0.5) * DistortStrength;

float heightFactor = UV.y;
float smartFade = smoothstep(0.99, 0.0, heightFactor); 

float2 FinalUV = UV + CombinedDistort * smartFade;

float2 adjustedScale1 = scale1 / CandleSize;  
float2 adjustedScale2 = scale2 / CandleSize;

float r1 = length((FinalUV - offset1) * adjustedScale1);
float mask1 = exp(-pow(r1 * intensity1, power1));
float r2 = length((FinalUV - offset2) * adjustedScale2);
float mask2 = exp(-pow(r2 * intensity2, power2));

float clipMask = step(0.0001, mask1); 

float3 grayBase1 = baseGrayColor1 * mask1 * grayIntensity1;
float3 coloredLayer1 = ColorTop * mask1 * colorBlend1;
float3 grayBase2 = baseGrayColor2 * mask2 * clipMask * grayIntensity2;
float3 coloredLayer2 = ColorBottom * mask2 * clipMask * colorBlend2;
float3 Result1 = grayBase1 + coloredLayer1;
float3 Result2 = grayBase2 + coloredLayer2;

float blendFactor = mask2 * clipMask;
float3 Result = lerp(Result1, Result2, blendFactor);

//----------OUTPUTS----------
return Result;
