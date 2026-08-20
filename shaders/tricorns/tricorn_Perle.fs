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

float conjug(float x, float y)
{
	return x*x - y*y;
}

float puissance(float x, float n)
{
	float res = 1.0;
	for(int i = 0; i < n; i++)
		res *= x;
	return res;
}

vec3[6] Perle() {
    vec3 pallet[6];
    pallet[0] = vec3(10.f	/ 255.f,	9.f		/ 255.f,	12.f	/ 255.f);
    pallet[1] = vec3(240.f	/ 255.f,	237.f	/ 255.f,	238.f	/ 255.f);
    pallet[2] = vec3(7.f	/ 255.f,	57.f	/ 255.f,	60.f	/ 255.f);
    pallet[3] = vec3(44.f	/ 255.f,	102.f	/ 255.f,	110.f	/ 255.f);
    pallet[4] = vec3(144.f	/ 255.f,	221.f	/ 255.f,	240.f	/ 255.f);
    pallet[5] = vec3(10.f	/ 255.f,	9.f		/ 255.f,	12.f	/ 255.f);
    return pallet;
}

vec3 get_color(float iterations)
{
	int nbColors = 6;
	vec3[6] pallet = Perle();

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

vec3 tricorn(vec2 p)
{
	int iter = 0;
	vec2 z = p;
	vec2 temp = vec2(0.0, 0.0);
	float color_mod = float(maxIter) * colorRange * 0.01f;

	float smooth_val = exp(-length(temp));

	while(z.x*z.x + z.y*z.y < 4.0 && iter < maxIter)
	{
		temp.x = z.x * z.x - z.y * z.y + p.x;
		temp.y = -2.0 * z.x * z.y + p.y;
		z = temp;
		iter++;
		smooth_val += exp(-length(temp));
	}

	vec3 color = vec3(0);

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
	vec3 col = tricorn(pos);
	FragColor = vec4(col, 1.0);
}