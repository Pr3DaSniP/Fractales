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

#define MAX_PALETTE_SIZE 8
uniform vec3 palette[MAX_PALETTE_SIZE];
uniform int paletteSize;

double modulus_2(dvec2 z)
{
	return z.x * z.x + z.y * z.y;
}

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

dvec2 cmul(dvec2 a, dvec2 b)
{
    return dvec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

vec3 lambdaFractal(dvec2 p)
{
    dvec2 lambda = dvec2(0);
    if (!infiniteZoom)
        lambda = p;
    else
        lambda = dvec2(centerX, centerY) + p / pow(1.001f, float(zoomFactor));

    dvec2 z = dvec2(0.5, 0.0);
    int i = 0;
    vec3 color = vec3(0);
    float color_mod = float(maxIter) * colorRange * 0.01f;

    float smooth_val = exp(-length(vec2(z)));

    double max_mod = smooth_color ? 1000.0lf : 4.0lf;

    while (modulus_2(z) < max_mod && i < maxIter)
    {
        dvec2 oneMinusZ = dvec2(1.0, 0.0) - z;
        z = cmul(lambda, cmul(z, oneMinusZ));
        i++;
        smooth_val += exp(-length(vec2(z)));
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
    double zooming = 1.0lf;
    dvec2 pos;
	pos = (2.5lf * (dvec2(gl_FragCoord.xy) - 0.5lf * dvec2(width, height)) / double(height)) / (zooming + zoom);
    pos += dvec2(-mouseX, mouseY);
	vec3 col = lambdaFractal(pos);
    FragColor = vec4(col,1);
}
