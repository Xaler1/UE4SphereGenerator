// Copyright Epic Games, Inc. All Rights Reserved.

#include "SphereGeneratorBPLibrary.h"
#include "SphereGenerator.h"

USphereGeneratorBPLibrary::USphereGeneratorBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FVector RThetaPhi_to_XYZ(float R, float Theta, float Phi);

void USphereGeneratorBPLibrary::getRadialSphere(const float radius, const float divisions, TArray<FVector>& Verticies, TArray<int32>& Triangles) {
	for (float theta = PI / (divisions + 1), a = 0; theta < PI && a < divisions; theta += PI / (divisions + 1), a++) {
		for (float phi = 0, b = 0; phi < 2 * PI && b <= divisions; phi += 2 * PI / (divisions + 1), b++) {
			Verticies.Add(RThetaPhi_to_XYZ(radius, theta, phi));
			if (a < divisions && b < divisions) {
				Triangles.Add((a + 1) * divisions + b);
				Triangles.Add(a * divisions + b);
				Triangles.Add((a + 1) * divisions + b + 1);
				Triangles.Add(a * divisions + b + 1);
				Triangles.Add((a + 1) * divisions + b + 1);
				Triangles.Add(a * divisions + b);
			}
			else if (a < divisions - 1) {
				Triangles.Add(a * divisions + b);
				Triangles.Add((a + 1) * divisions);
				Triangles.Add((a + 1) * divisions + b);
				Triangles.Add(a * divisions + b);
				Triangles.Add((a + 1) * divisions);
				Triangles.Add(a * divisions);
			}
		}
	}
	Verticies.Add(FVector(0.0f, 0.0f, radius));
	Verticies.Add(FVector(0.0f, 0.0f, -radius));
	for (int i = 0; i < divisions; i++) {
		Triangles.Add(divisions * (divisions + 1));
		Triangles.Add(i + 1);
		Triangles.Add(i);
		Triangles.Add(divisions * (divisions + 1) + 1);
		Triangles.Add(divisions * divisions + i - 1);
		Triangles.Add(divisions * divisions + i);
	}
	Triangles.Add(divisions * (divisions + 1));
	Triangles.Add(0);
	Triangles.Add(divisions);
	Triangles.Add(divisions * (divisions + 1) + 1);
	Triangles.Add(divisions * (divisions + 1) - 1);
	Triangles.Add(divisions * divisions - 1);
}

FVector RThetaPhi_to_XYZ(float R, float Theta, float Phi) {
	return FVector(R * sin(Theta) * cos(Phi), R * sin(Theta) * sin(Phi), R * cos(Theta));
}

