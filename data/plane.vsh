uniform mat4 transformed;
uniform vec4 u_color;

in vec4 a_position;

out vec4 v_color;

void main()
{
    gl_Position = transformed * a_position;
    v_color = u_color;
}