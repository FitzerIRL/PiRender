// vertexShader.glsl

// precision mediump float;  // GLES 3+

/*
attribute vec2 vPosition;

void main() {
   gl_Position = vec4(vPosition.xy, 0.0, 1.0);
}
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - -

// vertexShader.glsl

attribute vec4 aPosition;
uniform mat4 u_mvpMatrix;

void main()
{
   gl_Position = u_mvpMatrix * vec4(aPosition.xyz, 1.0);
}

