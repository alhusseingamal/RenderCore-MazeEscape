// #version 330 core

sampler2D albedo; //An Albedo Map is basically an Image texture without any shadows or highlights
sampler2D specular;
sampler2D roughness;
sampler2D ambient_occlusion;
sampler2D emissive; to be added to the color directly

#define Directional_Light 0
#define Point_Light 1
#define Spot_Light 2

#define MAX_LIGHTS 10
struct Light {
    int type; // 0: directional, 1: point, 2: spot
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation; // constant, linear, quadratic
    float inner_cone_angle;
    float outer_cone_angle;
};
struct Material {
    // sampler2D diffuse; //= albedo
    // sampler2D specular; // = specular
    // sampler2D ambient; // = AO * albedo
    // sampler2D shininess; // =2/((roughness)^4)-2
    sampler2D albedo_map;
    sampler2D specular_map;
    sampler2D roughness_map;
    sampler2D ambient_occlusion_map;
    sampler2D emissive_map;
};
uniform Material material;

in Varyings {
    vec4 color;
    vec2 textureCoordinates;
    vec3 normal;
    vec3 view;
    vec3 worldPos; // world position of the fragment
} fs_datain;

uniform Light lights[MAX_LIGHTS];
uniform int light_count;
uniform vec3 ambient_light;
out vec4 FragColor;


void main() {


    vec3 view=normalize(fs_datain.view);
    vec3 normal=normalize(fs_datain.normal);
    //material properties
    float material_roughness = texture(material.roughness_map, fs_datain.textureCoordinates).r;
    float material_shininess = 2.0 / pow(clamp(material_roughness,0.001,0.999), 4.0) - 2.0;
    float material_ambient_occlusion = texture(material.ambient_occlusion_map, fs_datain.textureCoordinates).r;
    
    vec3 material_diffuse = texture(material.albedo_map, fs_datain.textureCoordinates).rgb;
    vec3 material_specular = texture(material.specular_map, fs_datain.textureCoordinates).rgb;
    vec3 material_ambient = material_diffuse * material_ambient_occlusion;
    vec3 material_emissive = texture(material.emissive_map, fs_datain.textureCoordinates).rgb;
    //////////////////////////
    vec3 ambient = ambient_light * material_ambient;
    vec3 color = vec3(0.0, 0.0, 0.0);
    color += ambient;
    color += material_emissive;

    for(int i = 0; i < light_count; i++) {
    Light light = lights[i];

    vec3 light_Direction;
    float attenuation=1;

    if (light.type == Directional_Light) {
        light_Direction = -light.direction;
        // attenuation = 1.0;
    } else  {
        frag_light_vector = light.position - fs_datain.worldPos;
        float d = length(frag_light_vector);
        light_Direction = frag_light_vector / d;
        attenuation = 1.0 / dot(light.attenuation, vec3(1, d, d*d));
        if (light.type == Spot_Light) {
            float theta_s = acos(dot(light.direction, -light_Direction)); // angle between light direction and direction to the fragment
            attenuation *= smoothstep(light.outer_cone_angle, light.inner_cone_angle, theta_s);
        }
    }
    

    float lambert = max(dot(normal, lightDirection), 0.0);
    // diffuse= kd * Id * lambert;
    // vec3 kd = vec3(0.5, 0.5, 0.5); // diffuse reflection coefficient of the material to be changed
    // vec3 Id = vec3(1.0, 1.0, 1.0); // intensity of the light source
    vec3 diffuse = material_diffuse * light.color * lambert;
    // ambient = ka * Ia
    // vec3 ka = vec3(0.1, 0.1, 0.1); // ambient reflection coefficient of the material to be changed
    // vec3 light_ambient = vec3(0.1, 0.1, 0.1); // ambient light
    
    // specular = ks * Is * (R*V)^n
    vec3 r = reflect(-lightDirection, normal)
    float phong = pow(max(dot(r, view), 0.0), material_shininess);
    vec3 specular = light.color * material_specular * phong;
    color += (diffuse + specular) * attenuation;
    }
}



// struct Directional_Light {
//     vec3 direction;
//     vec3 color;
// }
// vec3 calculateDirectionalLight(Directional_Light light, vec3 normal, vec3 view) { // normal and view should be normalized
//     vec3 lightDirection = -light.direction; // light direction should be normalized
//     float lambert = max(dot(normal, lightDirection), 0.0);
//     vec3 diffuse = lambert * light.color* material.diffuse;
//     vec3 reflectDir = reflect(-lightDirection, normal);
//     float phong = pow(max(dot(view, reflectDir), 0.0), material.alpha);
//     vec3 specular = phong * light.color * material.specular;
//     vec3 ambient = material.ambient * ambient_light;
//     return (diffuse + specular + ambient);
// }
// struct Point_Light {
//     vec3 position;
//     vec3 color;
//     vec3 attenuation; // constant, linear, quadratic
//     //Attenuation is applied only on the diffuse & Specular components
// }
// vec3 calculatePointLight(Point_Light light, vec3 normal, vec3 view, vec3 fragPos) { // normal should be normalized
//     vec3 frag_light_vector = (light.position - fragPos); // light direction should be normalized
//     float distance = length(frag_light_vector);
//     vec3 light_direction=frag_light_vector/distance;

//     float attenuation = 1.0 / dot(light.attenuation, vec3(1.0, distance, distance * distance));

//     float lambert = max(dot(normal, light_direction), 0.0);
//     vec3 diffuse = lambert * light.color* material.diffuse;

//     vec3 reflectDir = reflect(-light_direction, normal);

//     float phong = pow(max(dot(view, reflectDir), 0.0), material.alpha);
//     vec3 specular = phong * light.color * material.specular;
//     vec3 ambient = material.ambient * ambient_light;
//     return (ambient + (diffuse + specular) * attenuation); // to be used in a vec4 color with alpha = 1.0
// }
// struct Spot_Light {
//     vec3 position;
//     vec3 direction;
//     vec3 color;
//     vec3 attenuation;
//     vec3 cone_angles;
// }
// vec3 calculateSpotLight(Spot_Light light, vec3 normal, vec3 view, vec3 fragPos) { // normal should be normalized
//     vec3 lightDirection = normalize(light.position - fragPos);
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / dot(light.attenuation, vec3(distance * distance, distance, 1.0));
//     float angle = cos(dot(light.direction, -lightDirection));
//     attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
//     float lambert = max(dot(normal, lightDirection), 0.0);
//     vec3 diffuse = lambert * light.color;
//     vec3 reflectDir = reflect(-lightDirection, normal);
//     float spec = pow(max(dot(view, reflectDir), 0.0), 32);
//     vec3 specular = spec * light.color;
//     return (diffuse + specular) * attenuation;
// }

// uniform Directional_Light dir_light;
// uniform Point_Light point_light;
// uniform Spot_Light spot_light;
