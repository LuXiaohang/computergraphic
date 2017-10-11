#version 150 compatibility
uniform vec3 lightposition; 
uniform vec4 eyeposition;
 
out vec3 lightd;
out vec3 halfvec;
out vec3 norm;
out vec3 eyed;
 
in vec3 rm_Tangent;
 
void main(void)
{
   vec4 pos = gl_ModelViewMatrix * gl_Vertex;
   pos = pos / pos.w;
   
   vec4 vlightposition = (gl_ModelViewMatrix * vec4(lightposition, 1.0));
   vec4 veyeposition   = (gl_ModelViewMatrix * eyeposition);
   
   lightd = normalize(vlightposition.xyz - pos.xyz);
   eyed = normalize(veyeposition.xyz - pos.xyz);
   
  //模型空间下的TBN
   norm = normalize(gl_NormalMatrix * gl_Normal);

   vec3 vtangent  = normalize(gl_NormalMatrix * rm_Tangent);
 
   vec3 vbinormal = cross(norm,vtangent);
   
   //将光源向量和视线向量转换到TBN切线空间
   lightd.x = dot(vtangent,lightd);
   lightd.y = dot(vbinormal,lightd); 
   lightd.z = dot(norm, lightd);
   lightd = normalize(lightd);
   
   eyed.x = dot(vtangent,eyed);
   eyed.y = dot(vbinormal,eyed);
   eyed.z = dot(norm, eyed);

   eyed = normalize(eyed);
   
   halfvec = normalize(lightd + eyed);
 
   gl_FrontColor = gl_Color;
   
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   gl_Position = ftransform();
}