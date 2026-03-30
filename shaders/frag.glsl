#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in float AO;

uniform sampler2D ourTexture;

void main()
{
	vec3 lightDir = normalize(vec3(0.6, 1.0, 0.4));

	// Diffuse lighting
	float diff = max(dot(normalize(Normal), lightDir), 0.0);

	// Ambient so shadows aren't black
	float ambient = 0.65;

	float lighting = ambient + diff * 0.75;

	float minLight = 0.2;
	float ao = mix(minLight, 1.0, AO);

	vec4 texColor = texture(ourTexture, TexCoord);
	FragColor = vec4(texColor.rgb * lighting * ao, texColor.a);
}