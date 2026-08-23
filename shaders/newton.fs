#version 410 core
out vec4 FragColor;

uniform float maxIter;
uniform double mouseX;
uniform double mouseY;
uniform double zoom;

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

dvec2 cmul(dvec2 a, dvec2 b)
{
    return dvec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

dvec2 cdiv(dvec2 a, dvec2 b)
{
    double denom = b.x * b.x + b.y * b.y;
    return dvec2((a.x * b.x + a.y * b.y) / denom, (a.y * b.x - a.x * b.y) / denom);
}

vec3 newton(dvec2 p)
{
    dvec2 z = p;
    if (infiniteZoom)
        z = dvec2(centerX, centerY) + p / pow(1.001f, float(zoomFactor));

    int i = 0;

    while (i < maxIter)
    {
        dvec2 z2 = cmul(z, z);
        dvec2 z3 = cmul(z2, z);
        dvec2 numerator = z3 - dvec2(1.0, 0.0);
        dvec2 denominator = 3.0lf * z2;
        dvec2 step = cdiv(numerator, denominator);

        z = z - step;
        i++;

        if (modulus_2(step) < 1e-12)
            break;
    }

    dvec2 roots[3] = dvec2[3](
        dvec2(1.0, 0.0),
        dvec2(-0.5, 0.8660254037844386),
        dvec2(-0.5, -0.8660254037844386)
    );

    int nearestRoot = 0;
    double bestDist = modulus_2(z - roots[0]);
    for (int r = 1; r < 3; r++)
    {
        double dist = modulus_2(z - roots[r]);
        if (dist < bestDist)
        {
            bestDist = dist;
            nearestRoot = r;
        }
    }

    vec3 baseColor = palette[nearestRoot % paletteSize];
    float shade = clamp(1.0 - float(i) / float(maxIter) + 0.3, 0.0, 1.0);

    return baseColor * shade;
}

void main()
{
    double zooming = 1.0lf;
    dvec2 pos;
	pos = (2.5lf * (dvec2(gl_FragCoord.xy) - 0.5lf * dvec2(width, height)) / double(height)) / (zooming + zoom);
    pos += dvec2(-mouseX, mouseY);
	vec3 col = newton(pos);
    FragColor = vec4(col,1);
}
