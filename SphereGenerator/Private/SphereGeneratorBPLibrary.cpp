// Copyright Epic Games, Inc. All Rights Reserved.

#include "SphereGeneratorBPLibrary.h"
#include "SphereGenerator.h"

USphereGeneratorBPLibrary::USphereGeneratorBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FVector rThetaPhi_to_XYZ(float R, float Theta, float Phi);
FVector translateToSphere(FVector original, float radius);

void USphereGeneratorBPLibrary::getRadialSphere(const float radius, const float divisions, TArray<FVector>& Verticies, TArray<int32>& Triangles) {
	for (float theta = PI / (divisions + 1), a = 0; theta < PI && a < divisions; theta += PI / (divisions + 1), a++) {
		for (float phi = 0, b = 0; phi < 2 * PI && b <= divisions; phi += 2 * PI / (divisions + 1), b++) {
			Verticies.Add(rThetaPhi_to_XYZ(radius, theta, phi));
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

void USphereGeneratorBPLibrary::getSquareSphere(const float radius, const float divisions, TArray<FVector>& Verticies, TArray<int32>& Triangles) {
	int side_divisions = floor(divisions / 2);
	float step_size = sqrt((radius*radius) / 3) * 2 / (side_divisions - 1);
	FVector corners[6];
	FVector first_add_vectors[6];
	FVector second_add_vectors[6];
	corners[0] = FVector(sqrt((radius*radius) / 3), -sqrt((radius*radius) / 3), sqrt((radius*radius) / 3));
	corners[1] = FVector(sqrt((radius*radius) / 3), sqrt((radius*radius) / 3), sqrt((radius*radius) / 3));
	corners[2] = FVector(-sqrt((radius*radius) / 3), sqrt((radius*radius) / 3), sqrt((radius*radius) / 3));
	corners[3] = FVector(-sqrt((radius*radius) / 3), -sqrt((radius*radius) / 3), sqrt((radius*radius) / 3));
	corners[4] = FVector(sqrt((radius*radius) / 3), sqrt((radius*radius) / 3), sqrt((radius*radius) / 3));
	corners[5] = FVector(sqrt((radius*radius) / 3), -sqrt((radius*radius) / 3), -sqrt((radius*radius) / 3));
	first_add_vectors[0] = FVector(0.0f, 1.0f, 0.0f);
	second_add_vectors[0] = FVector(0.0f, 0.0f, -1.0f);
	first_add_vectors[1] = FVector(-1.0f, 0.0f, 0.0f);
	second_add_vectors[1] = FVector(0.0f, 0.0f, -1.0f);
	first_add_vectors[2] = FVector(0.0f, -1.0f, 0.0f);
	second_add_vectors[2] = FVector(0.0f, 0.0f, -1.0f);
	first_add_vectors[3] = FVector(1.0f, 0.0f, 0.0f);
	second_add_vectors[3] = FVector(0.0f, 0.0f, -1.0f);
	first_add_vectors[4] = FVector(0.0f, -1.0f, 0.0f);
	second_add_vectors[4] = FVector(-1.0f, 0.0f, 0.0f);
	first_add_vectors[5] = FVector(0.0f, 1.0f, 0.0f);
	second_add_vectors[5] = FVector(-1.0f, 0.0f, 0.0f);
	for (int i = 0; i < 6; i++) {
		for (int x = 0; x < side_divisions; x++) {
			for (int y = 0; y < side_divisions; y++) {
				Verticies.Add(translateToSphere(corners[i] + first_add_vectors[i] * y * step_size + second_add_vectors[i] * x * step_size, radius));
				if (x < side_divisions - 1 && y < side_divisions - 1) {
					Triangles.Add(i * side_divisions * side_divisions + x * side_divisions + y);
					Triangles.Add(i * side_divisions * side_divisions + (x + 1) * side_divisions + y + 1);
					Triangles.Add(i * side_divisions * side_divisions + (x + 1) * side_divisions + y);
					Triangles.Add(i * side_divisions * side_divisions + x * side_divisions + y);
					Triangles.Add(i * side_divisions * side_divisions + x * side_divisions + y + 1);
					Triangles.Add(i * side_divisions * side_divisions + (x + 1) * side_divisions + y + 1);
				}
			}
		}
	}
}

FVector rThetaPhi_to_XYZ(float R, float Theta, float Phi) {
	return FVector(R * sin(Theta) * cos(Phi), R * sin(Theta) * sin(Phi), R * cos(Theta));
}

FVector translateToSphere(FVector original, float radius) {
	return original / ((original.Size()) / radius);
}

