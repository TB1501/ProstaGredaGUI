#include "drawings.h"
#include <windows.h>
#include "statika.h"
#include <array>

using namespace statika;

// Function to handle drawing the beam, forces, and loads
void DrawBeamAndLoads(HDC hdc, RECT clientRect, const Beam& beam, const Force& force, const UniformLoad& uniformLoad, const Moment& moment) {
	int windowWidth = clientRect.right - clientRect.left;
	int windowHeight = clientRect.bottom - clientRect.top;

	//DRAWING THE BEAM

	// Set a margin so that the beam doesn't touch the window edges
	int margin = 50;

	// Adjust the length of the beam to fit within the window, with margins
	int drawableLength = windowWidth - 2 * margin;

	// Calculate the Y coordinate to draw the beam in the middle of the window
	int beamY = windowHeight / 2;

	// Draw the beam
	MoveToEx(hdc, margin, beamY, NULL); // Starting point
	LineTo(hdc, margin + drawableLength, beamY); // Ending point, scaled to window size


	//Drawing the beam supports

	//Triangle dimensions
	int triangleHeight = 20; // Adjust the height of the triangle
	int triangleBase = 20; // Adjust the base width of the triangle
	int endX = margin + drawableLength; // X coordinate for the end of the beam

	//Suport A
	std::array<POINT,3> triangleA;

	// Define the points for the triangle
	triangleA[0].x = margin; // Top of the triangle (aligned with beam start)
	triangleA[0].y = beamY;

	triangleA[1].x = margin - triangleBase / 2; // Bottom-left of the triangle
	triangleA[1].y = beamY + triangleHeight;

	triangleA[2].x = margin + triangleBase / 2; // Bottom-right of the triangle
	triangleA[2].y = beamY + triangleHeight;

	// Draw the triangle using the Polygon function
	Polygon(hdc, triangleA.data(), 3);

	// Draw text above the triangle A
	const wchar_t* supportText = L"A";
	int textWidth = 50;
	int textHeight = 20;

	// Calculate the position for the text
	int textX = margin - textWidth / 2;
	int textY = beamY; // Position it above the beam and triangle

	// Output the text
	TextOutW(hdc, textX, textY, supportText, wcslen(supportText));

	// Support B
	std::array<POINT,3> triangleB;

	// Define the points for the triangle
	triangleB[0].x = endX; // Top of the triangle (aligned with beam end)
	triangleB[0].y = beamY;

	triangleB[1].x = endX - triangleBase / 2; // Bottom-left of the triangle
	triangleB[1].y = beamY + triangleHeight;

	triangleB[2].x = endX + triangleBase / 2; // Bottom-right of the triangle
	triangleB[2].y = beamY + triangleHeight;

	// Draw the triangle using the Polygon function
	Polygon(hdc, triangleB.data(), 3);

	// Draw text above the triangle
	const wchar_t* supportTextB = L"B";
	int textWidthB = 50;
	int textHeightB = 20;

	// Calculate the position for the text
	int textXB = endX + textWidthB / 2;
	int textYB = beamY;

	// Output the text 
	TextOutW(hdc, textXB, textYB, supportTextB, wcslen(supportTextB));

	// Draw an underline beneath the second triangle
	MoveToEx(hdc, endX - triangleBase / 2, beamY + triangleHeight + 5, NULL); // Start point of the underline
	LineTo(hdc, endX + triangleBase / 2, beamY + triangleHeight + 5);          // End point of the underline


	// Drawing the beam dimension line

	// Draw a line beneath the beam
	int lineOffset = 100; // Distance below the beam where the line will be drawn
	MoveToEx(hdc, margin - 5, beamY + lineOffset, NULL); // Start of the line
	LineTo(hdc, margin + drawableLength + 5, beamY + lineOffset); // End of the line

	// Draw a vertical line at the start of the horizontal line (5 pixels down)
	MoveToEx(hdc, margin, beamY + lineOffset, NULL); // Start at the beginning of the line
	LineTo(hdc, margin, beamY + lineOffset + 5); // Draw downwards by 5 pixels

	// Draw a vertical line at the start of the horizontal line (5 pixels down)
	MoveToEx(hdc, margin, beamY + lineOffset, NULL); // Start at the beginning of the line
	LineTo(hdc, margin, beamY + lineOffset - 5); // Draw upwards by 5 pixels

	// Draw a vertical line at the end of the horizontal line (5 pixels down)
	MoveToEx(hdc, margin + drawableLength, beamY + lineOffset, NULL); // Start at the end of the line
	LineTo(hdc, margin + drawableLength, beamY + lineOffset + 5); // Draw downwards by 5 pixels

	// Draw a vertical line at the end of the horizontal line (5 pixels down)
	MoveToEx(hdc, margin + drawableLength, beamY + lineOffset, NULL); // Start at the end of the line
	LineTo(hdc, margin + drawableLength, beamY + lineOffset - 5); // Draw upwards by 5 pixels

	// Prepare a buffer to store the beam length as a wide string
	wchar_t beamLengthText[50];
	std::swprintf(beamLengthText, 50, L"%.2f m", beam.L);

	// Draw the beam length text in the middle of the line
	int textWidthLength = 50;  // Adjust this if needed based on the text length
	int textHeightLength = 20;  // Height of the text

	// Calculate the position for the text to be centered on the line
	int textXLength = margin + (drawableLength / 2) - (textWidthLength / 2);
	int textYLength = beamY + lineOffset - textHeightLength; // Position the text above the line

	// Output the beam length text
	TextOutW(hdc, textXLength, textYLength, beamLengthText, wcslen(beamLengthText));

	/*___________________________________________________________________________________________________________________________*/

	//DRAWING THE FORCE

	//Vertical force

	// Calculate the vertical force position on the beam

	int forceYPosition = margin + drawableLength * force.Fy_x / beam.L;

	//Arrow
	std::array<POINT,3> arrowForceY;

	//Arrow dimensions
	int arrowForceHeight = 15; // Adjust the height of the triangle
	int arrowForceBase = 15; // Adjust the base width of the triangle

	// Define the points for the triangle
	if (force.Fy != 0) {
		if (force.Fy > 0) {
			arrowForceY[0].x = forceYPosition; // Top of the triangle (aligned with beam start)
			arrowForceY[0].y = beamY;

			arrowForceY[1].x = forceYPosition - arrowForceBase / 2; // Bottom-left point
			arrowForceY[1].y = beamY - arrowForceHeight;

			arrowForceY[2].x = forceYPosition + arrowForceBase / 2; // Bottom-right point
			arrowForceY[2].y = beamY - arrowForceHeight;

			// Draw line from arrow
			MoveToEx(hdc, forceYPosition, beamY - arrowForceHeight, NULL); // Starting point
			LineTo(hdc, forceYPosition, beamY - 45); // Ending point, scaled to window size

			//Draw text
			int lineOffsetForceY = 40;  // Distance from the force where the text will be drawn
			// Prepare a buffer to store the text of force
			wchar_t forceYText[50];
			std::swprintf(forceYText, 50, L"%.2f kN'", force.Fy);

			// Draw the force text 
			int textWidthForceYLength = 50;  // Adjust this if needed based on the text length
			int textHeightForceYLength = 20;  // Height of the text

			// Calculate the position for the text
			int textForceYXLength = forceYPosition - textWidthForceYLength / 2;
			int textForceYYLength = beamY - lineOffsetForceY - textHeightForceYLength; // Position the text above 

			// Output the force text
			TextOutW(hdc, textForceYXLength, textForceYYLength, forceYText, wcslen(forceYText));
		}
		else {
			arrowForceY[0].x = forceYPosition; // Top of the triangle (aligned with beam start)
			arrowForceY[0].y = beamY;

			arrowForceY[1].x = forceYPosition - arrowForceBase / 2; // Bottom-left point
			arrowForceY[1].y = beamY + arrowForceHeight;

			arrowForceY[2].x = forceYPosition + arrowForceBase / 2; // Bottom-right point
			arrowForceY[2].y = beamY + arrowForceHeight;

			// Draw line from arrow
			MoveToEx(hdc, forceYPosition, beamY + arrowForceHeight, NULL); // Starting point
			LineTo(hdc, forceYPosition, beamY + 45); // Ending point, scaled to window size

			//Draw text
			int lineOffsetForceY = 40;  // Distance from the force where the text will be drawn
			// Prepare a buffer to store the text of force
			wchar_t forceYText[50];
			std::swprintf(forceYText, 50, L"%.2f kN'", force.Fy);

			// Draw the force text 
			int textWidthForceYLength = 50;  // Adjust this if needed based on the text length
			int textHeightForceYLength = 20;  // Height of the text

			// Calculate the position for the text
			int textForceYXLength = forceYPosition - textWidthForceYLength / 2;
			int textForceYYLength = beamY + lineOffsetForceY; // Position the text above 

			// Output the force text
			TextOutW(hdc, textForceYXLength, textForceYYLength, forceYText, wcslen(forceYText));
		}

		// Draw the arrow
		Polygon(hdc, arrowForceY.data(), 3);

		//Draw the dimension line for force
		// Draw a line beneath the beam
		int lineOffsetFy_x = -100; // Distance below the beam where the line will be drawn
		MoveToEx(hdc, margin - 5, beamY + lineOffsetFy_x, NULL); // Start of the line
		LineTo(hdc, forceYPosition + 5, beamY + lineOffsetFy_x); // End of the line

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetFy_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetFy_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetFy_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetFy_x - 5); // Draw upwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, forceYPosition, beamY + lineOffsetFy_x, NULL); // Start at the end of the line
		LineTo(hdc, forceYPosition, beamY + lineOffsetFy_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, forceYPosition, beamY + lineOffsetFy_x, NULL); // Start at the end of the line
		LineTo(hdc, forceYPosition, beamY + lineOffsetFy_x + -5); // Draw upwards by 5 pixels

		// Prepare a buffer to store the force dimension line as a wide string
		wchar_t forceFy_xText[50];
		std::swprintf(forceFy_xText, 50, L"%.2f m", force.Fy_x);

		// Draw the beam length text in the middle of the line
		int textWidthLengthFy_x = 10;  // Adjust this if needed based on the text length
		int textHeightLengthFy_x = 20;  // Height of the text

		// Calculate the position for the text to be centered on the line
		int textXLengthFy_x = forceYPosition / 2 - textWidthLengthFy_x / 2;
		int textYLengthFy_x = beamY + lineOffsetFy_x - textHeightLengthFy_x; // Position the text above the line

		// Output the beam length text
		TextOutW(hdc, textXLengthFy_x, textYLengthFy_x, forceFy_xText, wcslen(forceFy_xText));

	}

	//Horizontal force

	// Calculate the horizontal force position on the beam
	int forceXPosition = margin + drawableLength * force.Fx_x / beam.L;

	// Arrow
	std::array<POINT,3> arrowForceX;

	// Arrow dimensions
	int arrowForceXHeight = 15; // Adjust the height of the triangle
	int arrowForceXBase = 15;   // Adjust the base width of the triangle

	// Define the points for the triangle (horizontal arrow)
	if (force.Fx != 0) {
		if (force.Fx > 0) {
			// Positive horizontal force (rightward arrow)
			arrowForceX[0].x = forceXPosition;              // Tip of the arrow
			arrowForceX[0].y = beamY;                       // Aligned with the beam

			arrowForceX[1].x = forceXPosition - arrowForceXBase; // Left point
			arrowForceX[1].y = beamY - arrowForceXHeight / 2;

			arrowForceX[2].x = forceXPosition - arrowForceXBase; // Left point (bottom)
			arrowForceX[2].y = beamY + arrowForceXHeight / 2;

			// Draw line for the horizontal force
			MoveToEx(hdc, forceXPosition - arrowForceXBase, beamY, NULL);
			LineTo(hdc, forceXPosition - 50, beamY);  // Horizontal line towards left
		}
		else {
			// Negative horizontal force (leftward arrow)
			arrowForceX[0].x = forceXPosition;              // Tip of the arrow
			arrowForceX[0].y = beamY;                       // Aligned with the beam

			arrowForceX[1].x = forceXPosition + arrowForceXBase; // Right point
			arrowForceX[1].y = beamY - arrowForceXHeight / 2;

			arrowForceX[2].x = forceXPosition + arrowForceXBase; // Right point (bottom)
			arrowForceX[2].y = beamY + arrowForceXHeight / 2;

			// Draw line for the horizontal force
			MoveToEx(hdc, forceXPosition + arrowForceXBase, beamY, NULL);
			LineTo(hdc, forceXPosition + 50, beamY);  // Horizontal line towards right
		}

		// Draw the horizontal force arrow
		Polygon(hdc, arrowForceX.data(), 3);

		int lineOffsetForceX = 5;  // Distance from the force where the text will be drawn
		// Prepare a buffer to store the text of force
		wchar_t forceXText[50];
		std::swprintf(forceXText, 50, L"%.2f kN'", force.Fx);

		// Draw the force text 
		int textWidthForceXLength = 50;  // Adjust this if needed based on the text length
		int textHeightForceXLength = 20;  // Height of the text

		// Calculate the position for the text
		int textForceXXLength = forceXPosition - textWidthForceXLength / 2;
		int textForceXYLength = beamY - lineOffsetForceX - textHeightForceXLength; // Position the text above 

		// Output the force text
		TextOutW(hdc, textForceXXLength, textForceXYLength, forceXText, wcslen(forceXText));


		//Draw the dimension line for force
		// Draw a line beneath the beam
		int lineOffsetFx_x = -75; // Distance below the beam where the line will be drawn
		MoveToEx(hdc, margin - 5, beamY + lineOffsetFx_x, NULL); // Start of the line
		LineTo(hdc, forceXPosition + 5, beamY + lineOffsetFx_x); // End of the line

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetFx_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetFx_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetFx_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetFx_x - 5); // Draw upwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, forceXPosition, beamY + lineOffsetFx_x, NULL); // Start at the end of the line
		LineTo(hdc, forceXPosition, beamY + lineOffsetFx_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, forceXPosition, beamY + lineOffsetFx_x, NULL); // Start at the end of the line
		LineTo(hdc, forceXPosition, beamY + lineOffsetFx_x + -5); // Draw upwards by 5 pixels

		// Prepare a buffer to store the force dimension line as a wide string
		wchar_t forceFx_xText[50];
		std::swprintf(forceFx_xText, 50, L"%.2f m", force.Fx_x);

		// Draw the beam length text in the middle of the line
		int textWidthLengthFx_x = 10;  // Adjust this if needed based on the text length
		int textHeightLengthFx_x = 20;  // Height of the text

		// Calculate the position for the text to be centered on the line
		int textXLengthFx_x = forceXPosition / 2 - textWidthLengthFx_x / 2;
		int textYLengthFx_x = beamY + lineOffsetFx_x - textHeightLengthFx_x; // Position the text above the line

		// Output the beam length text
		TextOutW(hdc, textXLengthFx_x, textYLengthFx_x, forceFx_xText, wcslen(forceFx_xText));
	}


	/*___________________________________________________________________________________________________________________________*/

	//DRAWING THE MOMENT

	// Calculate the moment position on the beam
	int momentPosition = margin + drawableLength * moment.x / beam.L;

	// Draw the moment 
	// Set ellipse dimensions (moment symbol)
	int radiusWidth = 10;
	int radiusHeight = 30;


	// Calculate the arrow position depending on the moment direction (clockwise or counterclockwise)
	std::array<POINT,3> arrowMoment; // Points to form the arrowhead triangle

	int arrowMomentSize = 10;

	if (moment.M != 0)
	{
		if (moment.M < 0) {

			// Draw upper half of the ellipse (arc)
			Arc(hdc, momentPosition - radiusWidth * 2, beamY - radiusHeight, momentPosition + radiusWidth * 2, beamY + radiusHeight,
				momentPosition + radiusWidth, beamY - radiusHeight / 1.4, momentPosition - radiusWidth, beamY + radiusHeight / 1.4);

			// Counter Clockwise arrow at the top-right of the ellipse
			arrowMoment[0].x = momentPosition + 2 * arrowMomentSize;
			arrowMoment[0].y = beamY - radiusHeight;

			arrowMoment[1].x = momentPosition - arrowMomentSize / 4;
			arrowMoment[1].y = beamY - radiusHeight - arrowMomentSize;

			arrowMoment[2].x = momentPosition - arrowMomentSize / 4;
			arrowMoment[2].y = beamY - radiusHeight + arrowMomentSize;

		}
		else {
			// Counterclockwise arrow at the top-left of the ellipse

			  // Draw lower half of the ellipse (arc)
			Arc(hdc, momentPosition - radiusWidth * 2, beamY - radiusHeight, momentPosition + radiusWidth * 2, beamY + radiusHeight,
				momentPosition + radiusWidth, beamY + radiusHeight / 1.4, momentPosition - radiusWidth, beamY - radiusHeight);

			// Counterclockwise arrow at the top-left of the ellipse
			arrowMoment[0].x = momentPosition - 2 * arrowMomentSize;  // Tip of the arrowhead (left side)
			arrowMoment[0].y = beamY - radiusHeight;

			arrowMoment[1].x = momentPosition + arrowMomentSize / 4;  // Bottom-left of the arrowhead
			arrowMoment[1].y = beamY - radiusHeight - arrowMomentSize;

			arrowMoment[2].x = momentPosition + arrowMomentSize / 4;  // Bottom-right of the arrowhead
			arrowMoment[2].y = beamY - radiusHeight + arrowMomentSize;

		}

		// Draw the arrowhead (triangle)
		Polygon(hdc, arrowMoment.data(), 3);

		int lineOffsetMoment = 40;  // Distance from the moment where the text will be drawn
		// Prepare a buffer to store the text of moment
		wchar_t momentText[50];
		std::swprintf(momentText, 50, L"%.2f kNm", moment.M);

		// Draw the moment text 
		int textWidthMomentLength = 50;  // Adjust this if needed based on the text length
		int textHeightMomentLength = 20;  // Height of the text

		// Calculate the position for the text
		int textMomentXLength = momentPosition - textWidthMomentLength / 2;
		int textMomentYLength = beamY - lineOffsetMoment - textHeightMomentLength; // Position the text above moment

		// Output the moment text
		TextOutW(hdc, textMomentXLength, textMomentYLength, momentText, wcslen(momentText));


		//Draw the dimension line for moment
		// Draw a line beneath the beam
		int lineOffsetM_x = -125; // Distance below the beam where the line will be drawn
		MoveToEx(hdc, margin - 5, beamY + lineOffsetM_x, NULL); // Start of the line
		LineTo(hdc, momentPosition + 5, beamY + lineOffsetM_x); // End of the line

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetM_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetM_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetM_x, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetM_x - 5); // Draw upwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, momentPosition, beamY + lineOffsetM_x, NULL); // Start at the end of the line
		LineTo(hdc, momentPosition, beamY + lineOffsetM_x + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, momentPosition, beamY + lineOffsetM_x, NULL); // Start at the end of the line
		LineTo(hdc, momentPosition, beamY + lineOffsetM_x + -5); // Draw upwards by 5 pixels

		// Prepare a buffer to store the force dimension line as a wide string
		wchar_t M_xText[50];
		std::swprintf(M_xText, 50, L"%.2f m", moment.x);

		// Draw the beam length text in the middle of the line
		int textWidthLengthM_x = 10;  // Adjust this if needed based on the text length
		int textHeightLengthM_x = 20;  // Height of the text

		// Calculate the position for the text to be centered on the line
		int textXLengthM_x = momentPosition / 2 - textWidthLengthM_x / 2;
		int textYLengthM_x = beamY + lineOffsetM_x - textHeightLengthM_x; // Position the text above the line

		// Output the beam length text
		TextOutW(hdc, textXLengthM_x, textYLengthM_x, M_xText, wcslen(M_xText));

	}

	/*___________________________________________________________________________________________________________________________*/

	//DRAWING THE UNIFORM LOAD

	// Calculate the uniform load positions on the beam
	int uniformLoadX1 = margin + drawableLength * uniformLoad.x1 / beam.L;
	int uniformLoadX2 = margin + drawableLength * uniformLoad.x2 / beam.L;
	int uniformLoadSection = (uniformLoadX2 - uniformLoadX1) / 10;

	// Draw the uniform load

	//Arrow
	std::array<POINT,3> arrowUniformLoad;

	//Arrow dimensions
	int arrowULHeight = 5; // Adjust the height of the triangle
	int arrowULBase = 5; // Adjust the base width of the triangle

	// Define the points for the triangle

	if (uniformLoad.q != 0) {
		int counter = uniformLoadX1;  // Start from the first position of the uniform load
		for (int i = 0; i <= 10; i++) {  // Draw 10 arrows along the uniform load section
			if (uniformLoad.q > 0) {
				// Define the upward-facing triangle
				arrowUniformLoad[0].x = counter;  // Top of the triangle (aligned with beam start)
				arrowUniformLoad[0].y = beamY;

				arrowUniformLoad[1].x = counter - arrowULBase / 2;  // Bottom-left point
				arrowUniformLoad[1].y = beamY - arrowULHeight;

				arrowUniformLoad[2].x = counter + arrowULBase / 2;  // Bottom-right point
				arrowUniformLoad[2].y = beamY - arrowULHeight;

				// Draw line from arrow
				MoveToEx(hdc, counter, beamY - arrowULHeight, NULL);  // Starting point
				LineTo(hdc, counter, beamY - 25);  // Ending point (scaled)
			}
			else {
				// Define the downward-facing triangle
				arrowUniformLoad[0].x = counter;  // Top of the triangle (aligned with beam start)
				arrowUniformLoad[0].y = beamY;

				arrowUniformLoad[1].x = counter - arrowULBase / 2;  // Bottom-left point
				arrowUniformLoad[1].y = beamY + arrowULHeight;

				arrowUniformLoad[2].x = counter + arrowULBase / 2;  // Bottom-right point
				arrowUniformLoad[2].y = beamY + arrowULHeight;

				// Draw line from arrow
				MoveToEx(hdc, counter, beamY + arrowULHeight, NULL);  // Starting point
				LineTo(hdc, counter, beamY + 25);  // Ending point (scaled)
			}

			// Draw the arrow (triangle)
			Polygon(hdc, arrowUniformLoad.data(), 3);

			// Move to the next section
			counter += uniformLoadSection;
		}

	}

	if (uniformLoad.q != 0)
	{
		if (uniformLoad.q > 0)
		{
			MoveToEx(hdc, uniformLoadX1, beamY - 25, NULL);  // Starting point
			LineTo(hdc, uniformLoadX2, beamY - 25);  // Ending point (scaled)

			// Draw the uniform load text
			int lineOffsetUL = 25;  // Distance from the moment where the text will be drawn
			// Prepare a buffer to store the text of uniform load
			wchar_t uniformLoadText[50];
			std::swprintf(uniformLoadText, 50, L"%.2f kN/m'", uniformLoad.q);

			// Draw the uniform load text
			int textWidthULLength = 50;  // Adjust this if needed based on the text length
			int textHeightULLength = 20;  // Height of the text

			// Calculate the position for the text
			int textULXLength = uniformLoadX1 - (textWidthULLength / 2);
			int textULYLength = beamY - lineOffsetUL - textHeightULLength; // Position the text above the line

			// Output the uniform load text
			TextOutW(hdc, textULXLength, textULYLength, uniformLoadText, wcslen(uniformLoadText));

		}
		else
		{
			MoveToEx(hdc, uniformLoadX1, beamY + 25, NULL);  // Starting point
			LineTo(hdc, uniformLoadX2, beamY + 25);  // Ending point (scaled)

			//Draw the uniform load text
			int lineOffsetUL = 30;  // Distance from the moment where the text will be drawn
			// Prepare a buffer to store the text of uniform load
			wchar_t uniformLoadText[50];
			std::swprintf(uniformLoadText, 50, L"%.2f kN/m'", uniformLoad.q);

			// Draw the uniform load text
			int textWidthULLength = 50;  // Adjust this if needed based on the text length
			int textHeightULLength = 20;  // Height of the text

			// Calculate the position for the text
			int textULXLength = uniformLoadX1 - (textWidthULLength / 2);
			int textULYLength = beamY + lineOffsetUL; // Position the text above the line

			// Output the uniform load text
			TextOutW(hdc, textULXLength, textULYLength, uniformLoadText, wcslen(uniformLoadText));
		}

		//Draw the dimension line for moment
		// Draw a line beneath the beam
		int lineOffsetUL_y = -150; // Distance below the beam where the line will be drawn
		MoveToEx(hdc, margin - 5, beamY + lineOffsetUL_y, NULL); // Start of the line
		LineTo(hdc, uniformLoadX1 + 5, beamY + lineOffsetUL_y); // End of the line
		MoveToEx(hdc, uniformLoadX1, beamY + lineOffsetUL_y, NULL); // Start of the line
		LineTo(hdc, uniformLoadX2 + 5, beamY + lineOffsetUL_y); // End of the line

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetUL_y, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetUL_y + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the start of the horizontal line (5 pixels down)
		MoveToEx(hdc, margin, beamY + lineOffsetUL_y, NULL); // Start at the beginning of the line
		LineTo(hdc, margin, beamY + lineOffsetUL_y - 5); // Draw upwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, uniformLoadX1, beamY + lineOffsetUL_y, NULL); // Start at the end of the line
		LineTo(hdc, uniformLoadX1, beamY + lineOffsetUL_y + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, uniformLoadX1, beamY + lineOffsetUL_y, NULL); // Start at the end of the line
		LineTo(hdc, uniformLoadX1, beamY + lineOffsetUL_y + -5); // Draw upwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line(5 pixels down)
		MoveToEx(hdc, uniformLoadX2, beamY + lineOffsetUL_y, NULL); // Start at the end of the line
		LineTo(hdc, uniformLoadX2, beamY + lineOffsetUL_y + 5); // Draw downwards by 5 pixels

		// Draw a vertical line at the end of the horizontal line (5 pixels down)
		MoveToEx(hdc, uniformLoadX2, beamY + lineOffsetUL_y, NULL); // Start at the end of the line
		LineTo(hdc, uniformLoadX2, beamY + lineOffsetUL_y + -5); // Draw upwards by 5 pixels

		// Prepare a buffer to store the force dimension line as a wide string
		wchar_t UL_x1Text[50];
		std::swprintf(UL_x1Text, 50, L"%.2f m", uniformLoad.x1);
		wchar_t UL_x2Text[50];
		std::swprintf(UL_x2Text, 50, L"%.2f m", uniformLoad.x2 - uniformLoad.x1);

		// Draw the text in the middle of the line
		int textWidthLengthUL = 10;  // Adjust this if needed based on the text length
		int textHeightLengthUL = 20;  // Height of the text

		// Calculate the position for the text to be centered on the line
		int textXLengthUL_x1 = uniformLoadX1 / 2 - textWidthLengthUL / 2;
		int textYLengthUL_x1 = beamY + lineOffsetUL_y - textHeightLengthUL; // Position the text above the line
		int textXLengthUL_x2 = uniformLoadX1 / 2 + uniformLoadX2 / 2 - textWidthLengthUL / 2;
		int textYLengthUL_x2 = beamY + lineOffsetUL_y - textHeightLengthUL; // Position the text above the line

		// Output the beam length text
		TextOutW(hdc, textXLengthUL_x1, textYLengthUL_x1, UL_x1Text, wcslen(UL_x1Text));
		TextOutW(hdc, textXLengthUL_x2, textYLengthUL_x2, UL_x2Text, wcslen(UL_x2Text));

	}
}

// Function to handle drawing the internal moments
void DrawInternalMoments(HDC hdc, RECT clientRect, const Beam& beam, StaticEquilibrium& eq) {

	int windowWidth = clientRect.right - clientRect.left;
	int windowHeight = clientRect.bottom - clientRect.top;

	// Create a clipping region that matches the client area of the child window
	HRGN hRgn = CreateRectRgn(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
	SelectClipRgn(hdc, hRgn);  // Set the clipping region to the device context (HDC)


	//DRAWING THE BEAM

		// Set a margin so that the beam doesn't touch the window edges
	int margin = 50;

	// Adjust the length of the beam to fit within the window, with margins
	int drawableLength = windowWidth - 2 * margin;

	// Adjust the length to fit within the window, with margins
	int drawableHeight = windowHeight - 2 * margin;

	// Calculate the Y coordinate to draw the beam in the middle of the window
	int beamY = windowHeight / 2;

	// Draw the beam
	MoveToEx(hdc, margin, beamY, NULL); // Starting point
	LineTo(hdc, margin + drawableLength, beamY); // Ending point, scaled to window size


	//Drawing the beam supports

	//Triangle dimensions
	int triangleHeight = 20; // Adjust the height of the triangle
	int triangleBase = 20; // Adjust the base width of the triangle
	int endX = margin + drawableLength; // X coordinate for the end of the beam

	//Suport A
	std::array<POINT,3> triangleA;

	// Define the points for the triangle
	triangleA[0].x = margin; // Top of the triangle (aligned with beam start)
	triangleA[0].y = beamY;

	triangleA[1].x = margin - triangleBase / 2; // Bottom-left of the triangle
	triangleA[1].y = beamY + triangleHeight;

	triangleA[2].x = margin + triangleBase / 2; // Bottom-right of the triangle
	triangleA[2].y = beamY + triangleHeight;

	// Draw the triangle using the Polygon function
	Polygon(hdc, triangleA.data(), 3);

	// Draw text above the triangle A
	const wchar_t* supportText = L"A";
	int textWidth = 50;
	int textHeight = 20;

	// Calculate the position for the text
	int textX = margin - textWidth / 2;
	int textY = beamY; // Position it above the beam and triangle

	// Output the text
	TextOutW(hdc, textX, textY, supportText, wcslen(supportText));

	// Support B
	std::array<POINT,3> triangleB;

	// Define the points for the triangle
	triangleB[0].x = endX; // Top of the triangle (aligned with beam end)
	triangleB[0].y = beamY;

	triangleB[1].x = endX - triangleBase / 2; // Bottom-left of the triangle
	triangleB[1].y = beamY + triangleHeight;

	triangleB[2].x = endX + triangleBase / 2; // Bottom-right of the triangle
	triangleB[2].y = beamY + triangleHeight;

	// Draw the triangle using the Polygon function
	Polygon(hdc, triangleB.data(), 3);

	// Draw text above the triangle
	const wchar_t* supportTextB = L"B";
	int textWidthB = 50;
	int textHeightB = 20;

	// Calculate the position for the text
	int textXB = endX + textWidthB / 2;
	int textYB = beamY;

	// Output the text 
	TextOutW(hdc, textXB, textYB, supportTextB, wcslen(supportTextB));

	// Draw an underline beneath the second triangle
	MoveToEx(hdc, endX - triangleBase / 2, beamY + triangleHeight + 5, NULL); // Start point of the underline
	LineTo(hdc, endX + triangleBase / 2, beamY + triangleHeight + 5);          // End point of the underline

	/*____________________________________________________________________________________________________________________________*/


	// DRAWING THE INNER MOMENTS ON BEAM
	const auto& innerMoments = eq.getInternalMoments(); // Get the list of internal moments
	double maxMoment = eq.getMaxMomentValue(innerMoments); // Maximum moment value



	if (!innerMoments.empty())
	{

		// Prevent division by zero for very small moments
		if (maxMoment == 0) maxMoment = 1;

		// Limit the maximum scaling factor so that large moments don’t exceed drawable height
		double scaleFactor = (drawableHeight / 2) / maxMoment;

		// Iterate through the moments and draw them
		for (size_t i = 0; i < innerMoments.size(); ++i)
		{
			double momentValue = innerMoments[i].first;
			double positionAlongBeam = innerMoments[i].second;

			// Calculate the X and Y coordinates for this moment point
			int momentX = margin + static_cast<int>((positionAlongBeam / beam.L) * drawableLength);

			// Clip the moment height so that it fits within the drawable area
			int momentY = beamY + static_cast<int>(momentValue * scaleFactor);
			//momentY = max(beamY - drawableHeight, min(beamY, momentY)); // Ensure Y is within bounds

			// Draw a line from the beam to represent the moment
			MoveToEx(hdc, momentX, beamY, NULL); // Start at the beam
			LineTo(hdc, momentX, momentY); // Draw the line to the moment height

			// If it's not the first moment, connect the previous moment to this one with a line
			if (i > 0)
			{
				int prevMomentX = margin + static_cast<int>((innerMoments[i - 1].second / beam.L) * drawableLength);
				int prevMomentY = beamY + static_cast<int>(innerMoments[i - 1].first * scaleFactor);
				//prevMomentY = max(beamY - drawableHeight, min(beamY, prevMomentY)); // Clip Y to fit in the window

				MoveToEx(hdc, prevMomentX, prevMomentY, NULL); // Move to previous moment
				LineTo(hdc, momentX, momentY); // Connect the two moment points
			}
		}
	}

	// Release the clipping region after the drawing is complete
	SelectClipRgn(hdc, NULL); // Remove the clipping region
	DeleteObject(hRgn); // Clean up the region object
}
