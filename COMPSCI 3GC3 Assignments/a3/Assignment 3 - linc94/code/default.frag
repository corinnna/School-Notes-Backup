#version 330 core

// set uniform values
uniform sampler2D myTexture;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform float kc;
uniform float kl;
uniform float kq;
uniform float cutOff;

// create Spotlight class to access each light's direction and diffuseColor
// direction and diffuseColor are the only values that differ between lights

struct Spotlight {
    vec3 direction;
    vec3 diffuseColor;
};

// create each spotlight
uniform Spotlight redLight;
uniform Spotlight greenLight;
uniform Spotlight blueLight;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
    // calculate lightDir
    vec3 lightDir = normalize(lightPos - FragPos);

    // calculate each theta per light
    float redTheta = dot(lightDir, normalize(-redLight.direction));
    float greenTheta = dot(lightDir, normalize(-greenLight.direction));
    float blueTheta = dot(lightDir, normalize(-blueLight.direction));

    // initialize vectors to hold each light's color
    vec4 redFragColor;
    vec4 greenFragColor;
    vec4 blueFragColor;

    // if in red spotlight
    if (redTheta > cutOff) {
        // calculate attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));

        // calculate diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * redLight.diffuseColor * texture(myTexture, TexCoord).rgb;
        
        // factor attenuation into diffuse
        diffuse *= attenuation;

        // set redFragColor as diffuse
        redFragColor = vec4(diffuse, 1.0);
    }

    // if in green spotlight
    if (greenTheta > cutOff) {
        // calculate attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));

        // calculate diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * greenLight.diffuseColor * texture(myTexture, TexCoord).rgb;
        
        // factor attenuation into diffuse
        diffuse *= attenuation;

        // set greenFragColor as diffuse
        greenFragColor = vec4(diffuse, 1.0);
    }
 
    // if in blue spotlight
    if (blueTheta > cutOff) {
        // calculate attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));

        // calculate diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * blueLight.diffuseColor * texture(myTexture, TexCoord).rgb;
        
        // factor attenuation into diffuse
        diffuse *= attenuation;

        // set blueFragColor as diffuse
        blueFragColor = vec4(diffuse, 1.0);
    }

    // if not in any of the spotlights
    if (redTheta < cutOff && greenTheta < cutOff && blueTheta < cutOff)
	{
        // return FragColor as the ambientColor
		FragColor = vec4(ambientColor * 3 * texture(myTexture, TexCoord).rgb, 1.0);
	}

    else {
        // return FragColor which is the sum of all the spotlight colours and ambientColor
        vec4 ambient = vec4(ambientColor * texture(myTexture, TexCoord).rgb, 1.0);
        FragColor = redFragColor + greenFragColor + blueFragColor + ambient + ambient + ambient;
    }
    

}

// Task 1 
// #version 330 core

// in vec3 Normal;
// in vec2 TexCoord;
// out vec4 FragColor;

// uniform sampler2D myTexture;

// void main()
// {
    // FragColor = texture(myTexture, TexCoord);
// }