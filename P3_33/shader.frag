#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoord; 

uniform vec3 viewPos; 
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture1;

void main()
{
    // Ambiente
    float ambientI = 0.25;
    vec3 ambient = ambientI * lightColor;
  	
    // Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// Especular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;
            
   	vec3 result = (ambient + diffuse + specular) * objectColor; // Luz final
    FragColor = texture(texture1, TexCoord) * vec4(result, 1.0); // Textura
} 
