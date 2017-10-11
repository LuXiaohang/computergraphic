#version 150 compatibility
uniform float shiness;
uniform vec4 ambient, diffuse, specular;
 
uniform sampler2D bumptex;
uniform sampler2D basetex;
 
float amb = 0.2;
float diff = 0.3;
float spec = 0.2;
 
in vec3 lightd;
in vec3 halfvec;
in vec3 norm;
in vec3 eyed;
 
void main(void)
{
   vec3 vlightd = normalize(lightd);
   vec3 veyed = normalize(eyed);
 
   vec3 vnorm =   normalize(norm);
   vec3 vhalfvec =  normalize(halfvec);  
   
   vec4 baseCol = texture2D(basetex, gl_TexCoord[0].xy); 

   vec3 tbnnorm = texture2D(bumptex, gl_TexCoord[0].xy).xyz;
   
   tbnnorm = normalize((tbnnorm  - vec3(0.5))); 
   
   float diffusefract =  max( dot(lightd,tbnnorm) , 0.0); 
   float specularfract = max( dot(vhalfvec,tbnnorm) , 0.0);
   
   if(specularfract > 0.0)
   {
      specularfract = pow(specularfract, shiness);
   }
   gl_FragColor = vec4(amb * ambient.xyz * baseCol.xyz + diff * diffuse.xyz * diffusefract * baseCol.xyz + spec * specular.xyz * specularfract ,1);
}