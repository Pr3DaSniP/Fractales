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
