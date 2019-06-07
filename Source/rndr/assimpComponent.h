#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimpComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RNDR_API UassimpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UassimpComponent();
	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool openMesh(FString path, int32& SectionCount, FString& ErrorCode);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		bool getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FVector>& Tangents, TArray<FVector>&Transform);

	UFUNCTION(BlueprintCallable, Category = "Assimp")
		void clear();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "assimp")
		int32 uvIndex;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
		int32 _selectedVertex;
		int32 _meshCurrentlyProcessed;
		bool _addModifier;
		int _lastModifiedTime;
		bool _requiresFullRecreation;

		TArray<FVector> _transform;
		TArray<TArray<FVector>> _vertices;
		TArray<TArray<int32>> _indices;
		TArray<TArray<FVector>> _normals;
		TArray<TArray<FVector2D>> _uvs;
		TArray<TArray<FVector>> _tangents;
		TArray<TArray<FColor>> _vertexColors;

		void processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
};
