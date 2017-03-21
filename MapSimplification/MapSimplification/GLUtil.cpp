#include "GLUtil.h"

/// <summary>
/// Output error message
/// </summary>
/// <param name="message">The message</param>
bool OutErrorMsg(LPCSTR message)
{
	MessageBoxA(NULL, message, NULL, NULL);
	return false;
}


/// <summary>
/// Used to draw a 2D string using OpenGL
/// </summary>
/// <param name="text">The text to be drawn</param>
/// <param name="stringtype"></param>
/// <param name="x">The x position of the string (top left)</param>
/// <param name="y">The y position of the string (top left)</param>
void DrawString(std::string text, void* stringtype, int x, int y)
{
	glRasterPos2i(x, y);
	for(int i = 0; i < text.length(); i++)
	{
		glutBitmapCharacter(stringtype, text.c_str()[i]);	
	}
}

/// <summary>
/// Interpolates between 2 values using a variable t
/// </summary>
/// <param name="v0">First value</param>
/// <param name="v1">Second value</param>
/// <param name="t">Interpolation value (0 to 1)</param>
float Lerp(float v0, float v1, float t) 
{
	return (1-t)*v0 + t*v1;
}

/// <summary>
/// Clamps 2 values between a minimum and maximum value
/// </summary>
/// <param name="min">Minimum value</param>
/// <param name="max">Maximum value</param>
/// <param name="val">The value to be clamped</param>
float Clamp(float min, float max, float val)
{
	if (val < min)
		val = min;
	if (val > max)
		val = max;

	return val;
}