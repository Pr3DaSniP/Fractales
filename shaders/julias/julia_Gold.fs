#version 330 core
out vec4 FragColor;

uniform float v1;
uniform float v2;

uniform float maxIter;
uniform float time;
uniform float mouseX;
uniform float mouseY;
uniform float zoom;
uniform float colorRange;
uniform bool smooth_color;

uniform bool infiniteZoom;
uniform float centerX;
uniform float centerY;
uniform float zoomFactor;

uniform int width;
uniform int height;

// Helpers
float modulo(float a, float b)
{
	return a - b * floor(a / b);
}

float modulus_2(vec2 z)
{
	return z.x * z.x + z.y * z.y;
}

// Color palette
vec3[5] Gold() {
	vec3 pallet[5];
	pallet[0] = vec3(85.f	/ 255.f,	47.f	/ 255.f,	0.f		/ 255.f);
	pallet[1] = vec3(255.f	/ 255.f,	171.f	/ 255.f,	12.f	/ 255.f);
	pallet[2] = vec3(255.f	/ 255.f,	247.f	/ 255.f,	127.f	/ 255.f);
	pallet[3] = vec3(255.f	/ 255.f,	171.f	/ 255.f,	12.f	/ 255.f);
	pallet[4] = vec3(85.f	/ 255.f,	47.f	/ 255.f,	0.f		/ 255.f);
	return pallet;
}

vec3 get_color(float iterations)
{
	int nbColors = 5;
	vec3[5] pallet = Gold();

	float value = iterations / float(maxIter);
	vec3 color = vec3(0);

	float min_value;
	float max_value;

	for (int i = 0; i < int(nbColors - 1); i++)
	{
		min_value = float(i) / nbColors;
		max_value = float(i + 1) / nbColors;

		if (value >= min_value && value <= max_value)
		{
			color = mix(pallet[i], pallet[i + 1], (value - min_value) * nbColors);
            break;
		}
	}

	return color;
}

vec3 julia(vec2 p) 
{
    vec2 number = p;
    vec2 c = vec2(v1, v2);
    vec2 temp = vec2(0);
    int i = 0;
    vec3 color = vec3(0);
    float color_mod = float(maxIter) * colorRange * 0.01f;

    float smooth_val = exp(-length(number));

    if(!infiniteZoom)
        number = p;
    else {
        number = vec2(centerX, centerY) + p / (pow(1.001, zoomFactor));
	}

    float max_mod = smooth_color ? 1000.0 : 4.0;

    while (modulus_2(number) < max_mod && i < maxIter) 
    {
        temp = number;
        number.x = temp.x * temp.x - temp.y * temp.y + c.x;
        number.y = 2.0 * temp.x * temp.y + c.y;
        i++;
        smooth_val += exp(-length(number));
    }

    if (i == maxIter) {
        color = vec3(0.0);
    }
    else if (smooth_color)
    {
        float value = mod(smooth_val, float(maxIter) / color_mod);
        color = get_color(value);
    }
    else 
    {
        int shifted_i = i * int( maxIter / color_mod) % int(maxIter);
        float value = mod(float(shifted_i), float(maxIter) / color_mod);
        color = get_color(value);
    }

    return color;
}

void main()
{
    float zooming = 1.0;
    vec2 pos;
	pos = (2.5*(gl_FragCoord.xy - 0.5 * vec2(width, height)) / float(height)) / (zooming + zoom);
    pos += vec2(-mouseX, mouseY);
	vec3 col = julia(pos);
    FragColor = vec4(col,1);
}