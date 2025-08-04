//https://gist.github.com/postspectacular/2a4a8db092011c6743a7

float fract(float x) {
  return x - int(x);
}
float mix(float k, float l, float t) {
  return k + (l - k) * t;
}

float* hsv2rgb(float h, float s, float v, float* rgb) {
  rgb[0] = v * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = v * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = v * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}
