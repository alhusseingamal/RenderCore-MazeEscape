// #version 330 core

sampler2D albedo;
sampler2D specular;
sampler2D roughness;
sampler2D ambient_occlusion;
sampler2D emissive; 





struct Light {
    vec3 position;
    vec3 color;
    vec3 direction;
    vec3 attenuation;
    vec3 cone_angles;
}

uniform Light light;


in Varyings {
    vec4 color;
    vec2 textureCoordinates;
    vec3 normal;
    vec3 view;
} fs_datain;
 
 out vec4 FragColor;

void main() {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Not implemented yet

    // vec3 diffuse = texture(albedo, fs_datain.textureCoordinates).rgb;
    // vec3 specular = texture(specular, fs_datain.textureCoordinates).rgb;
    // float roughness = texture(roughness, fs_datain.textureCoordinates).r;
    // vec3 ambient = diffuse * texture(ambient_occlusion, fs_datain.textureCoordinates).r;
    // vec3 emissive = texture(emissive, fs_datain.textureCoordinates).r;

    // float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // Fadl dealing with multiple light sources


    vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0)); // from up to down
    // vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // IDK if 1, 1, 1, is correct

    float d = length(lightDirection);
    lightDirection /= d; // become weak when we are far

    
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor.rgb *= max(0.0, dot(lightDirection, fs_datain.normal)); // number won't be negative

    float lambert = max(dot(normalize(fs_datain.normal, lightDirection)), 0.0);


    // diffuse= kd * Id * lambert;
    vec3 kd = vec3(0.5, 0.5, 0.5); // diffuse reflection coefficient of the material to be changed
    vec3 Id = vec3(1.0, 1.0, 1.0); // intensity of the light source
    vec3 diffuse = kd * Id * lambert;

    // ambient = ka * Ia
    vec3 ka = vec3(0.1, 0.1, 0.1); // ambient reflection coefficient of the material to be changed
    vec3 light_ambient = vec3(0.1, 0.1, 0.1); // ambient light
    vec3 ambient = ka*light_ambient;

    // specular = ks * Is * (R*V)^n
    view=normalize(fs_datain.view);
    normal=normalize(fs_datain.normal);
    float alpha = 30; // shininess of the material to be changed
    vec3 ks = vec3(0.5, 0.5, 0.5); // specular reflection coefficient of the material to be changed
    vec3 Is = vec3(1.0, 1.0, 1.0); // intensity of the light source
    vec3 r = reflect(-lightDirection, normalize(fs_datain.normal))
    vec3 specular = ks * Is * pow(max(dot(r, view), 0.0), alpha);


    float attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

    float angle = cos(dot(light.direction, -lightDirection));
    attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle) // reduce attenuation


    FragColor = vec4(ambient+ (diffuse+specular)/attenuation, 1.0);
}


