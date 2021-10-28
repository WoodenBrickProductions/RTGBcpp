#include "raylib.h"


// void DrawBillboardProWithUp(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint)
// {
//     // NOTE: Billboard size will maintain source rectangle aspect ratio, size will represent billboard width
//     Vector2 sizeRatio = { size.y, size.x*(float)source.height/source.width };

//     Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);

//     Vector3 right = { matView.m0, matView.m4, matView.m8 };
//     //Vector3 up = { matView.m1, matView.m5, matView.m9 };

//     Vector3 rightScaled = Vector3Scale(right, sizeRatio.x/2);
//     Vector3 upScaled = Vector3Scale(up, sizeRatio.y/2);

//     Vector3 p1 = Vector3Add(rightScaled, upScaled);
//     Vector3 p2 = Vector3Subtract(rightScaled, upScaled);

//     Vector3 topLeft = Vector3Scale(p2, -1);
//     Vector3 topRight = p1;
//     Vector3 bottomRight = p2;
//     Vector3 bottomLeft = Vector3Scale(p1, -1);

//     if (rotation != 0.0f)
//     {
//         float sinRotation = sinf(rotation*DEG2RAD);
//         float cosRotation = cosf(rotation*DEG2RAD);

//         // NOTE: (-1, 1) is the range where origin.x, origin.y is inside the texture
//         float rotateAboutX = sizeRatio.x*origin.x/2;
//         float rotateAboutY = sizeRatio.y*origin.y/2;

//         float xtvalue, ytvalue;
//         float rotatedX, rotatedY;

//         xtvalue = Vector3DotProduct(right, topLeft) - rotateAboutX; // Project points to x and y coordinates on the billboard plane
//         ytvalue = Vector3DotProduct(up, topLeft) - rotateAboutY;
//         rotatedX = xtvalue*cosRotation - ytvalue*sinRotation + rotateAboutX; // Rotate about the point origin
//         rotatedY = xtvalue*sinRotation + ytvalue*cosRotation + rotateAboutY;
//         topLeft = Vector3Add(Vector3Scale(up, rotatedY), Vector3Scale(right, rotatedX)); // Translate back to cartesian coordinates

//         xtvalue = Vector3DotProduct(right, topRight) - rotateAboutX;
//         ytvalue = Vector3DotProduct(up, topRight) - rotateAboutY;
//         rotatedX = xtvalue*cosRotation - ytvalue*sinRotation + rotateAboutX;
//         rotatedY = xtvalue*sinRotation + ytvalue*cosRotation + rotateAboutY;
//         topRight = Vector3Add(Vector3Scale(up, rotatedY), Vector3Scale(right, rotatedX));

//         xtvalue = Vector3DotProduct(right, bottomRight) - rotateAboutX;
//         ytvalue = Vector3DotProduct(up, bottomRight) - rotateAboutY;
//         rotatedX = xtvalue*cosRotation - ytvalue*sinRotation + rotateAboutX;
//         rotatedY = xtvalue*sinRotation + ytvalue*cosRotation + rotateAboutY;
//         bottomRight = Vector3Add(Vector3Scale(up, rotatedY), Vector3Scale(right, rotatedX));

//         xtvalue = Vector3DotProduct(right, bottomLeft)-rotateAboutX;
//         ytvalue = Vector3DotProduct(up, bottomLeft)-rotateAboutY;
//         rotatedX = xtvalue*cosRotation - ytvalue*sinRotation + rotateAboutX;
//         rotatedY = xtvalue*sinRotation + ytvalue*cosRotation + rotateAboutY;
//         bottomLeft = Vector3Add(Vector3Scale(up, rotatedY), Vector3Scale(right, rotatedX));
//     }

//     // Translate points to the draw center (position)
//     topLeft = Vector3Add(topLeft, position);
//     topRight = Vector3Add(topRight, position);
//     bottomRight = Vector3Add(bottomRight, position);
//     bottomLeft = Vector3Add(bottomLeft, position);

//     rlCheckRenderBatchLimit(4);

//     rlSetTexture(texture.id);

//     rlBegin(RL_QUADS);
//         rlColor4ub(tint.r, tint.g, tint.b, tint.a);

//         // Bottom-left corner for texture and quad
//         rlTexCoord2f((float)source.x/texture.width, (float)source.y/texture.height);
//         rlVertex3f(topLeft.x, topLeft.y, topLeft.z);

//         // Top-left corner for texture and quad
//         rlTexCoord2f((float)source.x/texture.width, (float)(source.y + source.height)/texture.height);
//         rlVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);

//         // Top-right corner for texture and quad
//         rlTexCoord2f((float)(source.x + source.width)/texture.width, (float)(source.y + source.height)/texture.height);
//         rlVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);

//         // Bottom-right corner for texture and quad
//         rlTexCoord2f((float)(source.x + source.width)/texture.width, (float)source.y/texture.height);
//         rlVertex3f(topRight.x, topRight.y, topRight.z);
//     rlEnd();

//     rlSetTexture(0);
// }