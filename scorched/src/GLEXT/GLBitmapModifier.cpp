////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////


// GLBitmapModifier.cpp: implementation of the GLBitmapModifier class.
//
//////////////////////////////////////////////////////////////////////

#include <vector>
#include <math.h>
#include <GLEXT/GLBitmapItterator.h>
#include <GLEXT/GLBitmapModifier.h>
#include <common/Defines.h>

static const float ambientLightConst = 0.2f;
static const float directLightConst = 0.8f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool GLBitmapModifier::findIntersection(HeightMap &hMap,
										Vector start,
										Vector end,
										float &dist,
										float stopDist)
{
	bool result = false;
	dist = 0.0f;
	Vector point = start;
	Vector direction = end - start;
	float &pt0 = point[0];
	float &pt1 = point[1];
	float &pt2 = point[2];
	int width = hMap.getWidth();

	// Calculate how many pixels to jump for each itteration
	if (fabsf(direction[0]) > fabsf(direction[1])) direction /= fabsf(direction[0]);
	else direction /= fabsf(direction[1]);

	while (pt0 >= 0.0f && pt1 >= 0.0f &&
		pt0 <= width && pt1 <= width)
	{
		float height = hMap.getHeight(int(point[0]), int(point[1])) - 0.1f;
		float rayHeight = height - pt2;
		if (rayHeight > 0.0f)
		{
			if (rayHeight > dist) dist = rayHeight;
			result = true;
			if (dist > stopDist) return result;
		}

		point += direction;
	}

	return result;
}

void GLBitmapModifier::addLightMapToBitmap(GLBitmap &destBitmap, 
										   HeightMap &hMap,
										   Vector &sunPos,
										   ProgressCounter *counter)
{
	const float softShadow = 3.0f;
	const int sideFade = 16;
	const int mapWidth = 256;

	if (counter) counter->setNewOp("Light Map");

	// Itterate the dest bitmap pixels
	GLubyte *bitmap = new GLubyte[mapWidth * mapWidth];
	GLubyte *bitmapBits = bitmap;
	int y;
	for (y=0; y<mapWidth; y++)
	{
		if (counter) counter->setNewPercentage(100.0f * float(y) / float(mapWidth));

		for (int x=0; x<mapWidth; x++)
		{
			float dx = float(x)/float(mapWidth)*float(hMap.getWidth());
			float dy = float(y)/float(mapWidth)*float(hMap.getWidth());
			float dz = hMap.getInterpHeight(dx, dy);
			
			Vector testPosition(dx, dy, dz);
			Vector testNormal;
			hMap.getInterpNormal(dx, dy, testNormal);
			Vector sunDirection = (sunPos - testPosition).Normalize();

			// Calculate lighting
			float diffuseLight = directLightConst * (testNormal.dotP(sunDirection));
			float ambientLight = ambientLightConst;

			// Calculate light based on whether obejcts in path
			float dist = 0.0f;
			if (findIntersection(hMap, testPosition, sunPos, dist, softShadow))
			{
				// An object is in the path
				if (dist < softShadow)
				{
					// The object is only just in the path
					// Create soft shadow
					diffuseLight *= 1.0f - (dist / softShadow);
				}
				else
				{
					// Totaly in path, dark shadow
					diffuseLight = 0.0f;
				}
			}
			float lightIntense = diffuseLight + ambientLight;
			if (lightIntense > 1.0f) lightIntense = 1.0f;
			else if (lightIntense < 0.0f) lightIntense = 0.0f;

			// Fade the lightmap at the sides of the bitmap
			// All light -> 1 at borders
			int fadeAmount = sideFade;
			bool fade = false;
			if (x < sideFade)
			{
				fade = true;
				fadeAmount = MIN(fadeAmount, x);
			}
			if (y < sideFade)
			{
				fade = true;
				fadeAmount = MIN(fadeAmount, y);
			}
			if (x + sideFade > mapWidth)
			{
				fade = true;
				fadeAmount = MIN(fadeAmount, mapWidth - x);
			}
			if (y + sideFade > mapWidth)
			{
				fade = true;
				fadeAmount = MIN(fadeAmount, mapWidth - y);
			}
			if (fade)
			{
				float useIntense = lightIntense * fadeAmount / float(sideFade);
				float useFade = 1.0f - (fadeAmount / float(sideFade));
				lightIntense = useIntense + useFade;
			}

			GLubyte lightColor = GLubyte (lightIntense * 255.0f);
			*bitmapBits = lightColor;
			bitmapBits ++;
		}
	}

	GLubyte *copyDest = new GLubyte[destBitmap.getWidth() * destBitmap.getHeight()];
	gluScaleImage(
		GL_LUMINANCE, 
		mapWidth, mapWidth, 
		GL_UNSIGNED_BYTE, bitmap,
		destBitmap.getWidth(), destBitmap.getHeight(), 
		GL_UNSIGNED_BYTE, copyDest);
	
	GLubyte *srcBits = copyDest;
	GLubyte *destBits = destBitmap.getBits();
	for (y=0; y<destBitmap.getHeight(); y++)
	{
		for (int x=0; x<destBitmap.getWidth(); x++)
		{
			// Note 225 to lighten the darkened bitmap (due to rescale)
			float src = float(*srcBits) / 225.0f;
			if (src < 1.0f)
			{
				destBits[0] = GLubyte(float(destBits[0]) * src);
				destBits[1] = GLubyte(float(destBits[1]) * src);
				destBits[2] = GLubyte(float(destBits[2]) * src);		
			}

			srcBits ++;
			destBits += 3;
		}
	}

	delete [] bitmap;
	delete [] copyDest;
}

void GLBitmapModifier::addHeightToBitmap(HeightMap &hMap,
										 GLBitmap &destBitmap, 
										 GLBitmap &slopeBitmap,
										 GLBitmap &shoreBitmap,
										 GLBitmap **origHeightBitmaps,
										 int numberSources,
										 int destBitmapScaleSize,
										 ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Height Map");

	const float maxHeight = 30.0f; // Last texture ends at height 30
	const float blendHeightFactor = 0.4f; // Ends blend when 40% into height band
	const float blendNormalSlopeStart = 0.8f; // Starts blending slope at .80
	const float blendNormalSlopeLength = 0.3f; // Blends when 30% more slope
	const float blendNormalShoreStart = 0.8f; // Starts the sand
	const float blendNormalShoreLength = 0.1f; // Amount of sand
	const float noiseMax = 0.4f;

	float hMapMaxHeight = 0;
	for (int ma=0; ma<hMap.getWidth(); ma++)
	{
		for (int mb=0;mb<hMap.getWidth(); mb++)
		{
			float height = hMap.getHeight(ma, mb);
			if (height > hMapMaxHeight) hMapMaxHeight = height;
		}
	}

	// Create new bitmaps with the bitmap scaled to the correct size
	GLBitmap **heightBitmaps = new GLBitmap*[numberSources];
	GLBitmapItterator ** bitmapItors = new GLBitmapItterator*[numberSources+2];
	float bitmapScale = float(destBitmap.getWidth()) / float(destBitmapScaleSize);

	// Create a bitmap iterator for each bitmap
	// Create a bitmap correctly scaled to the scene
	int i;
	for (i=0; i<numberSources; i++)
	{
		if (bitmapScale != 1.0f)
		{
			// Create the newly scaled bitmaps
			heightBitmaps[i] = new GLBitmap(
				int(bitmapScale * origHeightBitmaps[i]->getWidth()),
				int(bitmapScale * origHeightBitmaps[i]->getHeight()));

			// Scale bitmap
			gluScaleImage(
				GL_RGB, 
				origHeightBitmaps[i]->getWidth(), origHeightBitmaps[i]->getHeight(), GL_UNSIGNED_BYTE, origHeightBitmaps[i]->getBits(),
				heightBitmaps[i]->getWidth(), heightBitmaps[i]->getHeight(), GL_UNSIGNED_BYTE, heightBitmaps[i]->getBits());
		}
		else
		{
			heightBitmaps[i] = origHeightBitmaps[i];
		}

		// Create iterator
		bitmapItors[i] = new GLBitmapItterator(
			*heightBitmaps[i], 
			destBitmap.getWidth(), 
			destBitmap.getHeight(), 
			GLBitmapItterator::wrap);
	}
	// Add shore and slopt itterators
	bitmapItors[numberSources] = 
		new GLBitmapItterator(
			slopeBitmap, 
			destBitmap.getWidth(), 
			destBitmap.getHeight(), 
			GLBitmapItterator::wrap);
	bitmapItors[numberSources + 1] = 
		new GLBitmapItterator(
			shoreBitmap, 
			destBitmap.getWidth(), 
			destBitmap.getHeight(), 
			GLBitmapItterator::wrap);
	
	GLfloat hdx = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getWidth();
	GLfloat hdy = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getHeight();

	GLubyte *destBits = destBitmap.getBits();

	GLfloat hy = 0.0f;
	for (int by=0; by<destBitmap.getHeight(); by++, hy+=hdy)
	{
		if (counter) counter->setNewPercentage((100.0f * float (by)) / float(destBitmap.getHeight()));

		GLfloat hx = 0.0f;
		for (int bx=0; bx<destBitmap.getWidth(); bx++, destBits+=3, hx+=hdx)
		{
			static Vector normal;
			hMap.getInterpNormal(hx, hy, normal);
			float height = hMap.getInterpHeight(hx, hy);
			float offSetHeight = hMap.getInterpHeight((float)hMap.getWidth() - hx, (float)hMap.getWidth() - hy);
			height *= (1.0f - (noiseMax/2.0f)) + ((offSetHeight*noiseMax)/hMapMaxHeight);

			// Find the index of the current texture by deviding the height into strips
			float heightPer = (height / maxHeight) * (float) numberSources;
			int heightIndex = (int) heightPer;
			if (heightIndex >= numberSources) 
			{
				heightIndex = numberSources - 1;
			}

			// Check if we are in a blending transition phase
			float blendFirstAmount = 1.0f;
			float blendSecondAmount = 0.0f;
			if (heightIndex < numberSources - 1)
			{
				float remainderIndex = heightPer - heightIndex;
				if (remainderIndex > blendHeightFactor)
				{
					// We need to do some blending, figure how much
					remainderIndex -= blendHeightFactor;
					blendSecondAmount = remainderIndex / (1.0f - blendHeightFactor);
					blendFirstAmount = 1.0f - blendSecondAmount;
				}
			}

			// Check to see if we need to blend in the side texture
			float blendSideAmount = 0.0f;
			float blendShoreAmount = 0.0f;
			if (normal[2] < blendNormalSlopeStart)
			{
				if (normal[2] < blendNormalSlopeStart - blendNormalSlopeLength)
				{
					// Only use the side texture
					blendSideAmount = 1.0f;
					blendFirstAmount = 0.0f;
					blendSecondAmount = 0.0f;
				}
				else
				{
					// Blend in the side texture
					float remainderIndex = normal[2] - (blendNormalSlopeStart - blendNormalSlopeLength);
					remainderIndex /= blendNormalSlopeLength;
				
					blendSideAmount = (1.0f - remainderIndex);
					blendFirstAmount *= remainderIndex;
					blendSecondAmount *= remainderIndex;
				}
			} 
			else if (normal[2] > blendNormalShoreStart && 
				height > 3.5f && height < 5.5f)
			{
				if (normal[2] > blendNormalShoreStart + blendNormalShoreLength)
				{
					// Only use the side texture
					blendShoreAmount = 1.0f;
					blendFirstAmount = 0.0f;
					blendSecondAmount = 0.0f;
				}
				else
				{
					// Blend in the side texture
					float remainderIndex = normal[2] - blendNormalSlopeStart;
					remainderIndex /= blendNormalSlopeLength;
				
					blendShoreAmount = (1.0f - remainderIndex);
					blendFirstAmount *= remainderIndex;
					blendSecondAmount *= remainderIndex;
				}
			}

			// Add first height component
			GLubyte *sourceBits1 = bitmapItors[heightIndex]->getPos();
			destBits[0] = (GLubyte) ((float) sourceBits1[0] * blendFirstAmount);
			destBits[1] = (GLubyte) ((float) sourceBits1[1] * blendFirstAmount);
			destBits[2] = (GLubyte) ((float) sourceBits1[2] * blendFirstAmount);

			if (blendSecondAmount > 0.0f)
			{
				// Add second height component (if blending)
				GLubyte *sourceBits2 = bitmapItors[heightIndex + 1]->getPos();
				destBits[0] += (GLubyte) ((float) sourceBits2[0] * blendSecondAmount);
				destBits[1] += (GLubyte) ((float) sourceBits2[1] * blendSecondAmount);
				destBits[2] += (GLubyte) ((float) sourceBits2[2] * blendSecondAmount);
			}

			if (blendSideAmount > 0.0f)
			{
				// Add side component (if blending normals)
				GLubyte *sourceBits3 = bitmapItors[numberSources]->getPos();
				destBits[0] += (GLubyte) ((float) sourceBits3[0] * blendSideAmount);
				destBits[1] += (GLubyte) ((float) sourceBits3[1] * blendSideAmount);
				destBits[2] += (GLubyte) ((float) sourceBits3[2] * blendSideAmount);
			}

			if (blendShoreAmount > 0.0f)
			{
				// Add side component (if blending normals)
				GLubyte *sourceBits4 = bitmapItors[numberSources + 1]->getPos();
				destBits[0] += (GLubyte) ((float) sourceBits4[0] * blendShoreAmount);
				destBits[1] += (GLubyte) ((float) sourceBits4[1] * blendShoreAmount);
				destBits[2] += (GLubyte) ((float) sourceBits4[2] * blendShoreAmount);
			}

			for (i=0; i<numberSources+2; i++) bitmapItors[i]->incX();
		}

		for (i=0; i<numberSources+2; i++) bitmapItors[i]->incY();
	}

	// Cleanup iterator and extra bitmaps	
	for (i=0; i<numberSources+2; i++)
	{
		delete bitmapItors[i];
	}
	delete [] bitmapItors;
	for (i=0; i<numberSources; i++)
	{
		if (bitmapScale != 1.0f)
		{
			delete heightBitmaps[i];
		}
	}
	delete [] heightBitmaps;
}

void GLBitmapModifier::addScorchToBitmap(HeightMap &hMap,
										 Vector &sunPos,
										GLBitmap &destBitmap,
										GLBitmap &scorchBitmap,
										DeformLandscape::DeformPoints &map,
										int scorchX, int scorchY,
										int scorchW)
{
	GLBitmapItterator bitmapItor(scorchBitmap,
								destBitmap.getWidth(), 
								destBitmap.getHeight(), 
								GLBitmapItterator::wrap);

	GLfloat hdx = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getWidth();
	GLfloat hdy = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getHeight();

	GLubyte *destBits = destBitmap.getBits();

	GLfloat hy = 0.0f;
	for (int by=0; by<destBitmap.getHeight(); by++, hy+=hdy, bitmapItor.incY())
	{
		int mapY = ((int) hy) - scorchY;
		if ((mapY >= 0) && (mapY < scorchW))
		{
			GLfloat hx = 0.0f;
			for (int bx=0; bx<destBitmap.getWidth(); bx++, destBits+=3, hx+=hdx, bitmapItor.incX())
			{
				int mapX = ((int) hx) - scorchX;

				if ((mapX >= 0) && (mapX < scorchW))
				{
					// Add first height component
					if (map.map[mapX][mapY] > 0.0f)
					{
						// Calculate lighting
						float dz = hMap.getInterpHeight(hx, hy);
			
						Vector testPosition(hx, hy, dz);
						Vector testNormal;
						hMap.getInterpNormal(hx, hy, testNormal);
						Vector sunDirection = (sunPos - testPosition).Normalize();

						float diffuseLight = directLightConst * (testNormal.dotP(sunDirection));
						float ambientLight = ambientLightConst;
						float lightIntense = diffuseLight + ambientLight;
						if (lightIntense > 1.0f) lightIntense = 1.0f;
						else if (lightIntense < ambientLightConst) lightIntense = ambientLightConst;

						float mag = map.map[mapX][mapY];

						GLubyte *sourceBits = bitmapItor.getPos();
						destBits[0] = (GLubyte) ((float(sourceBits[0]) * (mag * lightIntense)) + 
							(float(destBits[0]) * (1.0f - mag)));
						destBits[1] = (GLubyte) ((float(sourceBits[1]) * (mag * lightIntense)) + 
							(float(destBits[1]) * (1.0f - mag)));
						destBits[2] = (GLubyte) ((float(sourceBits[2]) * (mag * lightIntense)) + 
							(float(destBits[2]) * (1.0f - mag)));
					}
				}
			}
		}
		else
		{
			destBits += destBitmap.getWidth() * 3;
		}
	}
}

void GLBitmapModifier::addWavesToBitmap(HeightMap &hMap,
										GLBitmap &destBitmap,
										float waterHeight,
										float offSet)
{
	GLubyte *destBits = destBitmap.getBits();
	for (int y=0; y<destBitmap.getHeight(); y++)
	{
		for (int x=0; x<destBitmap.getWidth(); x++)
		{
			destBits[0] = GLubyte(0);
			destBits[1] = GLubyte(0);
			destBits[2] = GLubyte(0);
			destBits[3] = GLubyte(0);

			int a = int((float(x) / float(destBitmap.getWidth()) * 384.0f) - 64.0f);
			int b = int((float(y) / float(destBitmap.getHeight()) * 384.0f) - 64.0f);

			if (a>0 && b>0 &&
				a<hMap.getWidth() && b<hMap.getWidth())
			{
				float height = hMap.getHeight(a, b);
				if ((height > waterHeight - offSet) && (height <= waterHeight + offSet + offSet))
				{
					if (height > waterHeight)
					{
						destBits[0] = GLubyte(255);
						destBits[1] = GLubyte(255);
						destBits[2] = GLubyte(255);
						destBits[3] = GLubyte(128);
					}
					else
					{
						destBits[0] = GLubyte(255);
						destBits[1] = GLubyte(255);
						destBits[2] = GLubyte(255);
						destBits[3] = GLubyte(128.0f * (waterHeight - height)/offSet);
					}
				}
			}

			destBits+=4;
		}
	}
}

void GLBitmapModifier::removeWaterFromBitmap(HeightMap &hMap,
							GLBitmap &srcBitmap,
							GLBitmap &destBitmap,
							float waterHeight)
{
	GLubyte *destBits = destBitmap.getBits();
	GLubyte *srcBits = srcBitmap.getBits();

	GLfloat hdx = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getWidth();
	GLfloat hdy = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getHeight();

	GLfloat hy = 0.0f;
	for (int y=0; y<srcBitmap.getHeight(); y++, hy+=hdy)
	{
		GLfloat hx = 0.0f;
		for (int x=0; x<srcBitmap.getWidth(); x++, hx+=hdx, destBits+=4, srcBits+=3)
		{
			float height = hMap.getInterpHeight(hx, hy);

			GLubyte alpha = 0;
			if (height > waterHeight - 0.3)
			{
				alpha = 128;
				if (height > waterHeight)
				{
					alpha = 255;
				}
			}

			destBits[0] = srcBits[0];
			destBits[1] = srcBits[1];
			destBits[2] = srcBits[2];
			destBits[3] = alpha;
		}
	}
}

void GLBitmapModifier::addWaterToBitmap(HeightMap &hMap,
										GLBitmap &destBitmap,
										GLBitmap &waterBitmap,
										float waterHeight)
{
	const float waterPercentage = 0.75f;
	const float oneMinusPercentage = 1.0f - waterPercentage;

	GLBitmapItterator bitmapItor(waterBitmap,
								destBitmap.getWidth(), 
								destBitmap.getHeight(), 
								GLBitmapItterator::wrap);

	GLfloat hdx = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getWidth();
	GLfloat hdy = (GLfloat) hMap.getWidth() / (GLfloat) destBitmap.getHeight();

	GLubyte *destBits = destBitmap.getBits();

	GLfloat hy = 0.0f;
	for (int by=0; by<destBitmap.getHeight(); by++, hy+=hdy, bitmapItor.incY())
	{
		GLfloat hx = 0.0f;
		for (int bx=0; bx<destBitmap.getWidth(); bx++, destBits+=3, hx+=hdx, bitmapItor.incX())
		{
			float height = hMap.getInterpHeight(hx, hy);

			if (height <= waterHeight)
			{
				if (height <= waterHeight - 0.3)
				{
					GLubyte *sourceBits = bitmapItor.getPos();

					destBits[0] = GLubyte(
						(waterPercentage * float(sourceBits[0])) + 
						(oneMinusPercentage * float(destBits[0])));
					destBits[1] = GLubyte(
						(waterPercentage * float(sourceBits[1])) + 
						(oneMinusPercentage * float(destBits[1])));
					destBits[2] = GLubyte(
						(waterPercentage * float(sourceBits[2])) + 
						(oneMinusPercentage * float(destBits[2])));
				}
				else
				{
					destBits[0] = 200;
					destBits[1] = 200;
					destBits[2] = 200;
				}
			}
		}
	}
}

void GLBitmapModifier::addBorderToBitmap(GLBitmap &destBitmap,
										int borderWidth,
										float colors[3])
{
	DIALOG_ASSERT(destBitmap.getComponents() == 3);

	for (int x=0; x<destBitmap.getWidth(); x++)
	{
		for (int i=0; i<borderWidth; i++)
		{
			GLubyte *destBits = &destBitmap.getBits()[(x + i * destBitmap.getWidth()) * 3];
			destBits[0] = GLubyte(colors[0] * 255.0f);
			destBits[1] = GLubyte(colors[1] * 255.0f);
			destBits[2] = GLubyte(colors[2] * 255.0f);

			destBits = &destBitmap.getBits()[(x + (destBitmap.getHeight() -1 - i) * destBitmap.getWidth()) * 3];
			destBits[0] = GLubyte(colors[0] * 255.0f);
			destBits[1] = GLubyte(colors[1] * 255.0f);
			destBits[2] = GLubyte(colors[2] * 255.0f);
		}
	}

	for (int y=0; y<destBitmap.getWidth(); y++)
	{
		for (int i=0; i<borderWidth; i++)
		{
			GLubyte *destBits = &destBitmap.getBits()[(i + y * destBitmap.getWidth()) * 3];
			destBits[0] = GLubyte(colors[0] * 255.0f);
			destBits[1] = GLubyte(colors[1] * 255.0f);
			destBits[2] = GLubyte(colors[2] * 255.0f);

			destBits = &destBitmap.getBits()[(destBitmap.getWidth() - 1 - i + y * destBitmap.getWidth()) * 3];
			destBits[0] = GLubyte(colors[0] * 255.0f);
			destBits[1] = GLubyte(colors[1] * 255.0f);
			destBits[2] = GLubyte(colors[2] * 255.0f);
		}
	}
}
