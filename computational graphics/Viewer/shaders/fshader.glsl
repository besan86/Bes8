#version 330 core


// the "in" for the fragment shader are the output from vertix shader.
in vec3 fragColor;
in vec3 fragPos;
in vec3 normalPos;
in vec2 fragTexCoords;



// all data needed to calculate the shading passed as uniform from the renderer.
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularStr;
uniform float ambientStr;
// material data.
uniform vec3 MatAmbientColor;
uniform vec3 MatdiffuseColor;
uniform vec3 MatspecularColor;
uniform float Matshinness;

uniform bool activelight;
uniform bool toon;
uniform bool activeTextures;

uniform sampler2D textureMap;

out vec4 fColor;

uniform sampler2D texture;

void main() 
{ 
	vec3 textureColor = vec3(texture(textureMap, fragTexCoords));

	//Ambient light
	vec3 ambientlight = ambientColor  * ambientStr * MatAmbientColor;
	
	//Diffuse light
	vec3 posToLightDirVec = normalize(lightPos - fragPos);
	float diffuse = clamp(dot(posToLightDirVec,normalPos),0,1);
	vec3 diffuseFinal = diffuseColor * diffuse * MatdiffuseColor ;
	
	//Specular light
	vec3 reflectDirVec = normalize(reflect(posToLightDirVec,normalize(normalPos)));
	vec3 posToViewDir = normalize( fragPos - normalize(cameraPos));
	float specularConstant = pow(max(dot(posToViewDir, reflectDirVec),0),10);
	vec3 specularFinal = specularStr * specularColor * specularConstant * MatspecularColor ;

	vec3 finalLight = ambientlight+diffuseFinal+specularFinal;
	finalLight.x = clamp(finalLight.x,0,1);
	finalLight.y = clamp(finalLight.y,0,1);
	finalLight.z = clamp(finalLight.z,0,1);
	if(activelight){
		if(toon){
			float intensity;
			vec4 tcolor;
			intensity = dot(posToLightDirVec,normalize(normalPos));

			if (intensity > 0.92)
				tcolor = vec4(fragColor,1) * vec4(0.5,0.5,0.5,1.0);
			else if (intensity > 0.52)
				tcolor = vec4(fragColor,1) * vec4(0.3,0.3,0.3,1.0);
			else if (intensity > 0.25)
				tcolor =  vec4(fragColor,1) * vec4(0.2,0.2,0.2,1.0);
			else
				tcolor = vec4(fragColor,1) * vec4(0.1,0.1,0.1,1.0);
			fColor = tcolor;
		}else{
			if(activeTextures)
				fColor = vec4(textureColor, 1.0f) * vec4(finalLight,1.0f) ;
			else
				fColor = vec4(fragColor, 1.0f) * vec4(finalLight,1.0f)  ;	
		}
	}else{
		if(activeTextures)
			fColor = vec4(textureColor, 1.0f);
		else
			fColor = vec4(fragColor, 1.0f);	
	}
} 

