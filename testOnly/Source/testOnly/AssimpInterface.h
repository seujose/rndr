// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "GameFramework/Actor.h"
#include "AssimpInterface.generated.h"

UCLASS()
class TESTONLY_API AAssimpInterface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAssimpInterface();

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool openMesh(FString path, int32& SectionCount, FString& ErrorCode);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FVector>& Tangents);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		void clear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 _selectedVertex;
	int32 _meshCurrentlyProcessed;
	bool _addModifier;
	int _lastModifiedTime;
	bool _requiresFullRecreation;

	TArray<TArray<FVector>> _vertices;
	TArray<TArray<int32>> _indices;
	TArray<TArray<FVector>> _normals;
	TArray<TArray<FVector2D>> _uvs;
	TArray<TArray<FVector>> _tangents;
	TArray<TArray<FColor>> _vertexColors;

	void processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);

};





