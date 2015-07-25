#version 420 core

uniform vec4 Ambient = vec4(0.2f); // sets lighting level, same across many vertices
uniform vec3 LightDirection = vec3(-1.0, -1.0, 1.0);
uniform vec4 LightColor = vec4(1.0f);

uniform bool UseLighting = true;
uniform bool UseStaticColor = false;
uniform vec4 TintColor = vec4(0, 0, 0, 1);

uniform vec4 StaticColor;

uniform sampler2D TexSampler;
uniform bool UseTexture = false;

in vec4 Color;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

void main()
{
	float diffuse = 1.0f;
	vec4 ambient = vec4(0);

	vec4 color;

	if (UseStaticColor)
		color = StaticColor;
	else
		color = Color;

	if (UseTexture)
		color = texture(TexSampler, UV.xy);
	
	vec4 scatteredLight = vec4(1.0f);

	if (UseLighting)
	{
		diffuse = max(0.0, dot(normalize(-LightDirection), Normal));
		scatteredLight = vec4(LightColor.rgb * diffuse, LightColor.a);
		ambient = Ambient;
	}

	FragColor = min(color * (scatteredLight + ambient), vec4(1.0)) + TintColor;
}