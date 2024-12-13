#version 330 core

in Varyings {
    vec4 color;
    vec2 textureCoordinates;
    vec3 normal;
    vec3 view;
} fs_datain;

void main() {
    vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0)); // from up to down
    float lambert = max(dot(normalize(fs_datain.normal, lightDirection)), 0.0);
    // diffuse= kd * Id * lambert
    vec3 kd = vec3(0.5, 0.5, 0.5); // diffuse reflection coefficient of the material to be changed
    vec3 Id = vec3(1.0, 1.0, 1.0); // intensity of the light source
    vec3 diffuse = kd * Id * lambert;
    // ambient = ka * Ia
    vec3 light_ambient = vec3(0.1, 0.1, 0.1); // ambient light
    vec3 ka = vec3(0.1, 0.1, 0.1); // ambient reflection coefficient of the material to be changed
    vec3 ambient = ka*light_ambient;
    // specular = ks * Is * (R*V)^n
    view=normalize(fs_datain.view);
    normal=normalize(fs_datain.normal);
    float alpha = 30; // shininess of the material to be changed
    vec3 ks = vec3(0.5, 0.5, 0.5); // specular reflection coefficient of the material to be changed
    vec3 Is = vec3(1.0, 1.0, 1.0); // intensity of the light source
    vec3 r = reflect(-lightDirection, normalize(fs_datain.normal))
    vec3 specular = ks * Is * pow(max(dot(r, view), 0.0), alpha);
    FragColor = vec4(diffuse+ambient+specular, 1.0);
}