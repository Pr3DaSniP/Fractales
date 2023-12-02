#version 330 core
out vec4 FragColor;

uniform float maxIter;
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

uniform int numberOfBrot;

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

float puissance(float x, float n)
{
	float res = 1.0;
	for(int i = 0; i < n; i++)
		res *= x;
	return res;
}

vec3 multibrot(vec2 p, int n) 
{
	vec2 z = p;
	vec2 temp = vec2(0.0, 0.0);
	vec3 color = vec3(0, 0, 0);
	float color_mod = float(maxIter) * colorRange * 0.01f;

	float smooth_val = exp(-length(temp));

    int iter = 0;

	vec2 c = p;

	float max_mod = smooth_color ? 1000.0 : 4.0;

    while (z.x*z.x + z.y*z.y < max_mod && iter < maxIter)
    {
		float tmpVal = z.x*z.x+z.y*z.y;
		temp.x = puissance(tmpVal, n/2)*cos(n*atan(z.y,z.x)) + c.x;
		temp.y = puissance(tmpVal, n/2)*sin(n*atan(z.y,z.x)) + c.y;
        
		z.x = temp.x;
		z.y = temp.y;

		iter++;

		smooth_val += exp(-length(temp));
    }

	if (iter == maxIter) {
        color = vec3(0.0);
    }
    else if (smooth_color)
    {
        float value = mod(smooth_val, float(maxIter) / color_mod);
        color = get_color(value);
    }
    else 
    {
        int shifted_i = iter * int( maxIter / color_mod) % int(maxIter);
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
	vec3 col = multibrot(pos, numberOfBrot);
	FragColor = vec4(col, 1.0);
}