{
    vec4 gl_FragColor;
    vec4 a = vec4(2.,2.,2.,3.);
    vec4 b= vec4(2.,2.,2.,3.);
    vec3 c= vec3(2.,2.,3.);

    a = -----gl_FragColor + gl_FragCoord;
    b = c + a + (gl_FragCoord[2] + c);
}