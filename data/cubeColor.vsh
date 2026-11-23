uniform mat4 transformed;
uniform vec4 u_color;

in vec4 a_position;
in vec4 a_color;

out vec4 v_color;

void main()
{
    gl_Position = transformed * a_position;
    v_color = a_color * u_color;
}