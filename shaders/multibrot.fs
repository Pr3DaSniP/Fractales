#version 410 core
out vec4 FragColor;

uniform float maxIter;
uniform double mouseX;
uniform double mouseY;
uniform double zoom;
uniform float colorRange;
uniform bool smooth_color;

uniform bool infiniteZoom;
uniform double centerX;
uniform double centerY;
uniform double zoomFactor;

uniform int width;
uniform int height;

uniform int numberOfBrot;

#define MAX_PALETTE_SIZE 8
uniform vec3 palette[MAX_PALETTE_SIZE];
uniform int paletteSize;

vec3 get_color(float iterations)
{
	float value = iterations / float(maxIter);
	vec3 color = vec3(0);

	float min_value;
	float max_value;

	for (int i = 0; i < paletteSize - 1; i++)
	{
		min_value = float(i) / float(paletteSize);
		max_value = float(i + 1) / float(paletteSize);

		if (value >= min_value && value <= max_value)
		{
			color = mix(palette[i], palette[i + 1], (value - min_value) * float(paletteSize));
			break;
		}
	}

	return color;
}

double puissance(double x, float n)
{
	double res = 1.0lf;
	for (int i = 0; i < n; i++)
		res *= x;
	return res;
}

vec3 multibrot(dvec2 p, int n)
{
	dvec2 z = p;
	dvec2 temp = dvec2(0.0, 0.0);
	vec3 color = vec3(0, 0, 0);
	float color_mod = float(maxIter) * colorRange * 0.01f;

	float smooth_val = exp(-length(vec2(temp)));

    int iter = 0;

	dvec2 c = p;

	double max_mod = smooth_color ? 1000.0lf : 4.0lf;

    while (z.x * z.x + z.y * z.y < max_mod && iter < maxIter)
    {
		double tmpVal = z.x * z.x + z.y * z.y;
		float angle = float(n) * atan(float(z.y), float(z.x));
		temp.x = puissance(tmpVal, n / 2) * double(cos(angle)) + c.x;
		temp.y = puissance(tmpVal, n / 2) * double(sin(angle)) + c.y;

		z.x = temp.x;
		z.y = temp.y;

		iter++;

		smooth_val += exp(-length(vec2(temp)));
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
	double zooming = 1.0lf;
	dvec2 pos;
	pos = (2.5lf * (dvec2(gl_FragCoord.xy) - 0.5lf * dvec2(width, height)) / double(height)) / (zooming + zoom);
    pos += dvec2(-mouseX, mouseY);
	vec3 col = multibrot(pos, numberOfBrot);
	FragColor = vec4(col, 1.0);
}
