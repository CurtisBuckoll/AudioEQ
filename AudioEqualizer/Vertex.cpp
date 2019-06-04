/*========================================================================

    Name:     Vertex.cpp

    Date:     January 2017

    Author:   Curtis Buckoll

    Overview:

        Implementation of Vertex.h: general purpose vector, colour, and 
    vertex structs with some useful operations.

========================================================================*/

#include "stdafx.h"

#include "Vertex.h"

#include <math.h>
#include <iostream>

//========================================================================
//
vec4::vec4()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

//========================================================================
//
vec4::vec4( float x, float y, float z, float w /*= 1*/ )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

//========================================================================
//
void vec4::normalize()
{
    float length = (float)sqrt( pow( x, 2 ) + pow( y, 2 ) + pow( z, 2 ) + pow( w, 2 ) );

	if (length != 0)
	{
		x = x / length;
		y = y / length;
		z = z / length;
		w = w / length;
	}
}

//========================================================================
//
void vec4::hgDivision()
{
	if (w != 0)
	{
		x = x / w;
		y = y / w;
		z = z / w;
		w = 1.0f;
	}
}

//========================================================================
//
float vec4::dot( const vec4& rhs ) const
{
    return ( x * rhs.x ) + ( y * rhs.y ) + ( z * rhs.z ) + ( w * rhs.w );
}

//========================================================================
vec4 vec4::cross( const vec4& V2 ) const
{
    return vec4( y * V2.z - z * V2.y, z * V2.x - x * V2.z, x * V2.y - y * V2.x, 0.0f );
}

//========================================================================
// Returns the EUCLIDEAN magnitude of a vector - ignores the w component
float vec4::length()
{
    return (float)sqrt( pow( x, 2 ) + pow( y, 2 ) + pow( z, 2 ) );
}

//========================================================================
//
void vec4::print()
{
	std::cout << "x: " << x << " y: " << y << " z: " << z << " w: " << w << std::endl;
}

//========================================================================
//
float vec4::operator[]( int i ) const
{
	switch (i)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		return 0.0f;
	}
}

//========================================================================
//
vec4 vec4::operator*( float scalar ) const
{
    vec4 result = vec4( x * scalar, y * scalar, z * scalar, w );
	return result;
}

//========================================================================
//
vec4 vec4::operator-( const vec4& p2 ) const
{
    vec4 result = vec4( x - p2.x, y - p2.y, z - p2.z, w - p2.w );
	return result;
}

//========================================================================
//
vec4 vec4::operator+(const vec4& p2) const
{
    vec4 result = vec4( x + p2.x, y + p2.y, z + p2.z, w + p2.w );
	return result;
}

//========================================================================
//
bool vec4::operator==(const vec4& p2) const
{
    return ( x == p2.x ) && ( y == p2.y ) && ( z == p2.z ) && ( w == p2.w );
}

//========================================================================
//
bool vec4::operator!=(const vec4& p2) const
{
    return !( ( x == p2.x ) && ( y == p2.y ) && ( z == p2.z ) && ( w == p2.w ) );
}

//========================================================================
//
Color::Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

//========================================================================
//
Color::Color( float R, float G, float B )
{
    r = R;
    g = G;
    b = B;
}

//========================================================================
//
Color Color::operator*( Color rhs ) const
{
    return Color( r * rhs.r, g * rhs.g, b * rhs.b );
}

//========================================================================
//
Color Color::operator*(float scalar) const
{
    return Color( r * scalar, g * scalar, b * scalar );
}

//========================================================================
//
Color Color::operator+(Color rhs) const
{
    return Color( r + rhs.r, g + rhs.g, b + rhs.b );
}

//========================================================================
//
Color Color::operator-(Color rhs) const
{
    return Color( r - rhs.r, g - rhs.g, b - rhs.b );
}

//========================================================================
//
Color256::Color256()
{
    r = 0;
    g = 0;
    b = 0;
}

//========================================================================
//
Color256::Color256(unsigned char R, unsigned char G, unsigned char B)
{
    r = R;
    g = G;
    b = B;
}

//========================================================================
//
Color256::Color256(Color color)
{
    r = (unsigned char)( 255 * color.r );
    g = (unsigned char)( 255 * color.g );
    b = (unsigned char)( 255 * color.b );
}

//========================================================================
//
unsigned int Color256::packColor()
{
    return (unsigned int)( ( 0xff << 24 ) + ( r << 16 ) + ( g << 8 ) + b );
}

//========================================================================
//
Color Color256::convertToFloat()
{
    return Color( (float)r / 255, (float)g / 255, (float)b / 255 );
}

//========================================================================
//
bool Color256::operator==( const Color256& rhs ) const
{
    if( r == rhs.r && g == rhs.g && b == rhs.b )
    {
        return true;
    }
    return false;
}

//========================================================================
//
Color256 Color256::operator*( float scalar ) const
{
    return Color256( (unsigned char)( r * scalar ), 
                     (unsigned char)( g * scalar ), 
                     (unsigned char)( b * scalar ) );
}

//========================================================================
//
Vertex::Vertex()
{

}

//========================================================================
//
Vertex::Vertex(const vec4& Pos, const vec4& Normal, Color256 Colour)
    : pos( Pos )
    , normal( Normal )
    , color( Colour )
{

}

//========================================================================
//
void Vertex::print()
{
    std::cout << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z;
    std::cout << " r: " << (int)color.r << " g: " << (int)color.b << " b: " << (int)color.b << std::endl;
}
