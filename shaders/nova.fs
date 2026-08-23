#version 410 core
out vec4 FragColor;

uniform float relaxation;

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

dvec2 cdiv(dvec2 a, dvec2 b)
{
    double denom = b.x * b.x + b.y * b.y;
    return dvec2((a.x * b.x + a.y * b.y) / denom, (a.y * b.x - a.x * b.y) / denom);
}

vec3 nova(dvec2 p)
{
    dvec2 c = dvec2(0);
    if (!infiniteZoom)
        c = p;
    else
        c = dvec2(centerX, centerY) + p / pow(1.001f, float(zoomFactor));

    dvec2 z = dvec2(1.0, 0.0);
    int i = 0;
    float smooth_val = 0.0;
    float color_mod = float(maxIter) * colorRange * 0.01f;
    double max_mod = 1.0e8lf;

    while (i < maxIter)
    {
        if (!(z.x * z.x + z.y * z.y <= max_mod))
            break;

        dvec2 z2 = cmul(z, z);
        dvec2 z3 = cmul(z2, z);
        dvec2 numerator = z3 - dvec2(1.0, 0.0);
        dvec2 denominator = 3.0lf * z2;
        dvec2 step = cdiv(numerator, denominator);

        dvec2 zNext = z - double(relaxation) * step + c;

        if (!(zNext.x * zNext.x + zNext.y * zNext.y <= max_mod))
            break;

        smooth_val += exp(-length(vec2(zNext - z)));
        i++;

        if (length(vec2(zNext - z)) < 1e-6)
        {
            z = zNext;
            break;
        }

        z = zNext;
    }

    vec3 color = vec3(0);

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
	vec3 col = nova(pos);
    FragColor = vec4(col,1);
}