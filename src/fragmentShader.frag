#version 330 core

in vec3 vertexColor;
in vec2 texCoord;
out vec4 FragColor;
uniform float sine;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main(){
    if(true){
        FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), sine);
    }
    else{
        FragColor = vec4(vertexColor.rgb, 1.0);
    }
}

