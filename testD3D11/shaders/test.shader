
cbuffer ConstantBuffer : register( b0 )
{
	matrix worldViewProj;
}

struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
};

VertexShaderOutput vertexShaderMain( float4 pos : position ) : SV_POSITION
{
    VertexShaderOutput output;
    output.pos = mul( pos, worldViewProj );
    return output;
}

float4 pixelShaderMain( VertexShaderOutput input ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );
}
