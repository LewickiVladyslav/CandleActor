#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Candle.generated.h"

UCLASS()
class BLOODPUDDLE_API ACandle : public AActor
{
	GENERATED_BODY()
	
public:	
	ACandle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flame Settings", meta = (ClampMin = "0.0", ClampMax = "16.0"))
	float DistortionStr = 3.5f;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Candle")
	UStaticMeshComponent* Wax;

	UPROPERTY(VisibleAnywhere, Category = "Candle")
	UStaticMeshComponent* Wick;

	UPROPERTY(VisibleAnywhere, Category = "Candle")
	UPointLightComponent* CandleLight;

	UPROPERTY(VisibleAnywhere, Category = "Candle")
	UNiagaraComponent* Flame;
	
	UPROPERTY(VisibleAnywhere, Category = "Candle")
	UNiagaraComponent* Smoke;

	UPROPERTY(EditAnywhere, Category = "Flame Settings")
	UMaterialParameterCollection* MaterialParameterCollection;

	bool bWasFlameActive = true;
	float PreviousDistortionStr = -1.0f;
	void UpdateMaterialParameter();
};
