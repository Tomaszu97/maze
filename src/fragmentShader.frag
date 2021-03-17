#version 330 core

in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture0;
uniform float sine;

void main(){
    
    FragColor = mix(vec4(0.5f, 0.3f, 0.7f, 1.0f), texture(texture0, texCoord), 1.0);
    //FragColor = texture(texture0, texCoord);
}

