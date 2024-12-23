#version 330 core

uniform sampler2D tex; // Input texture

in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Sample the original texture color
    vec4 baseColor = texture(tex, tex_coord);

    vec3 horrorColor = vec3(0.5, 0.5, 0.25);

    vec3 finalColor = mix(baseColor.rgb, horrorColor, 0.2);

    // Increase contrast
    finalColor = finalColor * finalColor * (3.0 - 2.0 * finalColor);

    // Darken the scene
    finalColor *= 0.7;

    // Output the final color
    frag_color = vec4(finalColor, baseColor.a);
}